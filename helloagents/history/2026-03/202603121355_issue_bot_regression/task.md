# 任务清单: Issue Bot 本地回归补强

目录: `helloagents/plan/202603121355_issue_bot_regression/`

---

## 1. triage 健壮性
- [√] 1.1 为 `tools/issue_bot/triage_issues.py` 增加失败状态和空结果状态落盘
- [√] 1.2 统一 triage 状态文件字段，便于和 repair/preflight 对齐

## 2. 本地回归脚本
- [√] 2.1 新增 `tools/issue_bot/regression_suite.sh`，串联 triage mock、runner preflight、local smoke
- [√] 2.2 让回归脚本输出关键状态文件位置和结果摘要

## 3. 文档同步
- [√] 3.1 更新 `tools/issue_bot/README.md`
- [√] 3.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md`

## 4. 验证
- [√] 4.1 运行 Python / shell 语法检查
- [√] 4.2 运行本地回归脚本

## 执行备注
- 2026-03-12: `triage_issues.py` 现已覆盖 `success`、`failed`、`no_open_issues` 三类状态语义，并继续输出 triage 统计摘要。
- 2026-03-12: 已新增 `regression_suite.sh`，默认串联 triage mock、runner preflight 和 local smoke；可选追加 `--with-build-verify`。
- 2026-03-12: 已在当前工作区跑通本地回归脚本，状态目录包含 triage、preflight 和 smoke repair 三类摘要文件。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
