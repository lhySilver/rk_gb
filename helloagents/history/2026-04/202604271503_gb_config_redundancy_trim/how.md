# 实施方案

## 改动范围

- `App/Main.cpp`
  - 删除 `init_gb_zero_config()`。
  - 删除启动路径里的调用。
  - 删除随之不再需要的 `Inifile.h` 和 `LocalConfigProvider.h` include。

- `App/Protocol/config/LocalConfigProvider.cpp`
  - 删除 `WriteConfigHeader/WriteConfigInt/WriteConfigString`。
  - 删除 `RequiredZeroConfigFileError/IsZeroConfigFileRequired/NormalizeGbRegisterConfig` 这类只包一层常量、谓词或 normalizer 的小函数。
  - `SaveLocalGbConfigFile()`、`SaveLocalGbZeroConfigFile()`、`SaveLocalGatConfigFile()` 恢复直接 `fprintf` 字段。
  - `SaveLocalGbConfigFile()` 使用已归一化后的 `register_mode`，不再保存时重复调用归一化。
  - `InitDefaultLocalConfig()` 先用 `ProtocolExternalConfig()` 恢复结构默认值，再只写本地 GB/GAT 差异项。
  - `BuildDefaultGbRegisterParam()` / `BuildDefaultGatRegisterParam()` 只写结构构造函数默认值以外的项目默认值。
  - `UpdateGatRegisterConfig()` 直接基于新参数归一化 `enabled` 后校验和保存，不再读旧 GAT 配置。
  - `UpdateGbZeroConfig()` 直接写入 `string_code/mac_address`，不再保留仅使用一次的两行字段拷贝 helper。
  - GB 注册模式校验先归一化一次，后续复用 `useZeroConfig`。
  - 保留 `FinishConfigFileWrite()` 统一处理 `fflush/fclose` 返回值，避免重复错误处理。

- `tools/tests/*`
  - 更新配置清理回归，明确 `Main.cpp` 不再重复读 `zero_config`。
  - 更新 GAT enabled 回归，适配显式 `fprintf` 写入。

## 不做项

- 不改 `gb28181.ini/zero_config.ini/gat1400.ini` 文件格式。
- 不改 `ProtocolManager` 对外配置接口语义。
- 不改 Makefile/CMake。

## 验证

- `python3 tools/tests/protocol_config_cleanup_regression.py`
- `python3 tools/tests/issue45_gat_enabled_regression.py`
- `g++ -std=c++11 -fsyntax-only` 检查 `App/Main.cpp`、`App/Protocol/config/LocalConfigProvider.cpp`
- `git diff --check`
