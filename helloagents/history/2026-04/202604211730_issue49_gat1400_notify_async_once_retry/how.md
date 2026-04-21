# 技术设计: issue49 收口 GAT1400 结构化上报非阻塞与单次重发

## 技术方案
1. 为 issue49 新增最小回归脚本，先固定以下行为约束：
   - `NotifyFaces/NotifyMotorVehicles/NotifyNonMotorVehicles` 不再直接调用 `PostFaces/PostMotorVehicles/PostNonMotorVehicles`
   - issue49 入口统一走异步入队
   - issue49 入队项要带“最多 2 次总发送”的限制
2. 在 `GAT1400ClientService::PendingUploadItem` 中补充单条上传的最大尝试次数字段，并扩展序列化/反序列化。
3. 新增后台待发送 worker：
   - `NotifyGat*` 只负责快速入队
   - 已注册时唤醒后台 worker 立即回放队列
   - 未注册时继续沿用注册成功后的既有回放时机
4. 在 `ReplayPendingUploads()` 中按单条上传的最大尝试次数裁剪重发次数：
   - issue49 结构化通知项最多总发送 2 次
   - 达到上限后直接丢弃，不再保留在补传队列中
   - 其他非 issue49 上传项继续保留原有策略
5. 同步更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`。

## 设计取舍
- **采用方案:** 复用现有失败补传队列，并新增单 worker 做即时异步回放。
  - 优点: 代码改动集中，保留当前队列持久化能力，避免给 `NotifyGat*` 单独再造一套上传链路。
  - 缺点: 接口虽然不再被网络阻塞，但仍会保留一次本地入队/落盘成本。
- **未采用方案:** 在 `NotifyGat*` 里直接新起独立发送线程。
  - 原因: 生命周期管理和并发重复发送风险更高，不符合“最小必要改动”约束。

## 验证计划
- 先运行 issue49 回归脚本，确认旧实现失败。
- 实现后重新运行 issue49 回归脚本。
- 执行 `g++ -std=c++11 -fsyntax-only` 语法校验。
- 执行 `git diff --check` 确认无格式问题。
