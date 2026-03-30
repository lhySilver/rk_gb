# 任务清单: 收口 LocalConfigProvider 冗余抽象层

目录: `helloagents/history/2026-03/202603301223_local_config_provider_interface_trim/`

## 1. 删除空抽象层与未使用方法
- [√] 1.1 删除 `rk_gb/App/Protocol/config/IExternalConfigProvider.h`，并将 `LocalConfigProvider` 改为具体类
- [√] 1.2 将 `rk_gb/App/Protocol/ProtocolManager.h` 中 `m_provider` 收口为 `std::unique_ptr<LocalConfigProvider>`
- [√] 1.3 删除 `LocalConfigProvider` 中未被活代码调用的 `QueryCapabilities`、`SubscribeChange`、`SetMockConfig`，并顺手清理只服务这些方法的 `JoinConfigList`

## 2. 验证与收尾
- [√] 2.1 运行源码搜索和最小语法检查，确认删除后无悬空依赖
- [√] 2.2 对比 `LocalConfigProvider.o` / `ProtocolManager.o` 体积变化，确认继续瘦身；`dec` 分别从 `76121 -> 72816`、`236499 -> 236017`
- [√] 2.3 更新 `CHANGELOG.md`、`history/index.md` 并迁移方案包到 `history/`
