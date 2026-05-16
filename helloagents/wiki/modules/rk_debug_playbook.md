# RK IPC 排查手册

## 目的
整理当前 RK IPC 工程常用的构建、日志、抓包、实时流和录像回放排查路径，帮助定位问题发生在协议协商、媒体配置、DMC 分发、存储索引还是 RTP/PS 发送阶段。

## 模块概述
- **职责:** 提供可复用的设备端调试清单和日志关键字
- **状态:** ✅稳定
- **最后更新:** 2026-05-16
- **代码真实来源:** `App/Main.cpp`、`App/Protocol/ProtocolManager.cpp`、`App/Media/*`、`App/Storage/*`、`Middleware/Include/PAL/*`

## 规范

### 需求: 按证据定位媒体/协议问题
**模块:** RKDebugPlaybook
排查时先固定输入证据：平台抓包、设备 `debug.log`、当前分支、板型宏、录像文件和配置文件。不要直接从 SIP 错误码推断根因，应沿“协议解析 -> 媒体配置 -> DMC 回调 -> 存储/发送”的链路逐段切分。

#### 场景: 平台拉流失败
前置条件:
- 有平台抓包和设备日志
- 设备已经注册到 GB 平台
- 预期结果1: 先确认 `INVITE` 的 SDP、`f=`、transport、SSRC、端口和编码请求
- 预期结果2: 再确认设备是否应用编码参数、订阅 DMC、收到帧、发出 RTP/PS
- 预期结果3: 最后判断失败是协商拒绝、设备无帧、封装错还是网络发送失败

#### 场景: 回放 H264/H265 混合录像失败
前置条件:
- 同一时间段或同一天存在不同 codec 的录像文件
- 平台多次回放成功/失败表现不一致
- 预期结果1: 查 `gb replay codec probe` 日志确认建链使用的录像真实 codec
- 预期结果2: 查 demuxer `iCodeType` 与 `SendVideoFrameByCodecType`，确认每帧封装 codec
- 预期结果3: 查存储索引和 MP4 文件路径，确认是否选错文件或沿用旧会话状态

## 通用排查顺序

1. **固定版本:** `git status --short`、`git log -1 --oneline`，确认当前分支和未提交改动。
2. **固定板型:** 看 `build.sh` 默认值或实际 CMake 参数，确认 `RC0240/RC0240V20/RC0240V30/RC0240V40/RC0240_LGV10`。
3. **固定证据:** 保存 `debug.log`、pcap、相关 MP4/index 文件路径，不要只记录结论。
4. **先看协议入口:** SIP/SDP 是否进入对应 handler，是否回了 200/403/BYE。
5. **再看媒体应用:** 编码、OSD、翻转等是否按 `query -> compare -> apply` 下发。
6. **最后看数据面:** DMC 是否有帧，storage 是否有文件，RTP/PS 是否按正确 codec 发出。

## 常用日志位置

| 证据 | 位置/来源 | 说明 |
|------|-----------|------|
| 设备运行日志 | `/mnt/sdcard/debug.log` | `App/Main.cpp` 中存在 SD 卡日志重定向路径线索 |
| 平台抓包 | 用户提供的 `.pcap` | 用于确认 `INVITE`、SDP `f=`、RTP payload、BYE/TEARDOWN |
| 录像文件 | `/mnt/sdcard/DCIM/YYYY/MM/DD/*.mp4` | 与同目录 index 配合定位回放文件 |
| 协议配置 | `/userdata/conf/Config/GB/*.ini` | GB28181/GAT1400/零配置入口 |
| 媒体配置 | `CFG_VIDEO`、`CFG_OSD_TIME`、`CFG_OSD_TEXT`、`CFG_CAMERA_PARAM` | 由 `IConfigManager` 读取并触发 `applyOK` |

## 实时流排查

| 检查点 | 日志/代码关键字 | 判断 |
|--------|-----------------|------|
| SDP 是否解析 | `f=`、`streamnumber`、`HandleGbLiveStreamRequest` | 解析失败通常应在协议层直接暴露 |
| 编码是否下发 | `[VideoEncodeControl] apply cfg_video` | ret=0 且有差异字段才说明触发写回 |
| 是否误拒流 | `dispatch_nonfatal`、`403 Forbidden` | live `f=` 应尽量 best-effort，不应因非关键参数直接拒绝 |
| DMC 是否订阅 | `CaptureSetStreamCallBack`、`kGbLiveDmcModuleName` | 订阅失败会导致无帧 |
| 是否有关键帧 | `DMC_MEDIA_SUBTYPE_IFRAME`、发送统计 | 首帧或 codec 切换后缺 I 帧会影响平台解码 |
| 网络发送是否失败 | `GB28181RtpPsSender`、`EAGAIN`、`EWOULDBLOCK` | TCP 回压和断连要和编码问题分开 |

## 录像/回放排查

| 检查点 | 日志/代码关键字 | 判断 |
|--------|-----------------|------|
| 是否选到文件 | `playback request enter`、`date_index_start`、`no index match` | 没选到文件不是 codec 问题 |
| 建链 codec | `gb replay codec probe codec=` | SDP 应按这里的真实 codec 回 |
| MP4 codec | `CMp4Demuxer::GetVideoCodecType()` | H264/H265 应映射到 `iCodeType=1/2` |
| 每帧 codec | `frameInfo->iCodeType`、`SendVideoFrameByCodecType` | 发送阶段必须按帧 codec，不只按会话 codec |
| EOS 竞态 | `stale eos ignored` | 旧存储线程 EOS 不应清掉新会话 |
| PLAY 门控 | `gb replay frame gated before play-start` | ACK/PLAY 前有帧被 gate 是预期行为，长期 gate 才异常 |

## 编码配置排查

| 问题 | 优先确认 |
|------|----------|
| 平台要求 H264 但设备仍发 H265 | `CFG_VIDEO.chan[stream].enc_type` 是否已从 `1` 改到 `0`，`CaptureChangeEncParam()` 是否触发 |
| 帧率/码率不变 | `ApplyCfgVideoEncodeStreamConfig()` 是否判断有差异并一次 `setConfig(..., applyOK)` |
| 分辨率不变 | 当前 `rk_video_set_resolution()` 是兼容桩，不能当作真实 RK 分辨率重配 |
| 码率控制模式不变 | 当前 `rk_video_set_RC_mode()` 基本不落底层，日志仅说明协议侧尝试过 |
| 多次重配导致不稳定 | 检查是否绕过 `ApplyVideoEncodeStreamConfig()`，直接逐字段调用 `rk_video_set_*` |

## 存储排查

| 问题 | 优先确认 |
|------|----------|
| 录像没有生成 | `CRecordManager::Start()` 是否执行，`RecordStreamStart(H264|H265|AUDIO)` 是否订阅 |
| 录像 codec 错 | `onRecord_fn()` 中 `media_type -> iCodeType` 是否正确，MP4 muxer 是否按 `video_codec_type` 选择 codec |
| 回放选错文件 | `StorageManager` 搜索时间段、index 排序、文件路径构造 |
| H264/H265 混播失败 | demuxer `iCodeType`、ProtocolManager codec probe、RTP/PS sender codec 三处必须一致 |
| SD 卡状态异常 | `g_StorageManager->GetDiskState()`、`DiskManager`、`/mnt/sdcard/DCIM` 是否可写 |

## 抓包判断要点

- `INVITE` 中 `m=video`、`a=rtpmap`、`a=setup`、`a=connection`、`y=`、`f=` 要和设备 200 OK 对照。
- 平台请求 H264/H265 不代表录像文件就是该 codec；回放/下载必须看本地 MP4。
- 若 SIP 200 OK 正常但平台黑屏，继续看 RTP payload 是否持续、PS stream id 是否匹配 H264/H265、关键帧是否发出。
- 若设备直接 403，优先查协议 handler 的返回码来源，避免底层非关键配置 ret 被透传成拒流。

## 文档回写规则

- 发现可复用根因时，写入 `helloagents/cases/YYYYMMDDHHMM_topic.md`。
- 发现模块长期行为或边界时，写入 `helloagents/wiki/modules/*.md`。
- 发现当前文档与代码冲突时，以当前代码为准更新知识库，并在文档中标记历史结论或注意项。

## 依赖
- `App/Main.cpp`
- `App/Protocol/ProtocolManager.cpp`
- `App/Protocol/gb28181/GB28181RtpPsSender.*`
- `App/Media/AVManager.cpp`
- `App/Media/VideoEncodeControl.cpp`
- `App/Media/Record.cpp`
- `App/Storage/common/StorageManager.cpp`
- `Middleware/Include/PAL/Capture.h`
- `Middleware/Include/PAL/libdmc.h`

## 变更历史
- 2026-05-16: 新增 RK IPC 排查手册，沉淀实时流、编码配置、录像/回放、抓包和日志定位流程。
