# 任务清单: OSD 统一状态模型与多文本协议兼容

目录: `helloagents/history/2026-03/202603301049_gb_osd_state_unify_multitext/`

## 1. 媒体侧统一 OSD 结构
- [√] 1.1 在 `rk_gb/App/Media/VideoOsdControl.h` 中删除 `VideoOsdConfig`，扩展 `VideoOsdState` 以承载多文本项与统一写入字段
- [√] 1.2 在 `rk_gb/App/Media/VideoOsdControl.cpp` 中将 `ApplyVideoOsdConfig` 改为基于 `VideoOsdState`，并补齐多文本状态缓存 / 首条文本落地逻辑

## 2. 协议侧切换到统一状态模型
- [√] 2.1 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中移除 `VideoOsdConfig` 中间转换逻辑，改为直接基于 `VideoOsdState` 做 merge / compare / apply
- [√] 2.2 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中补齐 GB `OSDConfig` 的多文本 `Item[]` 组包与解析映射

## 3. 外部配置与文档同步
- [-] 3.1 在 `rk_gb/App/Protocol/config/ProtocolExternalConfig.h` 与相关配置读写链路中补齐多文本配置模型或兼容转换，本次保留现有单文本外部配置 schema 作为兼容输入，不纳入统一运行态结构
- [√] 3.2 更新 `rk_gb/helloagents/wiki/modules/gb28181.md`、`rk_gb/helloagents/wiki/data.md`、`rk_gb/helloagents/CHANGELOG.md`，同步统一结构与多文本边界

## 4. 验证与收尾
- [√] 4.1 运行最小编译 / 语法级验证，确认 `VideoOsdConfig` 已无遗留引用
- [√] 4.2 根据实际执行结果更新方案包并迁移到 `history/`
