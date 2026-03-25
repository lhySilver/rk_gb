# 变更提案: issue38 国标配置文件管理优化

## 背景
- 当前 `ProtocolManager::GetGbRegisterConfig()` / `SetGbRegisterConfig()` 只是直接读写 `m_cfg.gb_register`。
- 这组接口没有接入 `/userdata/conf/Config/gb28181.ini`，因此不满足 issue38 对“配置文件缺省生成、init 前后都可调用、修改实时生效”的要求。
- 外部模块实际只会修改国标注册相关的 6 个字段，不应该被整套协议配置模型和运行态细节绑死。

## 目标
- 把 `GetGbRegisterConfig` / `SetGbRegisterConfig` 收敛为真正的“国标注册配置文件接口”。
- 保证配置文件缺失时自动按默认值生成，并且在 `Init()` 前就可读写。
- 保证运行中通过 `SetGbRegisterConfig()` 修改国标注册配置后，GB 注册生命周期能立即按新配置生效。

## 范围
- `App/Protocol/ProtocolManager.*`
- `App/Protocol/config/LocalConfigProvider.*`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/CHANGELOG.md`

## 非目标
- 不重构当前整套 `ProtocolExternalConfig` / `HttpConfigProvider` 远程配置模型。
- 不改变 issue38 之外的 GB/GAT 配置字段语义。
