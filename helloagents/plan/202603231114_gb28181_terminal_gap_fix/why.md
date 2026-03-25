# 变更提案: GB28181终端白皮书差距收敛

## 需求背景
当前仓库已经完成 `GB/T 28181-2016` 基础链路和中国移动终端白皮书的需求对齐文档，但白皮书中的几个关键验收点仍停留在“已识别缺口、未进入实施”的状态。

现阶段最影响联调推进的是四类问题：
- 公网广播场景需要优先走 `TCP active`，并且 `VoiceFlowMode` 的查询/配置必须能回显到平台。
- 注册重定向要求首个 `REGISTER` 携带扩展身份字段，并在正式平台注册前后切换 `StringCode/deviceId` 身份。
- 设备信息查询应答缺少白皮书 `A.19` 和附录 `G` 要求的扩展字段及能力清单。
- 基础参数、告警扩展、多码流能力与终端白皮书仍有结构层面的缺口。

## 变更内容
1. 先收敛 P0 闭环：广播 `TCP active`、注册重定向扩展、设备信息查询扩展、基础参数扩展。
2. 再推进 P1 能力：告警扩展建模、真实事件接线、报警复位例外语义、多码流能力补齐。
3. 固化抓包和日志核对点，避免后续联调只看到“有回包”而忽略字段级不符合项。

## 影响范围
- **模块:** `GB28181BroadcastBridge`、`ProtocolManager`、配置读取模块、GB28181 SDK Client/XML 编解码链路
- **文件:** `rk_gb/App/Protocol/gb28181/GB28181BroadcastBridge.*`、`rk_gb/App/Protocol/ProtocolManager.*`、`rk_gb/App/Protocol/config/*`、`rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/**/*`
- **API:** SIP `REGISTER` / `INVITE` / `MESSAGE`，MANSCDP `DeviceInfo` / `ConfigDownload` / `Alarm`
- **数据:** `DeviceInfo`、`CfgBasicParam/BasicSetting`、`AlarmNotifyInfo`、广播媒体协商参数

## 核心场景

### 需求: 公网语音广播闭环
**模块:** `GB28181BroadcastBridge` / `ProtocolManager`
平台位于公网时，广播优先按 `TCP active` 协商，设备依据平台应答中的媒体 `IP/PORT` 主动发流。

#### 场景: 平台公网下发广播
平台发起广播 `INVITE`，设备回 `200 OK` 后，平台回 `ACK`。
- 设备将 `VoiceFlowMode=1` 映射到 `TCP active`
- 设备从平台回复的 SDP 中提取媒体 `IP/PORT`
- `ACK` 校验通过后启动向平台的主动推流

### 需求: 注册重定向扩展闭环
**模块:** SDK 注册链路 / 配置模块
终端先向重定向服务器注册，再依据 `302 Moved` 跳转到正式平台，首包身份和扩展头必须符合白皮书 `A.16`。

#### 场景: 首次重定向注册
终端尚未拿到正式 SIPID。
- 首次 `REGISTER` 的 `From/To` 使用 `StringCode`
- 首次 `REGISTER` 携带 `Mac/StringCode/Line/Manufacturer/Model/Name/CustomProtocolVersion`
- 重定向成功后切换为正式 `deviceId` 完成平台注册

### 需求: 设备信息查询扩展
**模块:** `DeviceInfo` XML 编码链路
平台查询设备信息时，终端除基础字段外，还需返回白皮书要求的扩展身份和能力字段。

#### 场景: 平台查询 DeviceInfo
平台发送 `DeviceInfo` 查询。
- 终端返回基础字段和 `StringCode/Mac/Line/CustomProtocolVersion`
- 终端返回最小能力清单，至少覆盖广播、图像翻转、多码流、OSD、升级、告警、广播 `TCP active`

### 需求: 基础参数查询与配置扩展
**模块:** `ConfigDownload` / 配置提供器
平台查询或配置基础参数时，需要看到广播流模式和接入参数，而不仅是当前的心跳和设备名。

#### 场景: 平台查询或下发 BasicParam
平台执行 `DeviceConfig` 或 `ConfigDownload`。
- 终端能回 `VoiceFlowMode/LocalPort/Domain/SipIp/SipPort/SipId/Password/ChannelList`
- `VoiceFlowMode` 与广播协商实际行为保持一致

## 风险评估
- **风险:** `GB28181Defs.h` 与 XML 编码链路属于 SDK 公共结构，字段扩展可能影响现有序列化兼容性。
- **缓解:** 按 P0/P1 分阶段实施，先补必需字段和最小能力清单；每一步都配套抓包与回归验证。
- **风险:** 白皮书内部仍存在告警编码和 `VoiceFlowMode` 描述不一致。
- **缓解:** 代码实现前以平台联调口径为准，并在日志和配置中保留明确映射关系。
