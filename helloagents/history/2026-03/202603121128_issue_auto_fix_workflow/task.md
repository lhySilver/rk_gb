# 任务清单: GitHub Issue 自动巡检与修复工作流

目录: `helloagents/plan/202603121128_issue_auto_fix_workflow/`

---

## 1. 工作流骨架
- [√] 1.1 在 `.github/workflows/issue-triage.yml` 中实现定时巡检和手工触发入口，验证 why.md#需求-定时巡检-open-issues-场景-周期巡检问题池
- [√] 1.2 在 `.github/workflows/issue-repair.yml` 中实现候选 issue 修复入口和 self-hosted runner 绑定，验证 why.md#需求-自动修复候选-issue-场景-自修复并开-pr，依赖任务1.1

## 2. 规则与执行器
- [√] 2.1 在 `tools/issue_bot/triage_rules.*` 中实现白名单/黑名单规则与 issue 风险评分，验证 why.md#需求-控制自动化风险-场景-拒绝高风险-issue
- [√] 2.2 在 `tools/issue_bot/repair_executor.*` 中实现 issue 上下文提取、分支命名、修复执行和日志输出，验证 why.md#需求-自动修复候选-issue-场景-自修复并开-pr，依赖任务2.1
- [√] 2.3 在 `tools/issue_bot/build_verify.*` 中接入 RK830 隔离交叉编译命令，验证 why.md#需求-自动修复候选-issue-场景-自修复并开-pr，依赖任务2.2

## 3. 权限与安全
- [√] 3.1 在 workflow 中声明最小 `permissions`，并补充仓库侧必需设置说明，验证 why.md#需求-控制自动化风险-场景-拒绝高风险-issue
- [√] 3.2 在 runner 使用说明中定义 worktree/临时目录清理、日志保留、禁止直接使用 issue 原文执行 shell 的规则，验证 why.md#需求-控制自动化风险-场景-拒绝高风险-issue

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/build_runtime.md`
- [√] 4.2 更新 `helloagents/project.md`
- [√] 4.3 补充自动化运维相关文档（如 `helloagents/wiki/modules/github_automation.md`）

## 5. 测试
- [√] 5.1 用 `workflow_dispatch` 和模拟 issue 编号验证 triage 逻辑
- [-] 5.2 在专用 self-hosted runner 上验证 repair 流程的 dry-run
- [-] 5.3 验证“修复成功开 PR / 修复失败回写 issue / 高风险 issue 跳过”三条分支路径

## 执行备注
- 2026-03-12: 已本地验证 `triage_issues.py --mock-json --dry-run`，覆盖构建类 issue 与协议讨论类 issue 的分流结果。
- 2026-03-12: 已用临时 git 仓库验证 `repair_executor.py` 的 worktree 创建/清理骨架，但未在真实 GitHub self-hosted runner 事件链中执行。
- 2026-03-12: 未在真实 GitHub Actions 环境中验证“推分支、创建 PR、失败回写 issue”完整链路，相关任务按未执行记录。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
