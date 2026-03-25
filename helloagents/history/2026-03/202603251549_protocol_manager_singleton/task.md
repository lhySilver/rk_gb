# 轻量迭代任务 - protocol_manager_singleton

> 类型: 轻量迭代
> 状态: 已完成
> 目标: 只对外开放 `ProtocolManager`，并将 `GB28181ClientSDK` 的所有权和生命周期下沉到 `ProtocolManager` 内部。

## 任务清单

- [√] 调整 `App/Protocol/ProtocolManager.h`、`App/Protocol/ProtocolManager.cpp`，新增 `ProtocolManager::Instance()` / `InstanceIfCreated()`，并收拢 `GB28181ClientSDK` 的创建、绑定、释放逻辑
- [√] 调整 `App/Main.cpp`、`App/Main.h`，移除 `CSofia` 对 `ProtocolManager` 和 `GB28181ClientSDK` 的持有与转发
- [√] 调整 `App/Protocol/gat1400/LowerGAT1400SDK.cpp`，直接复用 `ProtocolManager::Instance()` 访问运行中的协议服务
- [√] 更新 `helloagents/wiki/modules/protocol.md`、`helloagents/wiki/modules/gb28181.md`、`helloagents/wiki/modules/gat1400.md`、`helloagents/wiki/data.md`、`helloagents/wiki/overview.md`
- [√] 更新 `helloagents/CHANGELOG.md`
- [√] 更新 `helloagents/history/index.md`
