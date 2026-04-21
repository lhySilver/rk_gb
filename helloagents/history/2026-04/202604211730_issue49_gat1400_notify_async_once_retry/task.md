# 任务清单: issue49 收口 GAT1400 结构化上报非阻塞与单次重发

## 1. 最小回归
- [√] 1.1 新增 issue49 回归脚本，先固定旧实现应失败的非阻塞/重发约束

## 2. 代码实现
- [√] 2.1 在 `GAT1400ClientService` 上传队列项中补充 issue49 所需的最大尝试次数元数据
- [√] 2.2 为 `GAT1400ClientService` 增加后台异步回放 worker，并在 issue49 入队后立即唤醒
- [√] 2.3 把 `NotifyFaces/NotifyMotorVehicles/NotifyNonMotorVehicles` 改成快速入队，最多总发送 2 次

## 3. 知识库与验证
- [√] 3.1 更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`
- [√] 3.2 执行 issue49 回归、语法校验和工作区检查

## 执行结果

- `GAT1400ClientService::PendingUploadItem` 新增 `max_attempt_count` 元数据，并持久化到补传队列文件；issue49 这 3 个 `NotifyGat*` 入口统一写入 `max_attempt_count=2`。
- 新增 `PendingReplayLoop()` 后台 worker，并通过 `RequestPendingReplay()` 在 1400 已注册时即时异步回放 issue49 队列项；外部调用线程不再同步发 HTTP。
- `ReplayPendingUploadsInternal()` 现在会按单条上传的 `max_attempt_count` 裁剪总发送次数；issue49 队列项达到 `2` 次总尝试后会直接丢弃，不再继续长期补传。
- 已同步更新 `helloagents/wiki/api.md`、`helloagents/wiki/data.md`、`helloagents/wiki/modules/gat1400.md`、`helloagents/wiki/modules/protocol.md` 与 `helloagents/CHANGELOG.md`，把旧的“已注册直接发送”描述收口到 issue49 新语义。
- 已验证通过：`python3 tools/tests/issue49_gat_notify_async_once_retry_regression.py`、`python3 tools/tests/issue45_gat_enabled_regression.py`、`python3 tools/tests/issue45_gat_online_status_regression.py`、`g++ -std=c++11 -fsyntax-only -fpermissive .../GAT1400ClientService.cpp`、`git diff --check`。
