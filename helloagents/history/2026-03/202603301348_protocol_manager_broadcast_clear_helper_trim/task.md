# 任务清单: ProtocolManager 私有广播清理 helper 瘦身

目录: `helloagents/history/2026-03/202603301348_protocol_manager_broadcast_clear_helper_trim/`

## 1. 删除死 helper
- [√] 1.1 删除 `rk_gb/App/Protocol/ProtocolManager.h/.cpp` 中无调用的 `ClearGbBroadcastSessionState()`

## 2. 验证与收尾
- [√] 2.1 运行引用搜索，确认删除后无悬空依赖
- [√] 2.2 补充最小语法检查结果，能过则记录，通过不了则注明缺失头路径
- [√] 2.3 更新 `CHANGELOG.md`、`history/index.md` 并迁移方案包到 `history/`

## 执行结果
- `App/`、`doc/` 与 `helloagents/wiki/` 中已无 `ClearGbBroadcastSessionState` 活引用
- 已尝试对 `ProtocolManager.cpp` 做最小语法检查，但当前环境缺少 `libavcodec/avcodec.h` 头文件，无法在本机完成该项校验
