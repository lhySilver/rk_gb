# 变更提案: 清理 ProtocolManager 中未调用的包装方法

## 需求背景
继续做“只减体积、不改行为”的瘦身时，`ProtocolManager` 里还残留了一组明显的死代码：
- `GBManager_Start()`
- `GBManager_Stop()`
- `OnGbConfigChanged()`

全仓搜索后，这三个方法只剩声明和定义，没有任何调用点。同时，内部 helper `ClearGbUpgradePendingState(...)` 还保留了一个未使用的 `manager` 参数，编译器已给出 `unused parameter` 警告。

## 变更目标
1. 删除全仓无调用的 `ProtocolManager` 包装方法。
2. 删除 `ClearGbUpgradePendingState()` 的未使用参数。
3. 继续在不改运行行为的前提下收缩 `ProtocolManager.cpp` 编译产物。

## 影响范围
- `rk_gb/App/Protocol/ProtocolManager.h`
- `rk_gb/App/Protocol/ProtocolManager.cpp`

## 风险评估
- 风险: 这些公开方法可能被仓库外部代码调用。
- 缓解: 当前工作区与知识库都没有任何引用，且三者只是 `Start/Stop/Reload` 的薄包装，不属于当前仓库内的正式接口面；本次仅按仓库内真实调用关系收口。
