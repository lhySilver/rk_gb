# 变更提案: issue49 收口 GAT1400 结构化上报非阻塞与单次重发

## 需求背景
issue 49 要求外部模块调用 GAT1400 结构化对象上报入口时不要被同步 HTTP 上传阻塞。当前 `ProtocolManager::NotifyGatFaces()`、`NotifyGatMotorVehicles()`、`NotifyGatNonMotorVehicles()` 在 1400 已注册时会直接调用 `PostFaces/PostMotorVehicles/PostNonMotorVehicles`，外部调用方会被网络请求、重试和平台响应时间拖住。

随后又补充了两个实现约束：
1. 按“方案1”处理，即接口立即返回，实际发送改为后台异步执行。
2. 整条生命周期只允许重发一次，也就是同一条上报最多发送 2 次（首次发送 + 1 次重发），之后不再继续进入长期补传。

## 变更目标
1. 让 3 个 `NotifyGat*` 对外接口改成非阻塞返回，不再在调用线程里直接做 HTTP 上传。
2. 保持变更范围只落在 issue49 相关的 3 个通知入口和 GAT1400 上传队列调度，不改其他 `Post*` 接口和全局编译参数。
3. 为 issue49 增加最小回归，固定“异步入队 + 最多 2 次总发送”的约束。

## 非目标
- 不改变 `LOWER_1400_POST_*` 或其他直接 `Post*` 调用面的同步语义。
- 不改全局 `gat_upload.retry_policy` 的默认配置含义。
- 不重构 GAT1400 上传模块的整体架构，只做 issue49 所需的最小收口。
