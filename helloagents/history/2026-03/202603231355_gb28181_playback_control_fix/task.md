# 轻量迭代任务清单: gb28181_playback_control_fix

- [√] 1. 修复 RTSP 回放控制命令解析，确保 `TEARDOWN/PLAY+Scale/PLAY+Range` 与业务语义一致
- [√] 2. 修复 `ProtocolManager` 回放控制分发，补齐停止、倍速、拖动后的恢复发流逻辑
- [√] 3. 修复回放启动中的硬编码 `+28800` 时间偏移，统一回放时间基线
- [√] 4. 完成本地验证，并同步知识库、`CHANGELOG.md` 与历史索引
