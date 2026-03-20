# 技术设计

## 方案概要
采用“广播通知事务解耦 + 设备主动建链”的方式重构 GB 广播链路，使实现与 GB/T 28181-2022 第 9.12 节保持一致。

## 方案对比
### 方案1（最小兼容修补）
- 做法: 保留当前“平台后续主动 `INVITE`”模型，只修 `Notify` 字段、状态机和日志。
- 优点: 改动小、风险低、回归范围小。
- 缺点: 仍不符合 2022 版广播标准；遇到严格按标准实现的平台时仍可能失败。

### 方案2（2022 标准对齐，推荐）
- 做法: `Notify` 只回纯 SIP `200 OK`，再主动发广播应答 `MESSAGE`，随后由设备主动 `INVITE` 建立广播音频会话。
- 优点: 与标准一致，平台兼容性最好，链路语义清晰。
- 缺点: 需要调整 SDK 广播入口、会话状态机和日志，改动范围大于方案1。

## 推荐方案详细设计
### 1. 广播通知拆分为两步应答
- 修改 `CGBClientImpl::OnNotify()` 的广播分支。
- 第一步仅调用 `Response()` 返回纯 SIP `200 OK`，不再把 `Broadcast` XML 塞入事务响应。
- 第二步新增独立接口，使用 `m_sip_client->Message()` 发送广播业务应答 `MESSAGE`。
- 该应答复用 `Broadcast` XML，但发送方式应与 `DeviceControl`/`Catalog` 等独立 `MESSAGE` 保持一致。

### 2. 新增广播主动建链入口
- 在 `GB28181ClientSDK` / `CGBClientImpl` 中增加独立广播会话入口，例如 `StartBroadcastStreamRequest(...)`。
- 实现上可复用现有 `StreamRequest(...)` 主体能力，但不能复用“live”语义和日志。
- 入口负责构造广播音频 `MediaInfo`、发起 `INVITE`、接收 `200 OK`、发送 `ACK`，并输出 `StreamHandle`。

### 3. ProtocolManager 接管广播状态机
- 在 `ProtocolManager` 中扩展 `GbBroadcastSession`，新增以下状态:
  - `notify_received`
  - `notify_responded`
  - `invite_started`
  - `invite_established`
  - `bye_received`
- `HandleGbBroadcastNotify()` 只记录通知上下文，不再假设后续由平台来 `INVITE`。
- 新增一个“广播主动启动”入口，在广播应答 `MESSAGE` 成功后触发 `m_gb_client_sdk` 主动建链。

### 4. 广播 SDP 与传输方向
- 广播业务只涉及音频，SDP 使用 `m=audio`。
- 设备作为语音流接收者发起 `INVITE` 时，应由平台在 `200 OK` 中返回真正的媒体发送参数。
- 广播桥的 `ApplySdpOffer/ApplyTransportHint()` 要按“设备收流”语义重新确认方向，避免沿用实时流“设备发流”的思维。

### 5. 日志与可观测性
- 新增分阶段日志:
  - `gb broadcast notify sip-200 sent`
  - `gb broadcast response message sent/ok/fail`
  - `gb broadcast invite start`
  - `gb broadcast invite 200 ok`
  - `gb broadcast ack sent`
  - `gb broadcast media started`
- 失败日志要能区分:
  - 平台没回广播应答 `MESSAGE`
  - 平台没接受设备侧 `INVITE`
  - SDP/传输协商失败
  - 媒体已建链但广播桥收流/解码失败

## 影响范围
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp`
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.h`
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GB28181ClientSDK.cpp`
- `third_party/platform_sdk_port/CommonFile/CommonLib/GB28181ClientSDK.h`
- `App/Protocol/ProtocolManager.cpp`
- `App/Protocol/ProtocolManager.h`
- `App/Protocol/gb28181/GB28181ClientReceiverAdapter.cpp`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`

## 风险与规避
- 风险: 部分平台可能仍沿用旧版“平台主动 `INVITE`”流程。
- 规避: 设计为可配置兼容模式，保留旧流程开关，默认走 2022 标准。
- 风险: 广播主动建链与现有 live/playback 共用 `StreamRequest()` 可能混淆会话类型。
- 规避: 广播单独封装入口和日志标签，不直接复用 live 业务层。
- 风险: 主动建链后 SDP 方向理解错误，导致广播桥仍按发送端配置。
- 规避: 在广播会话上单独校验 `remote/local` 媒体语义，并增加方向日志。

## ADR
### ADR-20260320-broadcast-2022
- 决策: 广播链路按 GB/T 28181-2022 第 9.12 节重构为“纯 SIP 事务响应 + 独立广播应答 `MESSAGE` + 设备主动 `INVITE`”。
- 原因: 当前 issue 已证明旧流程与标准平台存在明显兼容性偏差，继续修补字段无法根治。
