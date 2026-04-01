# 技术设计: 调整 GAT1400CaptureControl 代码归属路径

## 技术方案
- 将 `GAT1400CaptureControl.h/.cpp` 物理迁移到 `App/Protocol/gat1400/`。
- `GAT1400ClientService.h` 改为直接包含同目录下的 `GAT1400CaptureControl.h`。
- `App/CMakeLists.txt` 和 `App/Protocol/gb28181/sdk_port/CMakeLists.txt` 中，将该源文件从 `Media_SRC` 调整到 `Protocol_SRC`，并改成新路径。
- 知识库中的当前态说明统一改为 `App/Protocol/gat1400/GAT1400CaptureControl.*`。

## 实现要点
- 不改 `media::GAT1400CaptureEvent`、`media::GAT1400CaptureControl` 的命名空间和接口签名，避免影响现有调用面。
- 不改 `NotifyGatAlarm()`、`NotifyCaptureEvent()`、`DrainPendingCaptureEvents()` 的行为。
- 不改图片 / 视频 / 文件 `/Data` Base64 上传逻辑。

## 风险与缓解
- **风险:** 文件移动后若 include 或 CMake 路径遗漏，会直接造成编译失败。
- **缓解:** 完成后执行 `git diff --check`，并对 `GAT1400CaptureControl.cpp`、`GAT1400ClientService.cpp` 做最小语法检查。
