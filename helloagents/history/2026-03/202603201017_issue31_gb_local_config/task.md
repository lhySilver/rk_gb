# 任务清单

- [√] 梳理 issue31 对 GB28181 本地配置文件的字段与路径要求
- [√] 为 `GbRegisterParam` 增加 `enabled` 字段并补齐默认值
- [√] 在 `LocalConfigProvider` 中实现 `/userdata/conf/Config/gb28181.ini` 的读取、缺省生成与回写
- [√] 在 `ProtocolManager` 中接入 GB 开关，禁用时跳过启动与重注册
- [√] 同步兼容 `HttpConfigProvider` 的 `gb_register_enabled` 字段
- [√] 完成隔离构建验证，确认源码编译通过；当前阻塞为工程既有库缺失导致的链接失败
