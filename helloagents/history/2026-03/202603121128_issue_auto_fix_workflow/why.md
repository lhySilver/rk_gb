# 变更提案: GitHub Issue 自动巡检与修复工作流

## 需求背景
`rk_gb` 是 GitHub 上持续维护的工程，期望通过自动化定期扫描仓库 Issue，筛选适合机器处理的问题，自动尝试修复，并以分支/PR 的形式提交结果，减少人工盯盘和重复性修复成本。

当前项目存在以下现实约束：
- `rk_gb` 依赖 Rockchip 本地交叉编译工具链和板级构建环境，普通 GitHub Hosted Runner 难以稳定复现。
- 项目尚无完整自动化测试体系，修复质量需要依赖“隔离交叉编译 + 有限规则筛选”保证下限。
- Issue 正文、标题、评论都属于不可信输入，不能直接拼接到 shell、脚本或自动修复 prompt 中。
- 自动化如果直接推送主分支，风险过高；更合理的目标是“自动修复并开 PR，由人工审核合并”。

## 变更内容
1. 设计一套 GitHub Actions + self-hosted runner 的自动巡检/自动修复工作流。
2. 明确工作流权限边界、认证方式、Issue 筛选规则和 runner 隔离规则。
3. 为后续开发实施拆解可执行任务，覆盖工作流文件、修复执行脚本和验证流程。

## 影响范围
- **模块:** GitHub Actions 工作流、仓库运维流程、交叉编译验证流程
- **文件:** `.github/workflows/*`、可能新增 `tools/issue_bot/*`、`docs/*` 或 `helloagents/wiki/*`
- **API:** GitHub REST API（Issue/Label/Comment/PR）
- **数据:** GitHub Issue 标签、评论、自动生成分支和 PR 元数据

## 核心场景

### 需求: 定时巡检 open issues
**模块:** Workflow/Triage
工作流按固定周期查看仓库 open issues，并标记哪些问题适合自动处理。

#### 场景: 周期巡检问题池
条件描述:
- 工作流由 `schedule` 或 `workflow_dispatch` 触发
- 仓库中存在 open issues
- 预期结果1: 自动拉取 issue 列表
- 预期结果2: 基于标签/关键词/模板完整度筛选候选 issue
- 预期结果3: 为候选问题打上专用标签，并为跳过问题记录原因

### 需求: 自动修复候选 issue
**模块:** Workflow/Repair
工作流针对已经筛选出的候选 issue，进入隔离目录完成修复与交叉编译验证。

#### 场景: 自修复并开 PR
条件描述:
- issue 已被 triage 标记为候选
- self-hosted runner 可访问本地 RK 工具链和仓库
- 预期结果1: 工作流创建独立工作目录或 worktree
- 预期结果2: 修复逻辑仅处理低风险、可编译验证的问题
- 预期结果3: 交叉编译通过后创建修复分支并发起 PR
- 预期结果4: 不直接推送主分支

### 需求: 控制自动化风险
**模块:** Security/Operations
避免敏感凭据、恶意 issue 输入和受污染 runner 导致仓库或宿主机受损。

#### 场景: 拒绝高风险 issue
条件描述:
- issue 涉及协议联调、需求讨论、生产凭据、模糊需求或高风险脚本执行
- 预期结果1: 工作流不会进入自动修复
- 预期结果2: issue 被标记为人工处理
- 预期结果3: 记录跳过原因，便于后续人工接管

## 风险评估
- **风险:** self-hosted runner 执行不可信 issue 内容会带来命令注入或 secrets 泄露风险
- **缓解:** 所有 issue 输入都做结构化提取与转义；不直接拼 shell；修复脚本只读取白名单字段
- **风险:** GitHub `schedule` 存在延迟或丢执行
- **缓解:** 避开整点，支持 `workflow_dispatch` 手工补跑
- **风险:** `rk_gb` 构建依赖本地工具链，Hosted Runner 无法稳定复现
- **缓解:** 强制使用专用 self-hosted runner，并复用已验证的隔离交叉编译命令
- **风险:** 自动修复误提交协议类复杂问题
- **缓解:** 首阶段仅放开编译失败、缺失依赖、路径/构建脚本等低风险 issue
