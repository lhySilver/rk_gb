# 任务清单: 移除 HttpConfigProvider 死代码

目录: `helloagents/history/2026-03/202603301210_http_config_provider_removal/`

## 1. 删除未使用实现
- [√] 1.1 删除 `rk_gb/App/Protocol/config/HttpConfigProvider.h` 与 `rk_gb/App/Protocol/config/HttpConfigProvider.cpp`
- [√] 1.2 确认源码与当前构建入口不再引用 `HttpConfigProvider`

## 2. 同步知识库
- [√] 2.1 更新 `rk_gb/helloagents/wiki/arch.md`、`rk_gb/helloagents/wiki/api.md`、`rk_gb/helloagents/wiki/data.md`
- [√] 2.2 更新 `rk_gb/helloagents/wiki/modules/zero_config.md`、`rk_gb/helloagents/wiki/modules/gb28181-talk-review.md`、`rk_gb/helloagents/wiki/modules/terminal_requirements.md`

## 3. 验证与收尾
- [√] 3.1 搜索残留引用并对 `ProtocolManager.cpp`、`LocalConfigProvider.cpp` 做最小语法检查
- [√] 3.2 更新 `CHANGELOG.md`、`history/index.md` 并迁移方案包到 `history/`
