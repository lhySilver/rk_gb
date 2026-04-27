# GB 配置冗余收口

## 背景

同事反馈当前国标配置链路里有多余处理，需要继续压缩配置相关代码，保持嵌入式代码简单、直接、易审。

## 发现的问题

1. `App/Main.cpp::init_gb_zero_config()` 在启动阶段手动读取 `/userdata/conf/Config/GB/zero_config.ini`，再调用 `ProtocolManager::GetGbZeroConfig()` 读取同一文件做比较，必要时再 `SetGbZeroConfig()` 写回同一文件。这个逻辑与 `LocalConfigProvider` 的职责重复，正常情况下没有有效行为。
2. `LocalConfigProvider` 上午新增的 `WriteConfigHeader/WriteConfigInt/WriteConfigString` 只是包了一行 `fprintf`，反而让 INI 字段写入不如直接列字段清楚。
3. `RequiredZeroConfigFileError/IsZeroConfigFileRequired` 只是返回常量或转发已有谓词，`SaveLocalGbConfigFile()` 保存前也已完成 `register_mode` 归一化，后续再包一层/再归一化没有实际收益。
4. `InitDefaultLocalConfig()` 在已默认构造的 `ProtocolExternalConfig` 上重复写入大量结构默认值；`BuildDefaultGbRegisterParam()` / `BuildDefaultGatRegisterParam()` 也重复写入结构构造函数已有的默认值；GB 注册模式校验也会重复 trim/lower。
5. `UpdateGatRegisterConfig()` 先构造默认值并读取旧 `gat1400.ini`，随后又用新参数整体覆盖，旧配置读取没有实际效果。

## 目标

- 删除主程序重复 zero_config 导入。
- 保留 `LocalConfigProvider` 作为唯一配置文件读写边界。
- INI 保存函数恢复字段逐行显式输出，不改 Makefile/CMake，不新增编译单元。
- 去掉不提供额外语义的一行包装、重复默认赋值和重复归一化。
