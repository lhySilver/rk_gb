# 技术设计

## 方案
1. 调整 `GB28181XmlParser.h/.cpp` 中 `UnPackBroadcastNotify()` 的签名，改为按引用返回 `sn` 与 `code`。
2. 在广播通知解包时回填 `notify.SN`，并使用 `notify.TargetID` 作为 `code`，保证响应 XML 中的 `SN` 与 `DeviceID` 正确。
3. 在 `ProtocolManager::HandleGbBroadcastNotify()` 增加 `resolvedGbCode` 兜底逻辑，优先使用上层 `gbCode`，为空时退回 `TargetID` 或本机设备 ID。

## 影响范围
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.h`
- `third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp`
- `App/Protocol/ProtocolManager.cpp`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
- `helloagents/wiki/modules/protocol.md`

## 风险与规避
- 风险: 广播 `DeviceID` 选错为 `SourceID` 可能导致平台继续不认可应答。
- 规避: 改为使用 `TargetID`，与广播请求目标设备和本机设备 ID 语义保持一致。
- 风险: 修改 XML 解析接口可能影响其它通知类型。
- 规避: 仅改 `kBroadcastNotify` 分支调用点，其他通知类型签名和行为保持不变。
