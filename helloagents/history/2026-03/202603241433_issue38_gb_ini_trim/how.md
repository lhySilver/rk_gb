# 技术设计: issue38 最新评论跟进

## 实现策略
1. `InitDefaultLocalConfig()` 继续装配完整 `ProtocolExternalConfig` 默认值，保证对讲、广播、升级、GAT 等业务运行不受影响。
2. `LoadLocalConfigFile()` 只从 `gb28181.ini` 读取 6 个注册字段；文件中即使存在历史遗留扩展项，也不再导入运行态。
3. `SaveLocalConfigFile()` 只回写 6 个注册字段，确保新生成和后续更新的文件内容符合 issue38 最新评论要求。
4. 同步修正知识库，将“其余字段仍写入本地 ini”的旧描述改成“其余字段固定在代码中”。

## 风险控制
- 不修改 `ProtocolManager::GetGbRegisterConfig()` / `SetGbRegisterConfig()` 的行为边界，只收缩持久化字段范围。
- 通过保留 `InitDefaultLocalConfig()` 的完整默认装配，避免广播、对讲、GAT、升级等功能因本地文件字段删除而拿到空值。
