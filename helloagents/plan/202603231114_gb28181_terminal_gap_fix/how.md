# 技术设计: GB28181终端白皮书差距收敛

## 技术方案
### 核心技术
- C++ 终端协议栈
- GB28181 SDK Client 与 XML 编解码
- SIP/SDP/RTP 广播链路
- 本地配置与 HTTP 配置双来源模型

### 实现要点
- 先做结构建模，再做业务接线，避免 App 层和 SDK 字段含义不一致。
- 广播链路优先保证公网 `TCP active` 闭环，`VoiceFlowMode` 查询/配置结果必须与实际协商模式一致。
- 注册重定向单独处理“首包身份”和“正式平台注册身份”，避免首包仍错误使用正式 `deviceId`。
- `DeviceInfo` 先补“白皮书必填字段 + 最小能力清单”，不要一次性引入过重的全量能力模型。
- 告警扩展和多码流能力放入第二阶段，前提是平台先确认告警编码和多码流验收口径。

## 架构决策 ADR
### ADR-20260323-01: 先做 P0 最小闭环，再扩展 P1 能力
**上下文:** 白皮书缺口横跨广播、注册、查询应答、配置和告警，且多个结构体位于共享 SDK 中，直接一次性全量重构风险高。
**决策:** 第一阶段只补联调阻塞项：`TCP active` 广播、注册重定向扩展字段、`DeviceInfo` 扩展、`BasicParam` 扩展；告警扩展和多码流能力作为第二阶段。
**理由:** 先恢复广播和注册联调，再扩展低频或平台口径未定的能力，能降低回归面并加快验证节奏。
**替代方案:** 一次性补齐白皮书全部字段和能力 → 拒绝原因: 共享结构改动面太大，且 `A.11/A.14/A.15` 仍有口径冲突。
**影响:** 需要维护 P0/P1 两套验收清单，但整体风险更可控。

### ADR-20260323-02: `VoiceFlowMode=1` 统一落到广播 `TCP active`
**上下文:** 白皮书 `A.14/A.15` 对 `VoiceFlowMode=1` 的文字描述存在差异，但公网平台广播场景需要终端主动向平台媒体地址发流。
**决策:** 广播能力默认按 `TCP active` 建模，配置查询/应答也以此作为 `VoiceFlowMode=1` 的实现口径。
**理由:** 与公网联调诉求一致，也与附录 `A.1` 的“TCP 主动拉流模式语音广播”一致。
**替代方案:** `VoiceFlowMode=1` 仅表示泛化的 `TCP`，运行时再由其他配置决定 `active/passive` → 拒绝原因: 平台验收和抓包分析都更难收敛。
**影响:** 需要同时检查 SDP 解析、媒体目标地址选择和配置回显。

## 数据模型
```text
CfgBasicParam / BasicSetting:
  + VoiceFlowMode
  + LocalPort
  + Domain
  + SipIp
  + SipPort
  + SipId
  + Password
  + ChannelList

DeviceInfo:
  + StringCode
  + Mac
  + Line
  + CustomProtocolVersion
  + DeviceCapabilityList
  + ProtocolFunctionList

AlarmNotifyInfo:
  + 扩展告警类型口径
  + EventType / ExtraInfo 结构化承载
```

## 安全与性能
- **安全:** 注册重定向和配置应答涉及设备身份、口令和平台地址，日志中不得明文泄露敏感字段。
- **安全:** 广播 `ACK` 校验、SDP 目标地址解析和配置来源切换需要做输入合法性检查，避免用错媒体地址。
- **性能:** 能力清单和扩展字段仅影响控制面 XML/SIP 组包，对实时媒体性能影响有限。

## 测试与部署
- **测试:** 以抓包 + debug 日志双视角验证，分别覆盖 `REGISTER` 重定向、广播 `INVITE/200/ACK/发流`、`DeviceInfo`、`BasicParam`、扩展告警。
- **测试:** 公网广播验证时，必须确认最终发流目标来自平台回复 SDP，而不是本地预置地址。
- **部署:** 先在联调环境灰度验证 P0；P1 等平台确认告警和多码流口径后再合入。
