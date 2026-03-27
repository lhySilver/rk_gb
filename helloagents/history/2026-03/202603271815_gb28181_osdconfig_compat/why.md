# 变更提案: GB28181 OSDConfig 设备侧兼容

## 需求背景
当前设备已经支持基于 `SVACEncodeConfig/SVACDecodeConfig` 的 OSD 三个位查询与设置，但联调平台实际发送的是白皮书定义的 `OSDConfig` 报文：
- 查询使用 `ConfigDownload + ConfigType=OSDConfig`
- 设置使用 `DeviceConfig + OSDConfig`

现网抓包和日志表明：
- SIP `MESSAGE` 已正常送达，设备也返回了 SIP `200 OK`
- 查询侧返回 `ConfigDownload` 但 `num=0`
- 设置侧进入 `HandleGbConfigControl` 时 `config_set_param` 为空并返回 `-82`

根因是设备 XML 编解码层和 App 配置层没有实现 `OSDConfig` 模型。

## 变更内容
1. 在共享 GB28181 配置模型中增加 `OSDConfig` 的查询/设置结构。
2. 在 XML 解析与打包链路中补齐 `OSDConfig` 的查询识别、应答编码和设置解析。
3. 在 `ProtocolManager` 中将 `OSDConfig` 映射到现有 `gb_osd` 与 `rk_osd_*` 能力。
4. 明确保留当前设备边界：最多 1 行自定义文字、`TimeType=1` 仅协议侧保存并尽力映射，底层显示格式仍受 RK OSD 能力限制。

## 影响范围
- **模块:** `ProtocolManager`、GB28181 SDK XML 编解码、共享配置结构
- **文件:** `App/Protocol/ProtocolManager.cpp`、`third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h`、`third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp`
- **协议:** `ConfigDownload` / `DeviceConfig` 中的 `OSDConfig`

## 核心场景

### 需求: 平台查询前端 OSD
**模块:** `GB28181XmlParser` / `ProtocolManager`
平台发送 `ConfigDownload` 查询 `OSDConfig`。

#### 场景: 设备返回白皮书 OSDConfig
- 平台下发 `<ConfigType>OSDConfig</ConfigType>`
- 设备识别这是前端 OSD 查询
- 设备返回 `<OSDConfig>`，包含 `Length/Width/TimeX/TimeY/TimeEnable/TimeType/TextEnable/SumNum`

### 需求: 平台配置前端 OSD
**模块:** `GB28181XmlParser` / `ProtocolManager`
平台通过 `DeviceConfig` 下发 `OSDConfig`。

#### 场景: 设备应用 OSDConfig
- 平台下发 `<OSDConfig>`
- 设备解析 `TimeX/TimeY/TimeEnable/TimeType/TextEnable/Item`
- 设备更新现有 `gb_osd` 配置并调用 `rk_osd_*` 生效

## 风险评估
- **风险:** `GB28181Defs.h` 属于共享结构，新增 `OSDConfig` 模型会影响 XML 层和 App 层的枚举判断。
- **缓解:** 仅扩展新类型，不改已有 `BasicParam/VideoParamOpt/SVAC*` 语义；所有旧分支保持原行为。
- **风险:** RK OSD 底层不原生支持白皮书 `TimeType=1` 的中文日期格式。
- **缓解:** 协议层保留 `TimeType` 值并接入现有配置模型，知识库明确底层显示缺口。
