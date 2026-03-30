# 任务清单: ProtocolManager 死包装方法瘦身

目录: `helloagents/history/2026-03/202603301333_protocol_manager_dead_wrapper_trim/`

## 1. 删除无调用包装代码
- [√] 1.1 删除 `rk_gb/App/Protocol/ProtocolManager.h/.cpp` 中无调用的 `GBManager_Start()`、`GBManager_Stop()`、`OnGbConfigChanged()`
- [√] 1.2 删除 `ClearGbUpgradePendingState()` 的未使用 `manager` 参数并同步调用点

## 2. 验证与收尾
- [√] 2.1 运行引用搜索和最小语法检查，确认删除后无悬空依赖
- [√] 2.2 对比 `ProtocolManager.o` 体积变化，确认继续瘦身
- [√] 2.3 更新 `CHANGELOG.md`、`history/index.md` 并迁移方案包到 `history/`

## 执行结果
- `ProtocolManager.cpp` 最小语法检查已通过
- `GBManager_Start|GBManager_Stop|OnGbConfigChanged` 活源码引用已清零
- `ProtocolManager.o` 在同编译宏条件下 `dec` 从 `236017` 下降到 `234806`，本轮继续减少 `1211` 字节
