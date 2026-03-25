# 任务清单: issue38 国标配置文件管理优化

- [√] 1. 在 `LocalConfigProvider` 中补充 `GbRegisterParam` 级别的默认值、读取和更新静态入口，满足 init 前可直接访问本地文件
- [√] 2. 重写 `ProtocolManager::GetGbRegisterConfig()` / `SetGbRegisterConfig()`，改为走文件配置并在运行中即时刷新 GB 注册生命周期
- [√] 3. 更新 `helloagents/wiki/modules/protocol.md`、`helloagents/CHANGELOG.md`，记录 issue38 的接口语义与行为变化
- [√] 4. 做最小验证并归档方案包到 `helloagents/history/2026-03/`
