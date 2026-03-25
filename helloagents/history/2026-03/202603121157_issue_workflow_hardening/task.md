# 任务清单: Issue Workflow 稳定性加固

目录: `helloagents/plan/202603121157_issue_workflow_hardening/`

---

## 1. repair 执行器
- [√] 1.1 调整 `tools/issue_bot/repair_executor.py` 的候选 issue 选取逻辑，验证空队列场景不会报错失败
- [√] 1.2 调整 `tools/issue_bot/repair_executor.py` 的失败状态迁移，验证失败 issue 不再保留 `ha-candidate`
- [√] 1.3 补充 `tools/issue_bot/repair_executor.py` 日志/返回码语义，便于 workflow 观察

## 2. 文档
- [√] 2.1 更新 `tools/issue_bot/README.md`，补充失败恢复和队列语义
- [√] 2.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 2.3 更新 `helloagents/CHANGELOG.md`

## 3. 测试
- [√] 3.1 运行 Python 语法检查
- [√] 3.2 以本地 stub 验证“无候选 issue 正常退出”
- [√] 3.3 以本地 stub 验证“失败 issue 退出候选队列”

## 执行备注
- 2026-03-12: `repair_executor.py` 的本地 mock 模式现在既支持单 issue 对象，也支持 issue 列表；传列表并配合 `--pick-next` 可模拟候选队列。
- 2026-03-12: 已验证空队列场景输出 `No pending ha-candidate issues found; exiting without repair` 并返回成功。
- 2026-03-12: 已验证失败路径会添加 `ha-failed`、移除 `ha-candidate` / `ha-in-progress`，并提示人工复核后再重新入队。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
