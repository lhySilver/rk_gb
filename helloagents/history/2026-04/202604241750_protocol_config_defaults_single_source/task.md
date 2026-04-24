# 协议配置默认版本与 GB 设备信息单源化

## 任务类型
- 轻量迭代 / 可维护性修复

## 背景
- `ProtocolExternalConfig::version` 默认值在构造函数、默认初始化、配置文件同步刷新等多处固定写死，改版本需要同时改多处。
- `GbRegisterParam::manufacturer/model` 也同时存在于结构默认构造和本地默认配置 helper 中，后续改厂商/型号容易漏改。

## 任务清单
- [√] 定位 `version`、`manufacturer`、`model` 默认值写入点。
- [√] 新增静态回归测试，约束默认版本、厂商、型号只在常量定义处保留字面值。
- [√] 在 `ProtocolExternalConfig.h` 中集中定义默认版本、自定义协议版本、厂商和型号常量。
- [√] 将 `GbRegisterParam`、`ProtocolExternalConfig` 构造函数和 `LocalConfigProvider` 默认/同步刷新逻辑改为引用常量。
- [√] 运行配置回归测试与语法检查。

## 验证
- `python3 tools/tests/protocol_config_defaults_single_source_regression.py`
- `python3 tools/tests/issue45_gat_enabled_regression.py`
- `python3 tools/tests/issue45_gat_online_status_regression.py`
- `g++ -std=c++11 -fsyntax-only -IApp -IApp/Protocol/config App/Protocol/config/LocalConfigProvider.cpp`

## 说明
- `make -C cmake-build dgiot -j4` 受现有构建目录内旧绝对路径 `/opt/cmake/cmake-3.16.3-Linux-x86_64/bin/cmake` 缺失影响，未作为本轮最终验证依据。
