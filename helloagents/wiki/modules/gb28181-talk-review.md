# GB28181 对讲业务审查记录

## 1. 文档用途
- 本文档用于沉淀 `rk_gb` 当前 GB28181 对讲链路的代码现状、审查结论和后续待办。
- 审查基线以 [`gb28181-2022-baseline.md`](gb28181-2022-baseline.md) 和 [`protocol.md`](protocol.md) 为准。
- 本文档关注的是代码实现审查，不代表平台侧联调结果已经完全覆盖。

## 2. 相关代码入口
- `App/Protocol/gb28181/GB28181ClientReceiverAdapter.cpp`
  - `OnStreamRequest()` 将 `kAudioStream` 转发给 `ProtocolManager::HandleGbAudioStreamRequest()`
- `App/Protocol/ProtocolManager.cpp`
  - `HandleGbAudioStreamRequest()`
  - `HandleGbStreamAck()`
  - `HandleGbStopStreamRequest()`
  - `BuildGbBroadcastMediaInfo()`
- `App/Protocol/gb28181/GB28181BroadcastBridge.*`
  - 负责当前音频接收、RTP 解包、解码和本地播音
- `App/Protocol/gb28181/GB28181ListenBridge.*`
  - 负责本地采音、编码和 RTP 发送
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp`
  - `OnInvite() / OnACK() / OnBye()`

## 3. 当前实现摘要
- 当前 `Talk` 被识别为 `kAudioStream`，SDP `s=Talk` 可正常进入音频请求分支。
- 被动 `Talk INVITE` 现在走独立的 `talk session`：
  - `HandleGbAudioStreamRequest()` 解析平台 SDP 中的音频 IP、端口、payload、codec
  - 回 `200 OK` 时使用独立的 `gb_talk.recv_port`
  - 在 `ACK` 之后才把协商结果应用到 `m_talk_broadcast`
  - 同步按协商结果启动/停止上行 `GB28181ListenBridge`
- 接收到的音频数据仍会在设备本地解码播音；若协商 transport 支持，设备也会向平台回送采音 RTP。
- `GbTalkParam` 已接入对讲链路，当前实际生效字段为 `codec`、`recv_port`、`sample_rate`；`jitter_buffer_ms` 已落入配置但尚未形成真正的抖动缓冲实现。

## 4. 审查结论

### 4.1 `Talk` 已从 `Broadcast` 会话中拆出
- 当前 `Talk` 使用独立的 `m_gb_talk_session` 和 `m_talk_broadcast`。
- 广播 `notify -> invite -> ack/bye` 仍保留在 `m_gb_broadcast_session`，不会再被被动 `Talk INVITE` 覆盖。
- 这一步已经完成此前 P0 里“会话隔离”的核心目标。

### 4.2 已补上基础双向闭环，但仍存在 transport 限制
- 被动 `Talk INVITE` 在 `ACK` 后会启动下行播音和上行采音发送。
- 对 `UDP`、以及需要设备主动 `connect()` 的 `TCP active` 场景，当前链路可以建立完整闭环。
- 对“平台主动连设备、设备需复用同一条已接入 TCP 回传音频”的场景，当前 `GB28181ListenBridge` 还不支持复用广播桥接受到的 socket，因此该类场景仍未闭环。

### 4.3 `gb_talk` 已从“死配置”变为实际配置
- `ProtocolExternalConfig::GbTalkParam` 现在包含 `codec`、`recv_port`、`sample_rate`、`jitter_buffer_ms`。
- `LocalConfigProvider` / `HttpConfigProvider` 已支持这些字段的默认值、校验和落盘/序列化。
- 被动对讲 `200 OK` 使用 `gb_talk.recv_port`，上行采音桥使用协商 codec 与 `gb_talk.sample_rate`。

### 4.4 建链时序已收敛到 `ACK` 后
- 被动 `Talk INVITE` 不再在 `200 OK` 之前执行 `ApplyTransportHint()`。
- 现在是先应答 SIP，再在 `HandleGbStreamAck()` 中启动媒体链路，避免把网络建链异常前移到 `INVITE` 阶段。

### 4.5 `Subject` 解析已从“定长截断”改为“按字段提取”
- SDK `OnInvite()` 不再简单截取前 20 字节。
- 当前按 `Subject` 的 `设备ID:流号,对端ID:流号` 结构提取首段设备 ID，能避免额外字段造成的误判。

### 4.6 音频协商兼容性已提升，但仍需联调验证
- 当前不再强依赖 SDP 中第一个 payload，而是会在多个 `rtpmap` 中优先选择本端支持 codec。
- 现阶段仍需通过真实平台包进一步验证多 payload、静态 payload、以及 `PCMA/PCMU` 混排场景。

## 5. 风险判断
- 若平台以 `UDP` 或设备主动 `TCP` 建链，对讲链路已经具备基础双向能力。
- 若平台要求设备在 `TCP passive` 场景下复用已建立的下行连接回传上行音频，则当前实现仍不完整。

## 6. TODO

### P0
- 基于真实平台抓包验证当前实现的三类场景：
  - `UDP`
  - 设备主动 `TCP active`
  - 平台主动 `TCP passive`

### P1
- 若要完整支持 `TCP passive` 双向对讲，需要让上行发送复用 `m_talk_broadcast` 接受到的已连接 socket，或重构 `GB28181ListenBridge` 的传输层。
- 把 `jitter_buffer_ms` 变成真正的接收缓冲/播放抖动控制，而不是仅停留在配置字段。

### P2
- 为对讲补齐更细的联调日志：
  - `invite accepted`
  - `ack apply transport`
  - `uplink start/stop`
  - `listen restore`

### P3
- 补充对讲链路联调 checklist：
  - `INVITE -> 200 OK -> ACK` 是否完整
  - `m=audio`、`payload/rtpmap`、`setup` 是否一致
  - 设备是否只在会话确认后认为链路稳定
  - 若为双向对讲，是否存在稳定的上行采音 RTP

## 7. 结论
- 当前代码已经完成“对讲会话拆分、配置归位、`ACK` 后建链、基础上行音频回送”这几个核心修复。
- 下一阶段的重点不再是会话模型，而是补齐 `TCP passive` 双向回传和 `jitter_buffer_ms` 的真实生效点。
