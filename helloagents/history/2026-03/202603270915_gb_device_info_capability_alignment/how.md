# 技术设计: GB28181 DeviceInfo 扩展字段与能力清单补齐

## 技术方案
### 核心技术
- C++ 公共协议结构体扩展
- slothxml `device_info` 嵌套 XML 编解码
- `ProtocolManager` 运行态能力归一化

### 实现要点
- `DeviceCapabilityList/ProtocolFunctionList` 按白皮书 XML 结构建模，不走单字符串拼装捷径。
- `ProtocolManager` 只回当前仓库能够确认的最小能力子集，其余字段保持可选缺省。
- 能力字符串按附录 `G` 格式编码：基础状态 `0/1/2`，必要时追加 `Defect:` 和 `Type:`。

## 架构决策 ADR
### ADR-20260327-01: `DeviceInfo` 直接扩成嵌套能力节点
**上下文:** 白皮书 `A.19` 的 `DeviceCapabilityList/ProtocolFunctionList` 是复杂 XML，而当前工程没有任何现成结构。
**决策:** 在 `device_info` XML 模型中直接加入嵌套结构，并让 `GB28181XmlParser` 按节点编解码。
**理由:** 这样与白皮书一致，后续再加更多能力项时只需要扩子结构，不会推翻现有实现。
**替代方案:** 先塞成平面字符串字段 → 拒绝原因: 平台若按 XML 节点校验会直接不兼容。
**影响:** 需要同时改 `GB28181Defs.h`、`device_info.*` 和 `GB28181XmlParser.cpp`。

### ADR-20260327-02: 能力回包优先真实缺陷，不为“看起来通过”硬填支持
**上下文:** 当前仓库已明确存在两码流、图像翻转能力有限、升级无版本回滚等事实。
**决策:** 对这些项直接回 `2/Defect:...`，而不是统一写成 `1`。
**理由:** 平台联调里最怕“结构有了但语义是假的”，真实能力字符串更利于后续验收与整改排序。
**替代方案:** 先全部写支持，后续再修正 → 拒绝原因: 容易制造新的联调误判。
**影响:** 文档中要明确本轮仍保留的已知缺陷。

## 数据模型
```text
DeviceInfo:
  + string_code
  + mac_address
  + line_id
  + custom_protocol_version
  + device_capability_list
    + PositionCapability
    + AlarmOutCapability
    + MICCapability
    + SpeakerCapability
    + ImagingCapability
    + StreamPeramCapability
    + SupplyLightCapability
  + protocol_function_list
    + BroadcastCapability
    + FrameMirrorCapability
    + MultiStreamCapability
    + OSDCapability
    + DeviceUpgradeCapability
    + AlarmCapability
    + BroadcastTcpActiveCapability
```

## 安全与性能
- **安全:** 不在日志中额外打印敏感口令，只增加设备能力与扩展字段的非敏感输出。
- **性能:** 仅扩展控制面 XML，应答量有限，不影响 RTP/PS 媒体链路。

## 测试与部署
- **测试:** 先做最小编译验证，确保公共结构与 XML 编解码无语法/链接问题。
- **测试:** 结合本地 `DeviceInfo` 回包日志检查扩展字段和能力字符串是否按预期组装。
- **部署:** 不引入新开关，沿用当前分支行为，标准国标与零配置平台都能直接查询到扩展后的 `DeviceInfo`。
