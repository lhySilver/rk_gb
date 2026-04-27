# 技术设计: 协议配置层清晰化整理

## 技术方案

### 核心技术
- C++11
- 现有 `CInifile` 读接口
- 现有 `FILE*` 文本写入方式
- 现有 Python 文本回归脚本

### 实现要点
- 保持 `LocalConfigProvider.cpp` 单编译单元，不新增 `.cpp`，避免 Makefile/CMake 改动。
- 在匿名命名空间内按职责整理 helper：
  - 路径、错误码、同步状态
  - 默认配置构造
  - 字符串/数字读取
  - GB/Zero/GAT 分组读写
  - GB/Zero/GAT 分组校验
  - 同步与日志
- 尽量只移动和改名，不改业务条件；确有明显不一致时单独处理并用测试覆盖。
- 保持 `PushApply()`、`PullLatest()`、`LoadOrCreate*()`、`Update*()` 对外行为不变。
- 对 `App/Main.cpp` 的旧 `/userdata/zero_config.ini` 问题只做风险记录；若实施中确认必须修正，限定为调用 `ProtocolManager::SetGbZeroConfig()` 前的入口兼容整理，不扩大到 Web 页面结构。

## 架构设计
不做架构拆分。配置链路仍为：

```text
gb28181.ini / zero_config.ini / gat1400.ini
  -> LocalConfigProvider
  -> ProtocolManager Get/Set/Restart
  -> GB28181 / GAT1400 lifecycle
```

## 架构决策 ADR

### ADR-20260427-01: 配置层采用文件内整理，不拆新编译单元
**上下文:** 用户要求嵌入式代码简单清晰，并尽量不改动 Makefile。
**决策:** 本轮只在 `LocalConfigProvider.cpp/.h` 与必要头文件内整理，不新增 `GbConfigStore.cpp`、`GatConfigStore.cpp` 等新文件。
**理由:** 文件内整理能降低构建系统风险，也更符合当前轻量回归测试和交叉编译环境约束。
**替代方案:** 拆分为多个配置源文件 -> 拒绝原因: 需要维护构建脚本，且当前配置规模还不值得引入新的编译单元。
**影响:** 单文件仍然较长，但阅读路径会更直，后续如确需拆分可基于本次分组继续演进。

### ADR-20260427-02: 不引入通用 INI 字段描述器
**上下文:** GB、零配置、GAT 字段数量有限，且存在不同的校验和运行态派生语义。
**决策:** 保留显式读写代码，只抽取小 helper，不做表驱动配置框架。
**理由:** 显式代码对嵌入式联调更容易排查，避免宏表、回调和泛型结构增加理解成本。
**替代方案:** 使用字段表统一读写 -> 拒绝原因: 通用性提升有限，调试复杂度上升。
**影响:** 少量重复代码保留，但每个字段的落盘行为更直观。

## API设计
不新增对外 API。

## 数据模型
不新增或删除 INI 字段。

当前保持：
- `gb28181.ini`: `enable`、`register_mode`、`username`、`server_ip`、`server_port`、`device_id`、`password`
- `zero_config.ini`: `string_code`、`mac_address`
- `gat1400.ini`: `enable`、`scheme`、`server_ip`、`server_port`、`base_path`、`device_id`、`username`、`password`、`auth_method`、`listen_port`、`expires_sec`、`keepalive_interval_sec`、`max_retry`、`request_timeout_ms`、`retry_backoff_policy`

## 安全与性能
- **安全:** 不新增网络访问、权限变更或敏感信息输出；保持现有明文配置行为不扩大。
- **性能:** 配置读写仍为低频 flash 文件操作；不引入动态框架或额外线程。

## 测试与部署
- **测试:**
  - 运行现有配置回归脚本。
  - 新增或扩展配置清晰化回归脚本，检查路径、字段、默认值和旧路径误用。
  - 对 `LocalConfigProvider.cpp` 做 `g++ -std=c++11 -fsyntax-only` 语法级验证。
  - 执行 `git diff --check`。
- **部署:** 不涉及部署步骤；不改 Makefile/CMake。
