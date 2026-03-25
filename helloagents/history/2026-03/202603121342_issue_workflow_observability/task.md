# 任务清单: Issue Workflow 可观测性补强

目录: `helloagents/plan/202603121342_issue_workflow_observability/`

---

## 1. 状态留痕
- [√] 1.1 为 `tools/issue_bot/repair_executor.py` 增加结构化状态文件和摘要输出
- [√] 1.2 为 `tools/issue_bot/runner_preflight.sh` 增加状态文件和摘要输出

## 2. Workflow 留痕
- [√] 2.1 在 `issue-repair.yml` 中上传 `ISSUE_BOT_STATE_DIR` artifact，并写入 step summary
- [√] 2.2 在 `issue-runner-preflight.yml` 中上传 `ISSUE_BOT_STATE_DIR` artifact，并写入 step summary

## 3. 文档同步
- [√] 3.1 更新 `tools/issue_bot/README.md`
- [√] 3.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md`

## 4. 验证
- [√] 4.1 运行语法与 YAML 静态检查
- [√] 4.2 运行本地 preflight / smoke，确认状态文件生成

## 执行备注
- 2026-03-12: `repair_executor.py` 已新增 `last-run.json` 和 `last-run-summary.md` 输出，覆盖成功、失败、空队列和拒绝执行等路径。
- 2026-03-12: `runner_preflight.sh` 已新增 `preflight.json` 和 `preflight-summary.md` 输出；若触发 `--with-local-smoke`，smoke repair 结果会落在 `ISSUE_BOT_STATE_DIR/smoke-test/`。
- 2026-03-12: 已验证 `issue-runner-preflight.yml` 和 `issue-repair.yml` 的 YAML 可解析，且本地 preflight + smoke 能正确生成状态文件树。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
