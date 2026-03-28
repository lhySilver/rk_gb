# 变更提案: 补齐 GB OSD / 编码参数控制链路的运行态查询闭环

## 需求背景
代码审查继续指出，GB28181 画面反转已经补成了协议层显式 `query -> apply`，但 OSD 和编码参数控制路径里，`ProtocolManager::HandleGbConfigControl()` 仍主要是直接调用 `ApplyVideoOsdConfig()` / `ApplyVideoEncodeStreamConfig()`，运行态查询只隐藏在媒体实现内部，不利于边界审查和联调定位。

## 变更目标
1. 让 GB 模块在 OSD 与编码参数设置命令上显式调用 `media::QueryVideoOsdState()` / `media::QueryVideoEncodeState()`。
2. 在协议层根据运行态和目标值做显式 compare，运行态已一致时记录 `skip_apply`，避免重复下发。
3. 保持 `VideoOsdControl`、`VideoEncodeControl` 仍是唯一设备侧接口面，不把底层 `rk_osd_*` / `rk_video_*` 重新拉回 `ProtocolManager`。

## 影响范围
- **模块:** `GB28181`、`Media`
- **文件:** `App/Protocol/ProtocolManager.cpp`、`App/Media/VideoOsdControl.*`、知识库
- **行为边界:** 仅补控制侧显式运行态查询与比对，不改变现有 XML 字段映射和媒体层设备适配职责
