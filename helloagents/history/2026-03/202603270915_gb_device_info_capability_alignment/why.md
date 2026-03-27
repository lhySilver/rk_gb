# 变更提案: GB28181 DeviceInfo 扩展字段与能力清单补齐

## 需求背景
上一轮零配置整改已经补齐了 `StringCode/Mac/Line/CustomProtocolVersion` 在注册链路中的承载和 `302` 重定向闭环，但平台查询 `DeviceInfo` 时，设备仍只返回基础字段。

当前还缺两类直接影响联调验收的内容：
- 白皮书 `A.19` 要求的 `StringCode/Mac/Line/CustomProtocolVersion`
- `DeviceCapabilityList/ProtocolFunctionList` 两组能力节点

知识库已经把这两项列为剩余重点缺口，用户本轮要求把它们补上。

## 变更内容
1. 扩展 `DeviceInfo` 公共结构和 `device_info` XML 模型，补齐 `A.19` 扩展身份字段。
2. 按白皮书真实 XML 结构补 `DeviceCapabilityList` 与 `ProtocolFunctionList` 子节点。
3. 在 `ProtocolManager::ResponseGbQueryDeviceInfo()` 里基于当前运行事实回最小真实能力集，不把当前不具备的能力伪装成支持。

## 影响范围
- **模块:** `ProtocolManager`、GB28181 XML 编解码
- **文件:** `rk_gb/App/Protocol/ProtocolManager.cpp`、`rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/device_info.*`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp`
- **协议:** MANSCDP `DeviceInfo` 查询应答

## 核心场景

### 需求: 平台查询设备信息时返回白皮书扩展字段
**模块:** `ProtocolManager` / `DeviceInfo` XML
平台发送 `DeviceInfo` 查询后，设备除了基础字段外，还要返回 `StringCode/Mac/Line/CustomProtocolVersion`。

#### 场景: 零配置或标准国标平台查询 `DeviceInfo`
- 设备回包中带 `StringCode/Mac/Line/CustomProtocolVersion`
- 字段值优先取当前本地配置，缺省时回退到已有 GB 基础身份

### 需求: `DeviceInfo` 应答带最小真实能力清单
**模块:** `ProtocolManager` / `DeviceInfo` XML
平台查询设备信息时，设备需要按附录 `G` 的描述格式回能力节点。

#### 场景: 平台校验能力节点
- `DeviceCapabilityList` 至少返回当前项目已明确的最小设备能力子集
- `ProtocolFunctionList` 至少返回广播、图像翻转、多码流、OSD、升级、告警、广播 `TCP active`
- 能力值必须反映当前实现现状，例如 `stream_count=2` 需要明确带缺陷

## 风险评估
- **风险:** `device_info.*` 是 slothxml 生成文件，直接改动后若模板不同步，后续再生成可能覆盖。
- **缓解:** 同步更新 `templete/device_info.json`，并将本轮字段清单沉淀到知识库。
- **风险:** 附录 `G` 能力字符串容易“写成看起来更好”的值，导致平台联调时暴露真实性问题。
- **缓解:** 本轮只回最小真实能力集，对已知缺陷直接编码到能力字符串中。
