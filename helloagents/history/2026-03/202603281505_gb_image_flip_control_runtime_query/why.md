# 变更提案: 补齐 GB 画面反转控制链路的运行态查询闭环

## 需求背景
代码审查反馈指出，GB28181 画面反转在查询命令链路上已经会调用 `QueryVideoImageFlipMode()`，但平台下发设置命令时，`ProtocolManager::HandleGbConfigControl()` 只显式调用了 `ApplyVideoImageFlipMode()`，运行态查询仅隐藏在媒体实现内部，不满足“GB 模块要明确走媒体接口 query -> apply 闭环，底层 `rk_isp_get_*` 由其他模块同事实现”的边界要求。

## 变更目标
1. 让 GB 模块在平台画面反转设置命令上显式调用 `media::QueryVideoImageFlipMode()`。
2. 保持 `VideoImageControl` 作为唯一设备侧接口面，不把底层 RK ISP 读写重新拉回 `ProtocolManager`。
3. 在运行态已等于目标值时避免重复下发，减少无意义的设备侧调用。

## 影响范围
- **模块:** `GB28181`、`Media`
- **文件:** `App/Protocol/ProtocolManager.cpp`、知识库
- **行为边界:** 仅补控制侧 `query -> apply` 显式链路，不改变 `FrameMirror` 协议映射和 `VideoImageControl` 的底层实现责任
