# 任务清单: issue46 收口 GB 实时预览 codec 获取时机

## 1. 最小回归
- [√] 1.1 调整回归脚本，固定 `ProtocolManager::ReconfigureGbLiveSender()` 必须在 live preview start 路径读取 `CFG_VIDEO`，同时禁止 `LocalConfigProvider` 再承担该读取

## 2. 代码实现
- [√] 2.1 删除 `LocalConfigProvider.cpp` 中上一版 issue46 的 `CFG_VIDEO` 默认值注入逻辑
- [√] 2.2 在 `ProtocolManager.cpp` 中把 live preview codec 选择顺序改为“runtime getter -> live 场景实时读取 CFG_VIDEO -> 既有配置兜底”

## 3. 知识库与验证
- [√] 3.1 更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`
- [√] 3.2 执行回归、语法编译和工作区检查

## 执行结果

- 已把 `tools/tests/issue46_gb_codec_from_cfg_video_regression.py` 改成新的回归目标，并先在旧实现上跑出失败，再在修正后转绿。
- `ProtocolManager::ReconfigureGbLiveSender()` 现在会在每次实时预览建链时重新确定 codec：优先用运行态 codec，live 场景下 getter 不可用时实时读取 `CFG_VIDEO`，最后才回退到既有配置默认值。
- `LocalConfigProvider::InitDefaultLocalConfig()` 已恢复为普通默认值初始化，不再读取 `CFG_VIDEO`。
- fresh 验证已通过：`python3 tools/tests/issue46_gb_codec_from_cfg_video_regression.py`、`python3 tools/tests/issue45_gat_enabled_regression.py`、`python3 tools/tests/issue45_gat_online_status_regression.py`、`g++ -std=c++11 -fsyntax-only .../LocalConfigProvider.cpp -IApp -IInclude`、`git diff --check`。
