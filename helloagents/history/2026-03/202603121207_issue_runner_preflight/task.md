# 任务清单: Issue Runner 预检支撑

目录: `helloagents/plan/202603121207_issue_runner_preflight/`

---

## 1. runner 预检脚本
- [√] 1.1 新增 `tools/issue_bot/runner_preflight.sh`，检查 `git`、`python3`、`RK_TOOLCHAIN_BIN`、`CMAKE_BIN`、`ISSUE_FIX_COMMAND`
- [√] 1.2 为 `runner_preflight.sh` 增加可选 smoke test / build verify 执行开关
- [√] 1.3 新增 `tools/issue_bot/runner_env.example`，明确 runner 侧推荐变量

## 2. 文档同步
- [√] 2.1 更新 `tools/issue_bot/README.md`，补充 runner 上线前检查步骤
- [√] 2.2 更新 `helloagents/wiki/modules/github_automation.md`
- [√] 2.3 更新 `helloagents/CHANGELOG.md`

## 3. 验证
- [√] 3.1 运行脚本语法检查
- [√] 3.2 用当前工作区配置运行 `runner_preflight.sh`

## 执行备注
- 2026-03-12: `runner_preflight.sh` 已支持 `--with-local-smoke` 和 `--with-build-verify` 两类可选验证路径。
- 2026-03-12: 已在当前工作区用真实 `RK_TOOLCHAIN_BIN`、私有 `cmake` 和示例 `ISSUE_FIX_COMMAND` 跑通 preflight + local smoke。
- 2026-03-12: 已在当前工作区跑通 preflight + build verify，确认脚本可串联 RK830 隔离交叉编译校验。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
