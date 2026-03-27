# 技术设计: GB 运行时注册模式切换

## 实现策略
1. 在 `GbRegisterParam` 中新增 `register_mode`，默认值设为 `standard`，并提供统一的运行时判定辅助函数。
2. 在 `LocalConfigProvider` 中把 `register_mode` 落到 `gb28181.ini`，`zero_config.ini` 保持只保存零配置扩展字段：
   - `standard` 模式下忽略 `zero_config.ini` 缺失
   - `zero_config` 模式下缺少 `zero_config.ini` 或关键字段时直接返回错误
3. 在 `HttpConfigProvider` 中新增 `gb_register_mode` 的默认值、序列化、反序列化和校验。
4. 在 `ProtocolManager` 与 `GBClientImpl` 中移除 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 的运行时分支，改为依据 `register_mode` 组装标准注册或零配置注册参数。
5. 同步移除构建脚本中对 `PROTOCOL_ENABLE_GB_ZERO_CONFIG` 的依赖说明，并更新知识库。

## 风险控制
- `register_mode` 只放在 `gb28181.ini`，避免把模式控制放进 `zero_config.ini` 导致加载顺序反转。
- `zero_config.ini` 仍然只在 `register_mode=zero_config` 时必填，避免对标准国标路径引入额外文件依赖。
- 通过 `g++ -fsyntax-only` 分别校验 `LocalConfigProvider.cpp`、`HttpConfigProvider.cpp`、`ProtocolManager.cpp`、`GBClientImpl.cpp`，降低跨模块改动引入的编译风险。
