# 任务清单

- [√] 1.1 定位 `Main.cpp::init_gb_zero_config()` 重复读取/写回 zero_config 的问题
- [√] 1.2 定位 `WriteConfigHeader/WriteConfigInt/WriteConfigString` 一行包装导致的过度抽象
- [√] 2.1 先更新回归检查并确认当前代码失败
- [√] 3.1 删除 `init_gb_zero_config()` 和启动调用
- [√] 3.2 删除一行 `WriteConfig*` 包装，恢复 INI 字段显式写入
- [√] 3.3 删除零配置错误码/模式判断的一行包装，并去掉保存前重复归一化
- [√] 3.4 收缩默认配置初始化，只覆盖结构默认值以外的差异项
- [√] 3.5 收缩 GB/GAT 默认参数构建函数，只写本地项目默认差异项
- [√] 3.6 删除 GAT 更新中无效的旧配置读取
- [√] 3.7 GB 注册模式校验改为一次归一化后复用
- [√] 4.1 更新知识库和变更记录
- [√] 5.1 运行回归、语法和 diff 校验

## 验证记录

- `python3 tools/tests/protocol_config_cleanup_regression.py` 通过
- `python3 tools/tests/issue45_gat_enabled_regression.py` 通过
- `python3 tools/tests/protocol_config_defaults_single_source_regression.py` 通过
- `python3 tools/tests/external_module_demos_doc_regression.py` 通过
- `g++ -std=c++11 -fpermissive -fsyntax-only ... App/Protocol/config/LocalConfigProvider.cpp` 通过
- `g++ -std=c++11 -fpermissive -DIPC_APP_VERSION=\"syntax\" -fsyntax-only ... App/Main.cpp` 通过；仅保留仓库既有 `GB_ID_LEN` 宏重定义和 `snprintf` format-security warning
- `git diff --check` 通过
