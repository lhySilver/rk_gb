# Protocol 模块

## 职责
- 管理 GB28181 与 GAT1400 生命周期。
- 处理 SIP/SDP 请求到本地媒体发送链路的映射。
- 统一输出协议日志与错误信息。

## 当前重点
- GB28181 本地配置文件读取、缺省值落盘和开关控制。
- GB 注册与鉴权稳定性。
- GB 注册成功后的时间同步与设备本地时间持久化。
- GB 实时流在 UDP/TCP 模式下的媒体协商兼容性。
- GB `RecordInfo` 查询与本地录像检索结果映射。
- 1400 注册、保活与基础能力补齐。

## 标准依据
- `GB/T 28181-2022` 的项目内调试基线见 [`gb28181-2022-baseline.md`](gb28181-2022-baseline.md)。
- 涉及注册、保活、实时流、回放、下载、广播、校时等流程时，优先以该基线文档中的章节和抓包检查项为准，再回看实现细节。
- 当前工程的关键落点集中在 `ProtocolManager`、`GBClientImpl`、`GB28181ClientReceiverAdapter`、`GB28181RtpPsSender` 和 `GB28181BroadcastBridge`。

## 注意事项
- `LocalConfigProvider` 现在会优先读取 `/userdata/conf/Config/gb28181.ini`；文件不存在时使用代码默认值并自动生成。
- `gb28181.ini` 现已支持 `image_flip_mode`，取值会映射到设备主配置 `CFG_CAMERA_PARAM.rotateAttr`，由 Camera/Alarm 既有回调统一生效。
- `gb_register.enabled` 为 `0` 时，`ProtocolManager` 会跳过 GB client 生命周期启动与重注册，但 GAT1400 相关配置校验和生命周期不受影响。
- GB `TeleBoot` 远程重启已接入冷却保护；短时间内重复命令会被拒绝并打印 `gb teleboot rejected` 日志。
- GB 广播通知 `MESSAGE` 只能对同一事务返回一次最终 SIP 响应；不要先发空 `200 OK` 再补带 XML 的 `Response`，否则平台只会收到前一个无业务体的应答。
- GB 广播 `Notify` 的业务 XML 响应必须回填平台原始 `SN`，并以 `TargetID` 作为响应 `DeviceID`；如果返回成 `SN=0` 或空设备编号，平台通常不会继续进入后续广播流程。
- 针对 GB/T 28181-2022 第 9.12 节广播流程，设备当前实现已调整为“`Notify` 先回纯 SIP `200 OK` -> 独立发送 `Broadcast` 应答 `MESSAGE` -> 设备主动发起 `Talk` 音频 `INVITE`”；仍保留被动 `INVITE` 处理链作为兼容兜底。
- GB 广播 `notify -> invite -> ack/bye` 现在有独立会话状态；`notify` 会校验 `TargetID` 是否匹配本机，`INVITE` 的音频 `200 OK` 会优先返回设备侧可达 IP，`BYE` 后会清理播音状态并把广播桥恢复到待机接收态。
- TCP 实时流要区分 `setup:active` 与 `setup:passive` 方向。
- SIP `INVITE` 应答不要被同步媒体建链阻塞。
- 对于 `MESSAGE` 类命令，要先区分“本事务立即返回纯 SIP `200 OK`”和“后续独立发送业务应答 `MESSAGE`”两层语义，不能混用。
- `Keepalive`、控制类应答、查询类应答、报警/目录通知确认都需要分别核对 `MESSAGE/200 OK` 与 XML 业务体的对应关系。
- 广播主动 `INVITE` 的 `200 OK` 若已收到却立刻出现 `parse answer failed` / `BYE`，先检查 SDP 解析结果里是否真正回填了 `payload/rtpmap`，不要只看 `IP/Port`。
- GB 回放音频不能按原始 PCM 直接塞给 RTP PS；应与实时流保持一致，按 SDP/`gb.live.audio_codec` 输出 `G711A` 或 `G711U`。
- GB 回放/下载回调需要带 session 身份；否则旧存储线程收尾时的 EOS 可能误伤新回放 session，表现为 ACK 已到但媒体立即 `eos video=0 audio=0`。
- GB 回放依赖 MP4 demux 连续读到有效 ES；对“packet 已读到但当前未产出有效音视频载荷”的场景不能直接当 EOF，否则会在首包前提前结束回放。
- 当 GB 回放仍在 `play-start` 后立即 `eos` 时，要同时观察存储层日志：先确认索引是否装载成功、是否选中目标录像文件，再区分 open/read 失败还是协议发送失败。
- GB 回放选择首个录像文件时，不能依赖 `index` 原始顺序和“包含开始时间”的单一条件；应先对有效记录按开始/结束时间排序，再按与请求区间重叠或紧随其后的文件进行匹配。
