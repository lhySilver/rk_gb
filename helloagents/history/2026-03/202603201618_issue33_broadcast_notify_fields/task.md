# 任务清单

- [√] 分析 reopened 的 issue #33 附件，确认失败仍停留在广播 `NOTIFY` 业务响应阶段
- [√] 修复广播通知 XML 解包时 `SN/DeviceID` 丢失的问题
- [√] 为广播会话 `gb_code` 增加 `TargetID`/本机设备 ID 兜底
- [√] 使用隔离目录 `/tmp/rk_gb_issue33_reopen_build` 完成交叉编译验证
