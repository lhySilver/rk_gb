# 任务清单: Issue Triage 可观测性补强

目录: `helloagents/plan/202603121347_issue_triage_observability/`

---

## 1. triage 状态留痕
- [√] 1.1 为 `tools/issue_bot/triage_issues.py` 增加结构化状态文件和摘要输出
- [√] 1.2 输出 triage 总数、候选数、人工处理数和 dry-run 状态

## 2. workflow 留痕
- [√] 2.1 在 `issue-triage.yml` 中接入 `ISSUE_BOT_STATE_DIR`
- [√] 2.2 在 `issue-triage.yml` 中追加 step summary 和 artifact 上传

## 3. 文档同步
- [√] 3.1 更新 `tools/issue_bot/README.md`
- [√] 3.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md`

## 4. 验证
- [√] 4.1 运行 Python 语法检查
- [√] 4.2 运行 triage mock 演练，确认状态文件生成
- [√] 4.3 运行 workflow YAML 静态检查

## 执行备注
- 2026-03-12: `triage_issues.py` 已新增 `triage-last-run.json` 和 `triage-last-run-summary.md`，记录 triage 总数、候选数、人工处理数和逐条结果。
- 2026-03-12: `issue-triage.yml` 已新增 job 级 `ISSUE_BOT_STATE_DIR`、step summary 和 artifact 上传。
- 2026-03-12: 已通过 mock issue 样例验证 triage 状态文件生成，并完成三条 workflow 的 YAML 静态解析。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
