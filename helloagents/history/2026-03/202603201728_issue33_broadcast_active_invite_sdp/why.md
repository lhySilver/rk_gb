# 变更提案

## 背景
- 同事提供的 `168.pcap` 和 `debug.log` 显示，设备在广播 `Notify` 后已经主动发起音频 `INVITE`，并收到了平台 `100 Trying` 与 `200 OK`。
- 现网现象仍然是“没有流发上去”，日志中对应错误为 `gb broadcast active invite parse answer failed`，抓包中紧跟着出现设备主动发送 `BYE`。
- 这说明问题不在广播前半段，而在设备对主动 `INVITE` 的 `200 OK` SDP 应答解析和后续媒体建链。

## 目标
- 修复广播主动 `INVITE` 对平台 `200 OK` SDP 的解析，使业务层能正确识别音频 payload、编码和端口。
- 保留足够日志，便于继续区分“SDP 解析失败”和“ACK/媒体传输异常”。

## 核心场景
- 平台返回 `200 OK`，SDP 中包含 `m=audio 16140 RTP/AVP 8` 与 `a=rtpmap:8 PCMA/8000`。
- 设备当前 SDK 能把 SDP 解析成 `MediaInfo` 基本字段，但没有把 `RtpDescri.mapDescri` 填出来，导致业务层取不到 codec/payload。

## 成功标准
- `HandleGbBroadcastNotifyResponse()` 不再把这类有效 `200 OK` 误判为 `parse answer failed`。
- 设备不再在收到有效 `200 OK` 后立刻主动 `BYE`。
- 代码可在隔离交叉编译目录下通过 `dgiot` 构建。
