# GitHubAutomation

## 目的
沉淀 `rk_gb` 的 GitHub issue 自动巡检、自动修复和 PR 收口流程。

## 模块概述
- **职责:** 定时筛选 issue、控制自动修复入口、绑定 self-hosted runner、执行隔离交叉编译验证并创建 PR
- **状态:** 🚧已落骨架
- **最后更新:** 2026-03-12
- **核心边界:** 自动化只开分支和 PR，不直接推主分支

## 规范

### 需求: 自动巡检仓库 issues
**模块:** GitHubAutomation
对 open issues 做周期扫描，筛选适合自动处理的低风险问题。

#### 场景: 定时 triage
前置条件:
- GitHub Actions `issue-triage.yml` 已部署
- 仓库 `GITHUB_TOKEN` 可写 issue 标签和评论
- 预期结果1: 低风险 issue 被标记为 `ha-candidate`
- 预期结果2: 高风险或信息不足的 issue 被标记为 `ha-manual`
- 预期结果3: triage 结果回写到 issue 评论

#### 场景: repair 工作流自动开 PR
前置条件:
- issue 已被标记为 `ha-candidate`
- self-hosted runner 已配置 RK830 工具链和可用 cmake
- runner 上已提供 `ISSUE_FIX_COMMAND`
- 预期结果1: repair 工作流创建独立 worktree
- 预期结果2: 修复完成后执行 RK830 隔离交叉编译验证
- 预期结果3: 构建通过后推送修复分支并创建 PR
- 预期结果4: issue 被打上 `ha-pr-open`

## API接口

### `triage_issues.py`
**描述:** 调用 GitHub Issues API，完成规则筛选和标签/评论回写。
**输入:** `GITHUB_TOKEN`、`GITHUB_REPOSITORY`、可选 `--issue-number`
**输出:** triage 分类结果、标签更新、issue 评论，以及 triage 状态摘要

### `repair_executor.py`
**描述:** 选择候选 issue，创建隔离 worktree，调用 runner 本地修复器，验证构建并创建 PR。
**输入:** `ISSUE_FIX_COMMAND`、`RK_TOOLCHAIN_BIN`、`CMAKE_BIN`
**输出:** 修复分支、构建日志、PR 链接或失败评论

### `local_smoke_test.sh`
**描述:** 在临时 git 仓库中模拟一次 repair 流程，不依赖真实 GitHub API。
**输入:** 本地 `git`、`python3`
**输出:** smoke test 通过/失败结果，以及 worktree 清理验证

### `runner_preflight.sh`
**描述:** 在 self-hosted runner 上检查工具链、修复器和可选构建能力，作为启用定时 repair 前的预检入口。
**输入:** `RK_TOOLCHAIN_BIN`、`CMAKE_BIN`、`ISSUE_FIX_COMMAND`
**输出:** 预检摘要，以及可选 smoke test / build verify 结果

### `regression_suite.sh`
**描述:** 在本地串联 triage mock、runner preflight、local smoke 和可选 build verify，作为改动后的统一回归入口。
**输入:** 本地 `git`、`python3`、可选 `RK_TOOLCHAIN_BIN` / `CMAKE_BIN`
**输出:** triage / preflight / smoke repair 摘要路径，以及可选 build verify 日志路径

## 数据模型

### 标签约定
| 标签 | 说明 |
|------|------|
| `ha-candidate` | 允许自动修复的低风险 issue |
| `ha-manual` | 需要人工处理 |
| `ha-in-progress` | 自动修复处理中 |
| `ha-failed` | 自动修复失败 |
| `ha-pr-open` | 已创建修复 PR |

### repair 队列语义
| 场景 | 行为 |
|------|------|
| 无待处理 `ha-candidate` | `issue-repair.yml` 正常退出，不报失败 |
| 已带 `ha-failed` 的 issue | 定时 repair 跳过，不自动重试 |
| 人工确认重试 | 移除 `ha-failed` 后重新添加 `ha-candidate` |

### runner 环境变量
| 变量 | 说明 |
|------|------|
| `GITHUB_TOKEN` | GitHub Actions 令牌 |
| `RK_TOOLCHAIN_BIN` | RK830 工具链目录 |
| `CMAKE_BIN` | cmake 可执行文件 |
| `ISSUE_FIX_COMMAND` | runner 本地修复命令 |
| `ISSUE_BOT_STATE_DIR` | 临时状态目录 |

### 留痕文件
| 文件 | 说明 |
|------|------|
| `ISSUE_BOT_STATE_DIR/last-run.json` | repair 执行结果的结构化状态 |
| `ISSUE_BOT_STATE_DIR/last-run-summary.md` | repair 执行摘要，供 GitHub Step Summary 展示 |
| `ISSUE_BOT_STATE_DIR/triage-last-run.json` | triage 执行结果的结构化状态 |
| `ISSUE_BOT_STATE_DIR/triage-last-run-summary.md` | triage 执行摘要，供 GitHub Step Summary 展示 |
| `ISSUE_BOT_STATE_DIR/preflight.json` | runner 预检结果的结构化状态 |
| `ISSUE_BOT_STATE_DIR/preflight-summary.md` | runner 预检摘要，供 GitHub Step Summary 展示 |

## 依赖
- `.github/workflows/issue-triage.yml`
- `.github/workflows/issue-repair.yml`
- `.github/workflows/issue-runner-preflight.yml`
- `tools/issue_bot/common.py`
- `tools/issue_bot/triage_rules.py`
- `tools/issue_bot/triage_issues.py`
- `tools/issue_bot/repair_executor.py`
- `tools/issue_bot/build_verify.sh`

## 注意事项
- 认证必须使用 `GITHUB_TOKEN`、GitHub App 或细粒度 PAT，不能把账号密码写入仓库。
- Issue 标题和正文属于不可信输入，禁止直接拼接进 shell。
- `issue-repair.yml` 必须运行在专用 self-hosted runner 上，且 runner 需要和人工开发环境隔离。
- `schedule` 触发不应直接引用 `workflow_dispatch` 的 `inputs` 上下文；当前 workflow 已拆成“定时路径”和“手工路径”两个 step 分别处理。
- `issue-repair.yml` 已在执行修复前串联 `runner_preflight.sh`，用同一套 runner 变量做基础环境核验。
- `repair_executor.py` 在成功和失败路径都会清理临时 `git worktree` 与本地修复分支，避免 runner 常驻仓库积累脏状态。
- repair 定时任务在空候选队列场景下应返回成功并记录日志，而不是制造 workflow 失败噪声。
- 自动修复失败的 issue 默认退出 `ha-candidate` 队列并保留 `ha-failed` 标签，后续是否重试由人工复核决定。
- 当前仓库已补充 `--mock-issue-json` 和 `local_smoke_test.sh`，可先在本地验证 repair 骨架，再接入真实 self-hosted runner。
- 当前仓库已补充 `runner_preflight.sh` 和 `runner_env.example`，用于真实 runner 接线前的环境核验。
- 当前仓库已补充 `issue-runner-preflight.yml`，可直接从 GitHub Actions 页面触发 runner 自检。
- triage / repair / preflight 三条 workflow 现在都会上传 `ISSUE_BOT_STATE_DIR` artifact，并在 Actions 页面展示摘要。
- 当前仓库已补充 `regression_suite.sh`，可以在本地一键回归 triage / repair / preflight 的 mock 主链路。
- repair 与 preflight workflow 会自动上传 `ISSUE_BOT_STATE_DIR` artifact，方便回看日志、issue 上下文和状态文件。
- 当前仓库仅落了 workflow 和执行器骨架，真正的修复器本体由 runner 侧 `ISSUE_FIX_COMMAND` 提供。

## 变更历史
- 2026-03-12: 新增 issue 自动巡检/自动修复模块文档，记录 workflow、标签和 runner 约束
- 2026-03-12: 修正 schedule/workflow_dispatch 的上下文边界，并补充 repair 执行后的 worktree 清理约束
- 2026-03-12: 新增本地 smoke test 能力，允许用 mock issue 验证 repair 执行器主流程
- 2026-03-12: 加固 repair 队列语义，补充空队列成功退出和失败 issue 退队规则
- 2026-03-12: 新增 runner 预检脚本和变量示例，降低 self-hosted runner 接线风险
- 2026-03-12: 将 runner 预检接入 repair workflow，并新增手工预检 workflow
- 2026-03-12: 为 repair / preflight 增加状态文件、step summary 和 artifact 留痕
- 2026-03-12: 为 triage 增加状态文件、step summary 和 artifact 留痕
- 2026-03-12: 新增本地 `regression_suite.sh`，统一回归 triage / preflight / smoke repair 主链路
