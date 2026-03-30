# 变更提案: 统一 OSD 读写模型并补齐多文本协议支持

## 需求背景
当前媒体侧 OSD 接口把“设置目标”和“查询运行态”拆成了两套结构：
- `ApplyVideoOsdConfig(const VideoOsdConfig&)`
- `QueryVideoOsdState(VideoOsdState*)`

这会带来两个直接问题：
- 开发者在阅读 `ProtocolManager` 的 `query -> merge -> apply` 链路时，需要在 `VideoOsdConfig` 与 `VideoOsdState` 之间频繁转换，理解成本高。
- 白皮书 `OSDConfig` 已经支持 `SumNum + Item[]` 多文本，但当前媒体/协议内部仍只保留单文本 `text_template/text`，导致“协议先支持多文本、底层尽力映射”的边界没有在统一模型里表达出来。

## 变更目标
1. 将媒体侧 OSD 读写统一收敛到 `VideoOsdState`，避免一套接口两种结构。
2. 在 `VideoOsdState` 中显式承载多文本项，至少保证 GB28181 `OSDConfig` 查询/设置链路可以完整表达 `Item[]`。
3. 在底层 RK OSD 真实能力不变的前提下，保持“协议优先支持、SoC 尽力映射”的行为：
   - 查询应答优先按统一状态模型回显多文本。
   - 设置下发时仍只把 SoC 能落地的字段应用到底层设备。

## 影响范围
- **媒体接口:** `App/Media/VideoOsdControl.h/.cpp`
- **协议映射:** `App/Protocol/ProtocolManager.cpp`
- **外部配置模型:** `App/Protocol/config/ProtocolExternalConfig.h` 及相关读写链路
- **知识库:** `helloagents/wiki/modules/gb28181.md`、`helloagents/wiki/data.md`、`helloagents/CHANGELOG.md`

## 核心场景

### 场景1: 平台设置多文本 OSD
- 平台通过 `DeviceConfig + OSDConfig` 下发 `SumNum > 1` 的 `Item[]`
- 协议层将全部文本项写入统一 `VideoOsdState`
- 媒体层只把第 1 个文本项尽力映射到底层 RK OSD，其余文本项保留在统一状态模型中

### 场景2: 平台查询 OSDConfig
- 协议层通过 `QueryVideoOsdState()` 获取统一运行态
- 查询应答中的 `<OSDConfig>` 按 `VideoOsdState` 回显时间字段与多文本 `Item[]`
- 不再依赖单独的 `VideoOsdConfig` 中间模型

## 风险评估
- **风险:** `VideoOsdConfig` 删除或退场后，所有调用点都要迁移，容易遗漏编译错误。
- **缓解:** 先全量搜索调用点，统一改为 `VideoOsdState`，并补最小编译验证。
- **风险:** RK OSD 仍只有一条文本能力，统一状态模型与真实设备能力可能不一致。
- **缓解:** 明确区分“统一状态模型”与“底层已落地子集”；查询优先回协议状态，底层只尽力映射首条文本。
