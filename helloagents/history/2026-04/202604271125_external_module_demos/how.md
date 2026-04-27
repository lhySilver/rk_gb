# 技术设计: 外部模块接入 Demo 文档

## 技术方案
- 新增 `helloagents/wiki/modules/external_module_demos.md`，作为外部模块接入示例入口。
- 在 `helloagents/wiki/api.md`、`helloagents/wiki/modules/protocol.md`、`helloagents/wiki/modules/gat1400.md`、`helloagents/wiki/modules/zero_config.md`、`helloagents/wiki/overview.md` 增加轻量链接。
- 新增 `tools/tests/external_module_demos_doc_regression.py` 做文本约束。

## 示例范围
- GB 标准平台注册参数写入与重启。
- GB 零配置 `string_code/mac_address` 写入与重启。
- GAT1400 注册参数、启停开关和重启。
- 在线状态查询与 GAT1400 人脸/机动车/非机动车结构化对象上报。

## 安全与性能
- 仅文档和文本回归脚本，无运行时代码。
- 不引入新编译单元，不改构建脚本。
- 示例中的平台地址、设备编码和密码使用占位值，避免误导为真实生产配置。

## 验证
- `python3 tools/tests/external_module_demos_doc_regression.py`
- `git diff --check`
