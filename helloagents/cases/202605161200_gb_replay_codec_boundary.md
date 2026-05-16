# GB 回放 codec 边界 case

## 结论
`262.pcap` 里的两次 GB 回放 `INVITE` / `200 OK`，`f=` 都是 `v/2/6/25/1/1024a/1/8/1`。结合同事反馈，第一路会话的问题不应只看抓包字符串，而要看“会话对应文件”和“codec 探测文件”是否一致。

更像的问题是：回放建链只按首个真正参与播放的录像文件 codec 回填 `200 OK`，但实际发帧仍按每个 MP4 demuxer 的 `iCodeType` 走。只要一个回放区间跨越 H264/H265 文件边界，平台客户端如果默认“一次会话只支持一种 codec”，就可能出现有的能播、有的不能播。

## 证据
- `debug.log`:
  - 播放线程曾先打开边界 H265 文件 `20260516110525-20260516110543.mp4`，但又 seek 到文件末尾并 EOF
  - 原协议探测按 `iEndTime <= startSec` 跳过边界文件，随后打开下一段 H264 文件并回 `f=v/2...`
  - `RespondGbMediaPlayInfo()` 和 `SendVideoFrameByCodecType()` 分别负责建链应答和逐帧封装
- `App/Protocol/ProtocolManager.cpp`:
  - `ResolveGbReplayFileVideoCodec()` 行为见 `4623-4680`
  - `BuildGbResponseMediaInfo()` 行为见 `11401-11516`
  - `HandleGbReplayStorageFrame()` 行为见 `12208-12410`
- `App/Storage/common/Mp4_Demuxer.cpp`:
  - `GetVideoCodecType()` 按文件返回 `iCodeType`

## 备注
修复方向不是修改 `VideoFormat=2/5` 映射，而是让 codec 探测更贴近实际录像文件：优先看文件名里的 `h264/h265`，没有标记时再打开 MP4 读 moov/stream info。

## 推荐处理
GB 回放会话应保持建链 SDP 中 `f=` 协商出的单一视频编码。遇到回放区间跨 H264/H265 文件边界时，设备侧优先只播放首个连续同编码片段；后续文件 codec 与应答 codec 不一致时，应结束当前回放会话并让平台重新发起下一段，或由平台按录像文件编码分段请求。

同一个 RTP/PS 会话里静默切换 H264/H265 对兼容性风险最大，即使 PS stream id 可以按帧切换，也不能假设所有平台客户端都能接受。
