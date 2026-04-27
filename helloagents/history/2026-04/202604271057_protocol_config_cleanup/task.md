# 任务清单: 协议配置层清晰化整理

目录: `helloagents/plan/202604271057_protocol_config_cleanup/`

---

## 1. 配置层边界整理
- [√] 1.1 在 `App/Protocol/config/LocalConfigProvider.cpp` 中按“路径/错误码、默认值、读写、校验、同步、公开方法”整理匿名命名空间 helper，验证 why.md#需求-配置层可读性提升-场景-维护人员查看-gb-注册配置
- [√] 1.2 在 `App/Protocol/config/LocalConfigProvider.cpp` 中保持 GB/Zero/GAT 字段读写显式化，必要时补充小 helper 去掉低价值重复，验证 why.md#需求-嵌入式低风险约束-场景-交叉编译环境缺失或构建目录绑定旧路径
- [√] 1.3 在 `App/Protocol/config/LocalConfigProvider.h` 与 `App/Protocol/config/ProtocolExternalConfig.h` 中只做必要的声明或常量调整，不新增对外 API，验证 why.md#需求-配置行为保持兼容-场景-设备已有配置文件

## 2. 行为兼容检查
- [√] 2.1 复核 `LoadOrCreateGbRegisterConfig()`、`LoadOrCreateGbRuntimeRegisterConfig()`、`UpdateGbRegisterConfig()`、`UpdateGbZeroConfig()` 行为不变，验证 zero_config 缺失与 `register_mode` 条件
- [√] 2.2 复核 `LoadOrCreateGatRegisterConfig()`、`UpdateGatRegisterConfig()` 行为不变，验证 `enabled=0` 时仍允许保存停服态配置
- [√] 2.3 复核 `PushApply()` / `PullLatest()` 仍保持当前 flash 同步和运行态派生语义，不新增旧路径迁移

## 3. 回归测试
- [√] 3.1 新增或扩展 `tools/tests/protocol_config_*_regression.py`，约束三份 INI 的路径、字段集合、默认值单源化和旧 `/userdata/zero_config.ini` 风险点
- [√] 3.2 运行 `python3 tools/tests/protocol_config_defaults_single_source_regression.py`
- [√] 3.3 运行 `python3 tools/tests/issue45_gat_enabled_regression.py`
- [√] 3.4 运行新增或扩展后的配置回归脚本
- [√] 3.5 对 `App/Protocol/config/LocalConfigProvider.cpp` 执行语法级验证

## 4. 安全检查
- [√] 4.1 检查本次改动未新增敏感信息日志、危险系统命令、外部服务访问或权限变更
- [√] 4.2 执行 `git diff --check`

## 5. 文档同步
- [√] 5.1 更新 `helloagents/wiki/modules/protocol.md`，记录 `LocalConfigProvider` 新的文件内职责分组
- [√] 5.2 按实际改动更新 `helloagents/wiki/modules/zero_config.md`、`helloagents/wiki/modules/gat1400.md`、`helloagents/wiki/data.md`
- [√] 5.3 更新 `helloagents/CHANGELOG.md`
- [√] 5.4 更新本任务状态并在完成后迁移方案包到 `helloagents/history/2026-04/`

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
