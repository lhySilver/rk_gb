# 轻量迭代任务 - gb_register_cold_start_retry

> 类型: 轻量迭代
> 状态: 已完成
> 目标: 修复 GB28181 冷启动时首次注册失败后生命周期直接退出、后续不再重试的问题。

## 任务清单

- [√] 调整 `App/Protocol/ProtocolManager.cpp`，让可恢复的首次 GB 注册失败改为保留生命周期并延后重试
- [√] 补充 `GbHeartbeatLoop` 未注册态的重注册逻辑和诊断日志
- [√] 更新 `helloagents/wiki/modules/protocol.md`、`helloagents/wiki/modules/gb28181.md`
- [√] 更新 `helloagents/CHANGELOG.md`
- [√] 更新 `helloagents/history/index.md`
