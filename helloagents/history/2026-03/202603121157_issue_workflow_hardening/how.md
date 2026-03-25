# 技术设计: Issue Workflow 稳定性加固

## 技术方案

### 核心策略
- 为 repair 执行器增加“空队列正常退出”语义，避免 `schedule` 任务在没有候选时失败。
- 明确失败 issue 的状态迁移：失败后打 `ha-failed` 并移除 `ha-candidate` / `ha-in-progress`，需要人工确认后才重新进入候选队列。
- 用本地 mock / stub 客户端验证队列选择和失败路径，不依赖真实 GitHub 事件。

## 实现要点

### 1. repair 队列语义
- `select_issue()` 在 `--pick-next` 模式下找不到候选时返回“空队列”状态，而不是直接抛错。
- `main()` 对空队列状态输出明确日志并返回 0，让 workflow 成功结束。

### 2. 失败状态迁移
- 失败路径保留 `ha-failed` 标签。
- 同时移除 `ha-candidate` 和 `ha-in-progress`，防止定时 repair 继续捞取该 issue。
- 文档里明确“人工复核后再重新加回 `ha-candidate`”的恢复流程。

### 3. 验证策略
- 用 `python3 -m py_compile` 校验脚本语法。
- 用最小临时测试脚本验证：
  - 空队列返回 0
  - `select_issue()` 跳过失败 issue
  - 异常路径会移除候选标签

## 风险与规避
- 风险: 失败后自动移除 `ha-candidate` 可能让 issue 暂时退出自动化闭环。
  - 规避: 这是有意的安全闸门；由人工确认后重新加回候选，更符合受控自动化目标。
- 风险: 空队列返回 0 后，workflow 可观察性下降。
  - 规避: 保留明确 stdout 日志，让运行记录可见且不触发失败告警。
