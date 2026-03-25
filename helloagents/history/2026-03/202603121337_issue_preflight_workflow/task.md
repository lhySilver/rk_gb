# 任务清单: Issue Preflight Workflow 接入

目录: `helloagents/plan/202603121337_issue_preflight_workflow/`

---

## 1. Workflow 接入
- [√] 1.1 在 `issue-repair.yml` 中接入 runner preflight 步骤，确保 repair 前先校验 runner 环境
- [√] 1.2 新增 `.github/workflows/issue-runner-preflight.yml`，提供手工预检入口

## 2. 文档同步
- [√] 2.1 更新 `tools/issue_bot/README.md`
- [√] 2.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 2.3 更新 `helloagents/CHANGELOG.md`

## 3. 验证
- [√] 3.1 校验 workflow YAML 语法和脚本调用路径
- [√] 3.2 复查预检与 repair 触发路径是否一致

## 执行备注
- 2026-03-12: `issue-repair.yml` 已提升为 job 级公共 env，并在 repair 前统一执行 `runner_preflight.sh`。
- 2026-03-12: 已新增 `issue-runner-preflight.yml`，支持从 GitHub Actions 页面手工触发 runner 预检、local smoke 和可选 build verify。
- 2026-03-12: 已使用本机 Python YAML 解析对 `issue-triage.yml`、`issue-repair.yml`、`issue-runner-preflight.yml` 做静态语法校验。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
