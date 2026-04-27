# 变更提案: 协议配置层清晰化整理

## 需求背景
当前 GB28181、零配置和 GAT1400 的本地配置已经收口到 `LocalConfigProvider`，但该文件同时承担默认值、INI 读写、字段归一化、校验、运行态派生和日志摘要等职责，阅读路径偏长。用户明确要求在嵌入式代码中保持“越简单，越清晰，越好”，并尽量不改动 Makefile。

本次优化选择文件内小步整理，不新增源码编译单元，不改变既有配置文件格式和生命周期语义。

## 变更内容
1. 将 `LocalConfigProvider.cpp` 中 GB、零配置、GAT 的默认值、读取、保存、校验边界整理成更清晰的小段落和小 helper。
2. 保持 `/userdata/conf/Config/GB/gb28181.ini`、`zero_config.ini`、`gat1400.ini` 的字段集合、错误码和默认行为不变。
3. 只在必要时补充非常小的常量或辅助函数，避免引入通用配置框架。
4. 补充轻量回归脚本，约束配置路径、字段、旧路径误用和默认值单源化。
5. 同步更新协议配置知识库，记录新的代码边界。

## 影响范围
- **模块:** Protocol 配置层、GB28181 注册配置、零配置入口配置、GAT1400 注册配置
- **文件:**
  - `App/Protocol/config/LocalConfigProvider.cpp`
  - `App/Protocol/config/LocalConfigProvider.h`
  - `App/Protocol/config/ProtocolExternalConfig.h`
  - `tools/tests/*protocol_config*`
  - `helloagents/wiki/modules/protocol.md`
  - `helloagents/wiki/modules/zero_config.md`
  - `helloagents/wiki/modules/gat1400.md`
  - `helloagents/wiki/data.md`
  - `helloagents/CHANGELOG.md`
- **API:** 不新增对外接口；保持 `Get/Set/Restart*Config` 现有语义
- **数据:** 不新增 INI 字段；不迁移旧路径；不改变 flash 文件格式

## 核心场景

### 需求: 配置层可读性提升
**模块:** `LocalConfigProvider`
把 GB、零配置、GAT 的本地配置流程整理为“默认值 -> 读取 -> 校验 -> 保存 -> 同步”的直线式阅读路径。

#### 场景: 维护人员查看 GB 注册配置
当维护人员定位 `gb28181.ini` 的字段读写时：
- 可以在同一段落看到相关字段、默认值和校验。
- 不需要跳过大量 GAT 或零配置逻辑才能理解 GB 配置链路。

### 需求: 嵌入式低风险约束
**模块:** `Protocol/config`
保持现有编译单元和构建脚本不变。

#### 场景: 交叉编译环境缺失或构建目录绑定旧路径
当本地完整构建不可用时：
- 仍可通过脚本回归和 `g++ -fsyntax-only` 做最小验证。
- 不因为新增 `.cpp` 引入 Makefile/CMake 维护成本。

### 需求: 配置行为保持兼容
**模块:** GB28181 / ZeroConfig / GAT1400
保持现有字段和运行态语义。

#### 场景: 设备已有配置文件
当设备已有 `gb28181.ini`、`zero_config.ini`、`gat1400.ini` 时：
- 读取、保存和重启协议生命周期的行为不应改变。
- `register_mode=zero_config` 仍只在运行态使用代码内置重定向入口，不污染标准 GB 配置。

## 风险评估
- **风险:** 整理配置层时误改字段集合或错误码，导致现有 Web 配置、注册重载或平台联调行为变化。
- **缓解:** 不做跨文件大重构；增加文本级回归脚本约束关键字段、路径、默认值和旧路径误用。
- **风险:** 新增源码文件会牵涉构建系统。
- **缓解:** 本轮不新增源码编译单元，不改 Makefile/CMake。
