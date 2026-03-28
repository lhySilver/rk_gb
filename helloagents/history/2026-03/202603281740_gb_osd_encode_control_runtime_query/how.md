# 技术设计: 补齐 GB OSD / 编码参数控制链路的运行态查询闭环

## 技术方案
### 核心技术
- 在 `ProtocolManager::HandleGbConfigControl()` 中，把 `VideoParamAttribute` 改成“先汇总每路目标值，再统一查询运行态并决定是否 apply”。
- 为 OSD 增加显式 `QueryVideoOsdState()` 比对逻辑；在媒体状态中补齐 `date_style/time_style` 运行态字段，支撑协议层判断时间格式是否已经落地。
- 保持真正的设备设置仍由 `App/Media/VideoOsdControl.*`、`App/Media/VideoEncodeControl.*` 承接。

## 实现要点
- `App/Media/VideoOsdControl.h/.cpp`
  - 扩展 `VideoOsdState`，新增 `date_style/time_style` 运行态字段
  - `QueryVideoOsdState()` 增加 `rk_osd_get_date_style()` / `rk_osd_get_time_style()` 读取
- `App/Protocol/ProtocolManager.cpp`
  - 新增编码参数目标汇总 / merge / compare helper
  - 新增 OSD 时间格式 compare helper
  - `HandleGbConfigControl()` 在控制收尾阶段显式查询运行态，并输出 `skip_apply` / `dispatch` 日志

## 架构决策 ADR
### ADR-20260328-02: OSD / 编码参数控制侧也由 GB 模块显式承担运行态查询
**上下文:** 用户要求与画面反转保持一致，GB 模块必须把媒体查询边界明确暴露出来，设备底层实现仍由编码/媒体模块负责。
**决策:** 在 `ProtocolManager` 控制侧显式调用 `QueryVideoOsdState()` / `QueryVideoEncodeState()`，由协议层决定是否真正下发设置。
**理由:** 这样既满足“GB 模块边界清晰可审查”，也不破坏媒体层统一承接设备细节的职责。

## 测试与部署
- 使用 `git diff --check` 校验补丁格式
- 生成隔离 `build-rk830` / `Middleware/build-rk830` 构建目录
- 使用交叉编译器对 `App/Media/VideoOsdControl.cpp`、`App/Protocol/ProtocolManager.cpp` 做最小对象编译验证
