# 变更提案: 移除未使用的 HttpConfigProvider

## 需求背景
当前协议配置链路已经收口到 `LocalConfigProvider`：
- `ProtocolManager` 初始化时只实例化 `LocalConfigProvider`
- `App/CMakeLists.txt` 当前也只编译 `Protocol/config/LocalConfigProvider.cpp`

但仓库里仍保留 `HttpConfigProvider.h/.cpp` 以及多处把它当现役组件的知识库描述。这会带来两个问题：
- 代码体积和仓库噪音继续保留一整套已经不用的配置提供者实现
- 知识库会误导后续开发者，以为当前仍存在“HTTP 拉取协议配置”的正式链路

## 变更目标
1. 删除未被源码和构建入口使用的 `HttpConfigProvider` 类实现。
2. 保持当前协议配置行为不变，继续只走 `LocalConfigProvider`。
3. 同步更新知识库，明确 HTTP 配置提供者已移除，避免继续误导。

## 影响范围
- `App/Protocol/config/HttpConfigProvider.h`
- `App/Protocol/config/HttpConfigProvider.cpp`
- `helloagents/wiki/arch.md`
- `helloagents/wiki/api.md`
- `helloagents/wiki/data.md`
- `helloagents/wiki/modules/zero_config.md`
- `helloagents/wiki/modules/gb28181-talk-review.md`
- `helloagents/wiki/modules/terminal_requirements.md`

## 风险评估
- 风险: 旧构建目录 `cmake-build/` 仍保留对 `HttpConfigProvider.cpp` 的历史生成引用。
- 缓解: 以源码和当前 `App/CMakeLists.txt` 为准完成删除；不修改生成产物，并在验证中以“源码零引用”作为主检查项。
