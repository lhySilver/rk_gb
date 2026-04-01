# 变更提案: 调整 GAT1400CaptureControl 代码归属路径

## 需求背景
issue 41 明确指出：`GAT1400CaptureControl.h/.cpp` 当前放在 `App/Media/`，但这组代码没有调用媒体层能力，也只被 GAT1400 模块使用。

继续把它留在 `Media` 目录，会让模块边界显得模糊，也会让后续维护者误判这部分代码和音视频采集链路存在强耦合。

## 变更目标
1. 将 `GAT1400CaptureControl.h/.cpp` 从 `App/Media/` 迁移到 `App/Protocol/gat1400/`。
2. 同步调整源码 include 和构建脚本中的源文件归属。
3. 同步更新当前知识库中对这组代码路径的描述，避免文档继续指向旧目录。

## 核心场景

### 需求: 1400 抓拍桥接归属收口到协议模块
**模块:** `GAT1400CaptureControl`、`GAT1400ClientService`

#### 场景: 算法/编码侧通知 1400 模块上传抓拍结果
算法或编码侧通过 `ProtocolManager::NotifyGatAlarm()` 或内部桥接队列提交抓拍结果。
- 抓拍事件继续使用现有 `media::GAT1400CaptureEvent`
- 上传时序、排队语义和 `/Data` Base64 逻辑保持不变
- 但桥接控制器源码位置调整到 `App/Protocol/gat1400/`，明确它属于 1400 协议实现的一部分

## 影响范围
- `rk_gb/App/Protocol/gat1400/GAT1400CaptureControl.*`
- `rk_gb/App/Protocol/gat1400/GAT1400ClientService.h`
- `rk_gb/App/CMakeLists.txt`
- `rk_gb/App/Protocol/gb28181/sdk_port/CMakeLists.txt`
- `helloagents/wiki/modules/gat1400.md`
- `helloagents/wiki/api.md`
- `helloagents/wiki/data.md`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
