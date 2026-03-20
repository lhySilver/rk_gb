# 技术设计

## 方案
1. 保留 `ProtocolManager` 现有 GB 控制入口，但把图像翻转读取/应用从 `rk_isp_*image_flip` 改为 `CFG_CAMERA_PARAM.rotateAttr`。
2. 通过 `g_configManager.getConfig/setConfig` 与 `TExchangeAL<CameraParamAll>::getConfigV2/setConfigV2` 复用 Tuya 侧同一条翻转配置链路。
3. 扩展 `LocalConfigProvider` 的本地 INI 读写逻辑，新增 `image_flip_mode` 字段并补充日志摘要。
4. 在 `ProtocolManager::Start()` 和 `ReloadExternalConfig()` 中补上翻转配置下发，确保冷启动与热更新一致。
5. 在 `HandleGbTeleBootControl()` 中增加冷却时间判断，失败时保留原状态并输出拒绝日志。

## 影响范围
- `App/Protocol/ProtocolManager.cpp`
- `App/Protocol/ProtocolManager.h`
- `App/Protocol/config/LocalConfigProvider.cpp`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
- `helloagents/wiki/data.md`
- `helloagents/wiki/modules/protocol.md`

## 风险与规避
- 风险: GB 翻转如果继续走独立 RK ISP 接口，可能与 Camera/Alarm 当前配置状态不一致。
- 规避: 统一改走 `CFG_CAMERA_PARAM.rotateAttr`，保持和 Tuya 侧一致。
- 风险: 启动时强制下发翻转配置可能影响现有画面方向。
- 规避: 只在配置值变化时更新 `CFG_CAMERA_PARAM`，并复用原有 Camera 模块的配置变更回调。
- 风险: 远程重启冷却误伤合法命令。
- 规避: 冷却时间仍沿用 `gb_reboot.cooldown_sec`，默认值不变，仅阻止短时间重复触发。

## ADR
### ADR-20260320-issue32
- 决策: GB 画面翻转统一复用 `CFG_CAMERA_PARAM.rotateAttr` 主链路，不单独维护 RK ISP 旁路状态。
- 原因: 该链路已经被 Camera、Alarm 和 Tuya DP 侧长期使用，行为更稳定，也能自动继承现有配置回调。
