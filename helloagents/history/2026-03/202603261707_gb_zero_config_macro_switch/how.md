# 技术设计: GB28181 零配置宏开关与注册重定向闭环

## 技术方案

### 核心技术
- C++ `ProtocolManager` 生命周期管理
- 本地 GB 配置持久化
- SIP `REGISTER` 组包与响应解析
- GB28181 Client SDK 注册状态机

### 实现要点
- 通过编译期宏 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 切换两条注册链路，避免把零配置逻辑侵入标准国标路径。
- `ProtocolManager` 只负责装配“标准注册参数”或“零配置注册参数”，具体 `302` 消费和二次注册仍放在 GB SDK 内部。
- 扩展 `SipRegistParam`/`GBRegistParam`/`ConnectParam`/`SipData` 承载零配置首包扩展头和 `302` 返回字段。
- SIP SDK 在发送 `REGISTER` 前统一写入扩展头，在响应解析时补齐 `Contact` 和命名响应头，减少 GB SDK 直接碰 `osip_message_t` 的耦合。
- 零配置重试状态优先放在 `ProtocolManager::GbHeartbeatLoop` 中维护，避免把生命周期节奏塞进底层 SIP/GB SDK。

## 架构决策 ADR

### ADR-20260326-01: 用编译期宏隔离零配置流程
**上下文:** 用户要求宏开启走零配置，宏关闭保持标准国标流程，且当前零配置字段和状态机会影响现有注册逻辑。
**决策:** 新增 `PROTOCOL_ENABLE_GB_ZERO_CONFIG`，默认值由公共头提供，构建脚本显式透传。
**理由:** 这是最直接、可控的隔离方式；标准路径无需承受零配置联调改动风险。
**替代方案:** 用运行时配置开关在同一条链路里分支处理 → 拒绝原因: 会扩大回归面，也不符合本次“用宏切流程”的明确要求。
**影响:** 需要保证 App 层和 SDK 层都能见到同一个宏定义。

### ADR-20260326-02: 302 消费放在 GB SDK 内部，重试节奏放在 ProtocolManager
**上下文:** `302` 解析与正式平台注册本质上属于“注册状态机”的一部分，但注册失败后的循环重试又依赖上层生命周期线程。
**决策:** 底层 SDK 负责一次“零配置注册尝试”内部的 `401/302/401/200`；`ProtocolManager` 负责 30 秒和 1 分钟的外层重试节奏。
**理由:** 这样能保持 SDK 关注单次注册事务，上层保留生命周期控制权。
**替代方案:** 全部塞进 SDK → 拒绝原因: 生命周期状态和线程等待逻辑会下沉到 SDK，后续维护更混乱。
**影响:** `ProtocolManager` 需要维护零配置失败计数和阶段性重定向刷新。

## 数据模型
```text
GbRegisterParam:
  + string_code
  + mac_address
  + line_id
  + redirect_domain
  + redirect_server_id
  + custom_protocol_version
  + manufacturer
  + model

GBRegistParam:
  + use_zero_config
  + auth_username
  + display_name
  + string_code
  + mac_address
  + line_id
  + custom_protocol_version
  + manufacturer
  + model

ConnectParam:
  + server_domain
  + server_id

SipData:
  + Contact
  + ServerDomain
  + ServerId
  + ServerIp
  + ServerPort
  + DeviceId
```

## 安全与性能
- **安全:** 日志中对口令字段继续打码，零配置新增字段只打印必要身份信息，不回显明文密码。
- **安全:** `302` 解析前检查地址、端口和 `deviceId` 合法性，避免把空值或非法目标写回正式注册。
- **性能:** 仅扩展控制面结构和注册状态机，不触碰实时音视频链路。

## 测试与部署
- **测试:** 至少验证两条路径：
  - 宏关闭时标准注册仍是 `401 -> 200`
  - 宏开启时首次 REGISTER 带扩展头，且能消费 `302` 后再次完成正式注册
- **测试:** 日志核对 `StringCode` 身份、`302` 平台字段、正式平台注册成功三个关键点。
- **部署:** 默认不改现网行为，只有显式打开 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 才进入新流程。
