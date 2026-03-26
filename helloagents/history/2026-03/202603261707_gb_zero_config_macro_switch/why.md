# 变更提案: GB28181 零配置宏开关与注册重定向闭环

## 需求背景
当前仓库的 GB28181 注册链路只覆盖标准国标流程：`REGISTER -> 401 -> Authorization REGISTER -> 200 OK`。知识库已经明确记录了中国移动终端白皮书中的“零配置 / 注册重定向”闭环，但代码仍存在几处关键缺口：

- 缺少编译期能力开关，无法在“标准国标流程”和“零配置流程”之间稳定切换。
- 本地 GB 配置模型不包含 `StringCode`、`Mac`、重定向域、重定向 `SIPID`、`CustomProtocolVersion` 等零配置字段。
- 首次重定向 `REGISTER` 仍使用正式 `device_id` 语义，没有用 `StringCode` 身份，也没有附加白皮书要求的扩展头。
- SDK 注册状态机没有显式消费 `302 Moved`，也不会切换到平台返回的正式地址和 `deviceId` 再完成二次注册。

本次变更目标是先落下“最小可联调零配置闭环”，同时保留标准国标流程不受影响。

## 变更内容
1. 引入编译期宏 `PROTOCOL_ENABLE_GB_ZERO_CONFIG`，宏开启走零配置重定向注册，宏关闭保持现有标准 GB 注册链路。
2. 扩展 GB 注册配置与本地持久化，补齐零配置首包和重定向所需的关键字段。
3. 在 SIP/GB SDK 注册链路中实现首次 `StringCode` 身份、扩展头组包、`302` 解析和正式平台注册切换。
4. 在 `ProtocolManager` 中补零配置模式下的注册参数装配与重试节奏。

## 影响范围
- **模块:** `ProtocolManager`、本地配置提供器、SIP SDK、GB28181 Client SDK
- **文件:** `rk_gb/App/Protocol/ProtocolManager.*`、`rk_gb/App/Protocol/config/*`、`rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/*`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/SipSDK/**/*`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/*`
- **API:** GB 注册配置接口、SIP `REGISTER`
- **数据:** `GbRegisterParam`、`GBRegistParam`、`ConnectParam`、`SipRegistParam`、`SipData`

## 核心场景

### 需求: 编译期切换 GB 标准/零配置流程
**模块:** `ProtocolManager` / 编译配置
同一套仓库需要同时支持标准国标接入和零配置重定向接入。

#### 场景: 宏关闭
构建时未开启零配置宏。
- 注册链路保持原有 `401 -> 200` 标准国标行为
- 不要求配置 `StringCode`、`Mac`、重定向扩展字段

#### 场景: 宏开启
构建时开启零配置宏。
- 注册链路切换为 `StringCode -> 401 -> 302 -> 正式平台注册 -> 401 -> 200`
- `302` 前后身份和目标地址按白皮书要求切换

### 需求: 首次 REGISTER 携带零配置身份和扩展头
**模块:** SIP SDK / GB28181 Client SDK
首个重定向 `REGISTER` 必须满足白皮书 `A.16` 的字段要求。

#### 场景: 首次向重定向服务器注册
设备尚未取得正式 `deviceId`。
- `From/To/Contact` 中的本地身份填写 `StringCode`
- 请求头带上 `Mac/StringCode/Line/Manufacturer/Model/Name/CustomProtocolVersion`

### 需求: 收到 302 后切换正式平台完成注册
**模块:** GB28181 Client SDK
通过重定向服务器鉴权后，需要从 `302` 响应中提取正式平台信息并继续注册。

#### 场景: 重定向服务器返回正式平台信息
第二次带鉴权的重定向 `REGISTER` 收到 `302 Moved`。
- 解析 `Contact/ServerDomain/ServerId/ServerIp/ServerPort/deviceId`
- 切换为正式平台地址与 `deviceId`
- 再次完成正式平台 `401 -> Authorization -> 200`

### 需求: 零配置模式下的失败重试
**模块:** `ProtocolManager`
知识库要求正式平台注册失败时按固定节奏重试，必要时回到重定向阶段重新拿最新地址。

#### 场景: 正式平台注册连续失败
零配置已开启，且当前未注册成功。
- 单次失败后 30 秒重试正式注册
- 连续失败达到 3 次后，等待 1 分钟重新发起重定向注册

## 风险评估
- **风险:** SIP/GB SDK 公共结构体扩展后会影响现有注册链路和日志字段。
- **缓解:** 用编译期宏隔离零配置主流程，标准流程默认保持原样。
- **风险:** `302` 平台字段来源可能存在多种携带方式（`Contact`、普通 SIP Header）。
- **缓解:** 本次同时支持 `Contact` 解析和扩展响应头读取，并保留详细日志便于联调抓包核对。
