# GB28181 远程抓拍与目录订阅审查记录

## 1. 文档用途
- 本文档用于沉淀 `rk_gb` 当前 GB28181 远程抓拍、目录订阅、报警订阅和移动位置订阅的代码现状、缺口判断和后续待办。
- 审查基线以 [`gb28181-2022-baseline.md`](gb28181-2022-baseline.md) 和 [`protocol.md`](protocol.md) 为准。
- 本文档只基于当前分支代码事实，不代表平台联调已经覆盖这些能力。

## 2. 相关代码入口
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.h`
  - `UnPackKeyFrameControl()`
  - `UnPackCatalogSubcribe()`
  - `UnPackAlarmSubcribe()`
  - `UnPackMobilePositionSubcribe()`
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp`
  - `OnDeviceControl()`
  - `OnCatalogSub()`
  - `OnAlarmSub()`
  - `OnMobilePositionSub()`
  - `SubscribeExpireDelThread()`
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/SubscribeManager.cpp`
  - `InsertSubscribeHandle()`
  - `DeleteSubscribeHandle()`
  - `CheckSubscribeStatusTimer()`
- `App/Protocol/gb28181/GB28181ClientReceiverAdapter.cpp`
  - `OnSubscribe()`
  - `OnDeviceControl()`
- `App/Protocol/ProtocolManager.cpp`
  - `HandleGbDeviceControl()`
  - `HandleGbSubscribe()`
  - `NotifyGbCatalogInternal()`
  - `ResponseGbQueryCatalog()`
  - `NotifyGbAlarm()`
  - `NotifyGbMobilePosition()`

## 3. 审查结论

### 3.1 远程抓拍目前未形成完整业务能力
- SDK 侧已具备 `KeyFrameControl` 的 XML 解析入口，可识别 `IFameControl` 类控制。
- 但业务层 `ProtocolManager::HandleGbDeviceControl()` 对 `kIFameControl` 只是直接返回成功，没有触发任何实际动作。
- 当前代码里唯一明确调用 `CaptureForceIFrame()` 的位置，是实时流会话在 `ACK` 后主动拉首个 IDR，用于实时流建链，不是远程抓拍控制链路。
- 结论:
  - 若平台诉求只是“请求关键帧”，当前实现属于“协议识别已接入，业务执行缺失”。
  - 若平台诉求是 GB/T 28181-2022 第 9.14 节的“图像抓拍”，当前工程还没有看到完整的抓图、文件生成、结果回传或图片上传链路。

### 3.2 目录订阅基础框架已接通，但只做到“保存句柄 + 首次静态快照”
- SDK 收到 `Catalog` `SUBSCRIBE` 后，会回 `200 OK`，保存订阅对话和事件头，并通过 `OnSubscribe()` 回调上层。
- 业务层 `HandleGbSubscribe()` 会保存目录订阅句柄，并在首次绑定时立即调用 `NotifyGbCatalog()`。
- 当前 `NotifyGbCatalogInternal()` 发送的是固定单条目录项：
  - `sum=1`
  - `Event=ON`
  - `Name=DeviceID`
  - `Manufacturer=IPC`
  - `Model=RC0240`
- `ResponseGbQueryCatalog()` 的查询应答同样是静态单条目录项，说明“目录查询”和“目录订阅通知”当前共用同一份最小静态数据。

### 3.3 订阅参数目前基本未落业务
- SDK 在 `OnCatalogSub()`、`OnAlarmSub()`、`OnMobilePositionSub()` 中都会先解析 XML 到结构体。
- 但传给业务层时，实际上传的是 `data->messgae.content` 指针，业务层 `HandleGbSubscribe()` 又直接把 `info` 丢掉了。
- 这意味着以下订阅参数当前大概率都没有真正生效：
  - 目录订阅中的过滤条件
  - 报警订阅中的优先级/告警方式/时间窗
  - 移动位置订阅中的 `Interval`

### 3.4 订阅超时处理存在明显缺口
- `SubscribeExpireDelThread()` 只显式区分了目录订阅和报警订阅两类超时。
- 移动位置订阅超时时，没有单独分支；按当前代码路径，会落到默认值，最终误报为 `kCatalogSubscribe` 系列事件。
- `SubscribeManager` 的过期回调是进程级全局函数指针 `g_pExpireMessage`，设计上偏脆弱；如果后面出现多实例或不同订阅管理器并存，存在相互覆盖风险。

### 3.5 报警和移动位置“可被推送”，但没有形成“按订阅驱动”的完整实现
- `ProtocolManager` 已经保留了 `m_gb_alarm_subscribe_handle` 和 `m_gb_mobile_position_subscribe_handle`，并提供：
  - `NotifyGbAlarm()`
  - `NotifyGbMobilePosition()`
- 这说明“上层主动触发一次通知”的能力是有的。
- 但当前没有看到“按订阅参数自动调度”的链路，尤其是移动位置 `Interval` 没有被保存和调度。

## 4. 风险判断

### 4.1 抓拍风险
- 平台若只测试“下发关键帧命令后设备是否真的触发关键帧”，当前代码会表现为“协议接受但无实际动作”。
- 平台若测试的是“JPEG 抓拍/图像抓拍结果”，当前代码大概率无法通过。

### 4.2 目录订阅风险
- 平台若只验证“是否支持 `SUBSCRIBE` 并能回一条目录 `NOTIFY`”，当前实现可能勉强通过。
- 平台若验证“目录树完整性、状态变化通知、目录增删改、事件字段准确性”，当前实现大概率不完整。

### 4.3 报警/移动位置订阅风险
- 平台若验证订阅过滤条件、移动位置订阅周期、订阅超时语义，当前实现存在明显缺口。
- 移动位置订阅超时事件目前还有类型误报风险，后续定位时容易把问题看错到目录订阅上。

## 5. Checklist

### 5.1 远程抓拍排查
- 平台下发的是“关键帧控制”还是“图像抓拍”。
- 设备是否只回了 SIP/业务成功，而没有真实执行抓拍动作。
- 下发 `IFameControl` 后，设备是否真的调用了 `CaptureForceIFrame()` 或等价能力。
- 若要做图像抓拍，是否存在图片生成、存储路径、上送方式和结果应答。

### 5.2 目录订阅排查
- `SUBSCRIBE` 是否成功建立，设备是否返回了纯 SIP `200 OK`。
- 建立订阅后，设备是否发送了首条 `Catalog NOTIFY`。
- `NOTIFY` 里的目录数量、`Event`、`DeviceID`、状态字段是否符合平台预期。
- 目录变化时，设备是否会再次发送增量 `NOTIFY`，而不是永远只发首次静态快照。

### 5.3 报警/移动位置订阅排查
- `Alarm` 和 `MobilePosition` 的 `SUBSCRIBE` 是否分别建立成功。
- 业务层是否保存了订阅参数，而不是只记住一个句柄。
- 移动位置订阅的 `Interval` 是否真的转化成发送周期。
- 订阅过期后，设备上报给业务层的超时类型是否正确。

## 6. TODO

### P0
- 明确平台对“远程抓拍”的真实诉求：
  - 只是触发关键帧
  - 还是 GB/T 28181-2022 第 9.14 节图像抓拍
- 修复 `kIFameControl` 业务空实现：
  - 至少补真实执行动作
  - 至少补日志，避免“看起来成功，实际上什么都没做”
- 修复移动位置订阅超时类型误报问题，确保不会被当成目录订阅超时。

### P1
- 为 `Catalog/Alarm/MobilePosition` 订阅补齐参数落地：
  - 过滤条件
  - 时间窗
  - 告警方式
  - 移动位置 `Interval`
- 让目录订阅从“首次静态快照”升级到“可持续通知”的实现，至少支持目录状态变化时再次发送 `NOTIFY`。

### P2
- 评估并补齐 GB28181 图像抓拍的完整链路：
  - 图片采集
  - 落盘路径
  - 结果应答
  - 如标准或平台要求，补图片上送
- 梳理目录项模型，评估是否需要从“单条本机目录”升级为“多通道/多目录树”表达。

### P3
- 补细日志，覆盖以下关键点：
  - 订阅建立
  - 订阅续订
  - 订阅超时
  - 首次目录快照发送
  - 增量目录通知发送
  - 远程抓拍控制命中与执行结果

## 7. 结论
- 当前工程在“目录订阅基本接入”和“报警/移动位置可手动通知”两点上已经有基础框架。
- 但远程抓拍仍未形成真正业务实现；目录/报警/移动位置订阅也还停留在“能接消息、能记句柄”的阶段，距离按标准长期稳定联调还有明显差距。
