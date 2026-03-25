# 任务清单: Issue Workflow 本地演练支撑

目录: `helloagents/plan/202603121156_issue_workflow_smoke_test/`

---

## 1. 执行器增强
- [√] 1.1 为 `tools/issue_bot/repair_executor.py` 增加本地 mock issue 演练入口，避免本地验证依赖 GitHub API
- [√] 1.2 为 `tools/issue_bot/repair_executor.py` 增加可选的跳过构建验证开关，便于只验证 worktree / commit / cleanup 骨架

## 2. 本地演练脚本
- [√] 2.1 新增本地 smoke test 脚本，自动创建临时 git 仓库并演练 repair 流程
- [√] 2.2 新增示例修复器脚本，作为 `ISSUE_FIX_COMMAND` 的最小安全示例

## 3. 文档同步
- [√] 3.1 更新 `tools/issue_bot/README.md`，补充本地 smoke test 和 mock issue 用法
- [√] 3.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 3.3 更新 `helloagents/CHANGELOG.md`

## 4. 验证
- [√] 4.1 运行 Python 语法检查
- [√] 4.2 运行本地 smoke test，确认 repair 骨架可在临时仓库中跑通

## 执行备注
- 2026-03-12: `repair_executor.py` 新增 `--mock-issue-json` 与 `--skip-build-verify`，本地演练不再依赖 GitHub API 和 RK830 工具链。
- 2026-03-12: `local_smoke_test.sh` 已成功在临时 bare repo + worktree 场景下跑通，验证了改动提交与 cleanup 逻辑。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
