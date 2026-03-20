# 技术设计

## 根因
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/SDP/SdpUtil.cpp` 中的 `CSdpUtil::String2MediaInfo()` 只解析了 `IP/Port/RequestType/RtpType`，没有把 `m=` 行里的 payload 和 `a=rtpmap` 解析回 `MediaInfo.RtpDescri`。
- `App/Protocol/ProtocolManager.cpp` 的 `HandleGbBroadcastNotifyResponse()` 依赖 `ExtractAudioTransportHint()` 从 `answer.RtpDescri` 读取 payload 和 codec；由于该结构为空，业务层把有效应答误判为失败，并调用 `StopStreamRequest()` 发送 `BYE`。
- 同一处 SDP 解析还有一个历史错误：`media type` 的 audio/video 标记写反了。

## 实施方案
- 在 `String2MediaInfo()` 中补齐：
  - `m=` payload 列表解析；
  - `a=rtpmap` 到 `RtpMap` 的映射；
  - 静态 payload 的缺省回填（0 -> PCMU/8000，8 -> PCMA/8000）；
  - `RtpDescri.type` 的音视频修正。
- 在 `HandleGbBroadcastNotifyResponse()` 中：
  - 失败日志打印 `IP/Port/transport/descriNum/mime/payload/rate`；
  - 释放 `answer.RtpDescri.mapDescri`，避免主动 `INVITE` 成功路径和失败路径都遗留内存。

## 风险与规避
- 风险：`String2MediaInfo()` 被 GB 其他流程复用，修改后可能影响被动 `INVITE` 解析。
- 规避：保持现有返回结构不变，只补缺失字段；同时修正明显错误的 media type 赋值。

## 验证
- 用 `/tmp/rk_gb_issue168_build` 隔离目录重新配置并交叉编译 `dgiot`。
- 后续抓包重点确认：
  - 收到广播 `200 OK` 后不再立刻发送 `BYE`；
  - 日志不再出现 `gb broadcast active invite parse answer failed`；
  - 如仍无音频，再继续检查 ACK 目标地址和 RTP 实际到达情况。
