# 技术设计: 补齐 GB 画面反转控制链路的运行态查询闭环

## 技术方案
### 核心技术
- 在 `ProtocolManager::HandleGbConfigControl()` 的 `kImageSetting` 路径中显式查询运行态画面反转模式
- 复用现有 `media::QueryVideoImageFlipMode()` / `media::ApplyVideoImageFlipMode()` 接口，不新增 GB 侧直连底层 ISP 的调用
- 如果运行态已与目标值一致，则只更新协议配置与日志，不重复下发设备侧设置

## 实现要点
- `App/Protocol/ProtocolManager.cpp`
  - 增加控制命令处理阶段的 `queriedRuntimeImageFlip/hasRuntimeImageFlip/runtimeImageFlipMode`
  - `kImageSetting` 到达时先调用 `media::QueryVideoImageFlipMode()`
  - `applyImageFlip` 阶段按“当前运行态 vs 目标值”判断是否需要实际 dispatch

## 架构决策 ADR
### ADR-20260328-01: GB 模块在控制侧显式承担画面反转运行态查询
**上下文:** 现有控制链路虽然最终会通过 `ApplyVideoImageFlipMode()` 间接触发查询，但从协议层看不到 `QueryVideoImageFlipMode()` 的直接调用，不利于审查和边界确认。
**决策:** 将运行态查询前移到 `ProtocolManager::HandleGbConfigControl()`，让 GB 模块显式走一次媒体查询接口，再决定是否下发设置。
**理由:** 保持媒体边界清晰，同时不要求协议层关心 `rk_isp_get_image_flip()` 的具体实现细节。

## 测试与部署
- 使用 `git diff --check` 校验补丁格式
- 使用交叉编译器对 `App/Protocol/ProtocolManager.cpp` 做最小对象编译验证
