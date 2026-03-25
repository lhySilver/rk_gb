# API 手册

## 概述
本项目对外不是传统 Web 服务工程，但存在四类“接口面”：
- 本地协议配置拉取接口
- GB28181 配置查询 / 设置接口与 SDK 回调
- GAT1400 作为下级系统发起 / 接收的 HTTP VIID 接口
- GB28181 通过 SDK 回调暴露的媒体与控制接口

## 认证方式
- 本地配置接口: 当前代码中未体现额外认证
- GAT1400 对上 HTTP: 支持 Digest / Basic 类认证流程，401 后通过 `http_auth` 重试
- GB28181: 由 SIP 注册参数 `username/password` 驱动

---

## 接口列表

### 配置服务

#### `GET /openapi/v1/ipc/protocol/config`
**描述:** `HttpConfigProvider` 拉取协议最新配置。

**请求参数:** 无

**响应:** 最小 JSON 配置，映射到 `ProtocolExternalConfig`

#### `POST /openapi/v1/ipc/protocol/config`
**描述:** 协议配置应用后回推当前有效配置。

#### `POST /openapi/v1/ipc/protocol/config/validate`
**描述:** 校验候选配置是否合法。

#### `GET /openapi/v1/ipc/protocol/capabilities`
**描述:** 查询配置能力集合。

### GB28181 本地注册配置接口

#### `GetGbRegisterConfig()`
**描述:** 读取 GB28181 注册配置；若 `ProtocolManager::Init()` 之前调用，会先按默认值补齐 `/userdata/conf/Config/gb28181.ini` 后返回。

#### `SetGbRegisterConfig()`
**描述:** 写入 GB28181 注册配置并按差异重载注册生命周期。

**当前约束:**
- 当前仅持久化 `enabled`、`username`、`server_ip`、`server_port`、`device_id`、`password` 6 个注册字段
- `device_name`、`expires_sec`、`gb_talk`、`gb_broadcast`、`gb_upgrade`、`gat_*` 等其他协议项不再写入本地 `ini`
- 运行中调用后会先落盘，再按差异重载 GB 注册状态，而不是只修改内存副本

### ProtocolManager 核心回调

#### `HandleGbLiveStreamRequest`
**描述:** 处理 GB28181 实时流请求，解析 SDP、准备媒体参数并返回应答媒体信息。

#### `HandleGbStreamAck`
**描述:** 处理平台对实时流 / 回放 / 下载应答的 `ACK`，驱动后续媒体启动。

#### `RespondGbMediaPlayInfo`
**描述:** 生成并发送实时流 / 回放 / 下载的 SIP `200 OK` SDP。

#### `HandleGbPlaybackRequest` / `HandleGbDownloadRequest`
**描述:** 启动存储回放通道，并在 `ACK` 或控制恢复后继续发流。

#### `HandleGbPlayControl`
**描述:** 处理 `PLAY / PAUSE / TEARDOWN / Scale / Range` 等回放控制。

#### `HandleGbAudioStreamRequest`
**描述:** 处理被动 `Talk INVITE`，使用独立 talk session，并在 `ACK` 后再应用媒体协商参数。

### GAT1400 对上接口

**规范基线:** `GA/T 1400.4-2017`

#### 通用约束
- 资源风格: 所有接口均按 REST 资源建模，使用 `/VIID/*` 或 `/VIAS/*` 层级 URI
- Content-Type: `application/*+JSON`
- 身份头: 请求头应带 `User-Identify`
- 认证: 应支持 `RFC 2617` 摘要认证，宜支持 `RFC 2818` HTTPS
- 响应约束: `GET` 成功时返回资源对象，非 `2XX` 场景返回 `ResponseStatus`

#### 系统类接口

| 方法 | URI | 说明 |
|------|-----|------|
| `POST` | `/VIID/System/Register` | 设备注册 |
| `POST` | `/VIID/System/UnRegister` | 设备注销 |
| `POST` | `/VIID/System/Keepalive` | 心跳保活 |
| `GET` | `/VIID/System/Time` | 校时；当前工程中已禁用为 no-op，调用只返回空时间字符串 |

#### 标准资源上报接口

| 方法 | URI | 说明 |
|------|-----|------|
| `POST` | `/VIID/Faces` | 批量上报人脸对象 |
| `POST` | `/VIID/Persons` | 批量上报人员对象 |
| `POST` | `/VIID/MotorVehicles` | 批量上报机动车对象 |
| `POST` | `/VIID/NonMotorVehicles` | 批量上报非机动车对象 |
| `POST` | `/VIID/Things` | 批量上报物品对象 |
| `POST` | `/VIID/Scenes` | 批量上报场景对象 |
| `POST` | `/VIID/AnalysisRules` | 批量上报视频图像分析规则 |
| `POST` | `/VIID/VideoLabels` | 批量上报视频图像标签 |
| `POST` | `/VIID/Dispositions` | 批量布控 |
| `POST` | `/VIID/DispositionNotifications` | 批量告警通知 |

#### 元数据 + Data 双阶段接口

| 元数据接口 | Data 接口 | 说明 |
|------------|-----------|------|
| `POST /VIID/Images` | `POST /VIID/Images/{ImageID}/Data` | 先上报图像元数据，再上传 Base64 图像数据 |
| `POST /VIID/VideoSlices` | `POST /VIID/VideoSlices/{VideoID}/Data` | 先上报视频片段元数据，再上传 Base64 视频数据 |
| `POST /VIID/Files` | `POST /VIID/Files/{FileID}/Data` | 先上报文件元数据，再上传 Base64 文件数据 |

#### 订阅与通知接口

| 方法 | URI | 说明 |
|------|-----|------|
| `GET/POST/PUT/DELETE` | `/VIID/Subscribes` | 查询 / 新增 / 修改 / 删除订阅任务 |
| `PUT` | `/VIID/Subscribes/{SubscribeID}` | 取消订阅，写入取消单位、取消人、取消时间、取消原因 |
| `POST` | `/VIID/SubscribeNotifications` | 发送订阅通知 |
| `DELETE` | `/VIID/SubscribeNotifications?IDList=a,b,c` | 删除通知记录 |

#### 其他标准资源

| 方法 | URI | 说明 |
|------|-----|------|
| `GET/PUT` | `/VIID/APEs` | 查询采集设备 / 动态口令更新 |
| `GET` | `/VIID/APSs` | 查询采集系统 |
| `GET` | `/VIID/Tollgates` | 查询视频卡口 |
| `GET` | `/VIID/Lanes` | 查询车道 |
| `GET/POST/PUT/DELETE` | `/VIID/AnalysisRules` | 分析规则查询 / 新增 / 更新 / 删除 |
| `GET/POST/PUT/DELETE` | `/VIID/VideoLabels` | 视频图像标签查询 / 新增 / 更新 / 删除 |

#### 当前工程实现备注
- 当前仓库以“设备侧 lower client”为主，重点覆盖注册、保活、结构化对象上报、订阅任务接收和通知发送。
- 当前本地 HTTP 服务面主要处理 `/VIID/Subscribes` 及 `/VIID/Subscribes/{ID}`。
- 当前对上请求目标由 `gat_register.scheme/server_ip/server_port/base_path/request_timeout_ms` 统一拼装；`overrideUrl` 仍可覆盖到完整 URL。
- 上传失败请求会按 `gat_upload.retry_policy` 重试，最终失败后落盘到 `gat_upload.queue_dir`，并在重新注册成功或保活恢复后回放。
- 标准还定义了 `/VIAS/*` 分析系统接口，当前仓库未见完整分析系统服务面；当前 `ExecuteRequest` 会对 `/VIAS/*` 直接返回本地不支持错误，且不会进入失败补传队列。
- 当前代码中额外存在 `POST /VIID/APEs` 的发送入口，这不是 `GA/T 1400.4-2017` 对 `APEs` 的标准定义；现已收口为兼容扩展开关，只有 `gat_upload_enable_apes_post_compat=1` 时才允许发送。
- 配置模型已接受 `https`，但 `ExecuteRequest` 仍只实现 `HTTP/TCP`，若平台要求 HTTPS 仍需继续开发。

### GAT1400 本地下级订阅接口

#### `GET /VIID/Subscribes`
**描述:** 查询本地维护的订阅列表。

#### `POST /VIID/Subscribes`
**描述:** 新增订阅。

#### `PUT /VIID/Subscribes`
**描述:** 更新订阅。

#### `DELETE /VIID/Subscribes?IDList=a,b,c`
**描述:** 批量删除订阅。

#### `PUT /VIID/Subscribes/{SubscribeID}`
**描述:** 取消单个订阅。

### GAT1400 SDK 导出接口

**描述:** 通过 `LowerGAT1400SDK` 将运行中的 `GAT1400ClientService` 暴露给业务模块。

#### 生命周期与状态

| 导出函数 | 说明 |
|----------|------|
| `LOWER_1400_START` | 基于 `REGIST_PARAM_S + CONNECT_PARAM_S + listenPort` 启动或重启当前进程内的 1400 client |
| `LOWER_1400_STOP` | 停止当前进程内 1400 client |
| `LOWER_1400_REGISTER` / `LOWER_1400_UNREGISTER` / `LOWER_1400_KEEPALIVE` | 主动触发注册、注销、保活 |
| `LOWER_1400_GET_TIME` | 读取平台校时结果；当前实现中返回空时间字符串，不再真正访问 `/VIID/System/Time` |

#### 上报与观察者

| 导出函数 | 说明 |
|----------|------|
| `LOWER_1400_POST_FACES/.../LOWER_1400_POST_APES` | 直接进入 `GAT1400ClientService::Post*` 主链路，复用当前重试和补传能力；其中 `LOWER_1400_POST_APES` 受 `gat_upload_enable_apes_post_compat` 开关控制 |
| `LOWER_1400_POST_SUBSCRIBENOTIFICATIONS` | 回发订阅通知 |
| `LOWER_1400_ADD_SUBSCRIBE_OBSERVER` / `LOWER_1400_DEL_SUBSCRIBE_OBSERVER` | 业务层消费 `/VIID/Subscribes` 变更 |
| `LOWER_1400_ADD_REGIST_OBSERVER` / `LOWER_1400_DEL_REGIST_OBSERVER` | 业务层监听注册状态变化 |
| `LOWER_1400_SET_1400INHTTPCMD(url, method, format, content)` | 按 `HTTP_MSG_TYPE_H + HTTP_URI_TYPE_H` 透传自定义 1400 HTTP 命令，内部仍复用现有发送链路；若目标为 `/VIAS/*` 或未开启的 `POST /VIID/APEs` 兼容扩展，会直接返回本地错误 |

### GB28181 设备能力接口

#### 实时流请求
**描述:** 由 `GB28181ClientReceiverAdapter::OnStreamRequest` 转发到 `ProtocolManager::HandleGbLiveStreamRequest`。

#### 历史回放 / 下载请求
**描述:** 由 `HandleGbPlaybackRequest` / `HandleGbDownloadRequest` 启动存储回放通道。

#### 历史回放控制
**描述:** 由 `HandleGbPlayControl` 处理 `PLAY / PAUSE / TEARDOWN / Scale / Range`，当前已支持暂停、恢复、倍速、拖动定位和停止。

#### 语音广播 / 对讲 / 监听
**描述:** 广播音频通过 `GB28181BroadcastBridge` 接收；被动 `Talk INVITE` 通过独立 talk session 处理；监听音频通过 `GB28181ListenBridge` 回传。

#### 广播链路约束
- 当前广播流程按 `GB/T 28181-2022` 第 9.12 节调整为“`Notify` 先回纯 SIP `200 OK` -> 独立发送 `Broadcast` 应答 `MESSAGE` -> 设备主动发起音频 `INVITE` 建链”。
- `Broadcast Notify` 的业务 XML 应答必须回填平台原始 `SN`，并使用 `TargetID` 作为响应 `DeviceID`；错误回填通常会导致平台继续判定广播失败。
- 广播主动 `INVITE` 收到 `200 OK` 后，要确保 SDP 里的 `payload/rtpmap` 已成功回填，再进入后续音频桥配置。

#### 设备控制
**描述:** 支持 PTZ、布防 / 撤防、告警复位、参数配置、设备升级、回到预置位、远程重启。

#### 终端白皮书扩展接口约束
- 注册重定向: 白皮书 `A.16` 要求设备先向重定向服务器发起 `REGISTER`，收到 `302 Moved` 后再向正式平台注册；请求头 / 设备信息中需要带 `StringCode`、`Mac`、`CustomProtocolVersion` 等扩展信息。
- 语音广播媒体协商: 白皮书 `A.1` 和 `A.14/A.15` 要求支持 `TCP 主动拉流`；当前实现通过 `StartBroadcastStreamRequest`、`BuildGbBroadcastMediaInfo`、`ApplyGbBroadcastTransportHint`、`BuildGbBroadcastAnswerSdp` 协商 `UDP / TCP active / TCP passive`。
- 基础参数查询 / 配置: 当前 `DeviceConfig` / `ConfigDownload` 主链已支持 `DeviceName`、`Expiration`、`HeartBeatInterval`、`HeartBeatCount`、`ImageFlip`、OSD 开关；白皮书扩展的 `LocalPort`、`Domain`、`SipIp`、`SipPort`、`SipId`、`VoiceFlowMode`、`Password`、`ChannelList` 尚未在当前 App 层结构中完整暴露。
- 设备信息查询应答: 当前 `ResponseDeviceInfo` 回的是标准基础字段；白皮书 `A.19` 要求的 `StringCode`、`Mac`、`CustomProtocolVersion`、`DeviceCapabilityList` 需作为联调确认项。

---

## 错误码

| 错误码 | 说明 |
|--------|------|
| `<0` | 本工程内大部分协议模块使用负数表示本地错误 |
| `0` | 成功 |
| `GAT1400 STATUS_CODE_H` | GAT1400 JSON 响应状态码，定义见 `CMS1400Struct.h` |
