# 任务清单: issue38 最新评论跟进

- [√] 1. 按最新评论重写 `ProtocolManager::GetGbRegisterConfig()` / `SetGbRegisterConfig()` 语义，并新增 `RestartGbRegisterService()`
- [√] 2. 在 `ProtocolManager::Start()` 前补一次本地配置重载，避免启动阶段继续使用旧缓存
- [√] 3. 进行最小编译级验证并同步 `helloagents` 知识库、CHANGELOG 与 history 归档
