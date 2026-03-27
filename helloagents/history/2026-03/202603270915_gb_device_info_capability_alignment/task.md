# 任务清单: GB28181 DeviceInfo 扩展字段与能力清单补齐

目录: `helloagents/history/2026-03/202603270915_gb_device_info_capability_alignment/`

---

## 1. 方案准备
- [√] 1.1 创建方案包并明确本轮只补 `A.19` 扩展身份字段与两组能力节点

## 2. 公共结构与 XML 模型
- [√] 2.1 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 中扩展 `DeviceInfo`，补齐扩展身份字段和最小能力子结构
- [√] 2.2 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/device_info.h`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/device_info.cpp` 中补齐 `DeviceCapabilityList/ProtocolFunctionList` 嵌套编解码
- [√] 2.3 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/templete/device_info.json` 中同步模板，避免后续再生成覆盖

## 3. 业务接线
- [√] 3.1 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp` 中补齐 `DeviceInfo` 扩展字段与能力节点的编解码映射
- [√] 3.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中基于当前运行事实组装扩展身份字段与最小真实能力值

## 4. 验证
- [√] 4.1 进行最小编译验证，确认 `DeviceInfo` 相关改动可以通过构建
- [√] 4.2 记录本轮能力回包中的已知缺陷表达，避免知识库与代码脱节

## 5. 文档更新
- [√] 5.1 更新 `rk_gb/helloagents/wiki/modules/zero_config.md`
- [√] 5.2 更新 `rk_gb/helloagents/wiki/modules/gb28181.md`
- [√] 5.3 更新 `rk_gb/helloagents/wiki/modules/terminal_requirements.md`
- [√] 5.4 更新 `rk_gb/helloagents/CHANGELOG.md`
- [√] 5.5 更新 `rk_gb/helloagents/history/index.md`

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
