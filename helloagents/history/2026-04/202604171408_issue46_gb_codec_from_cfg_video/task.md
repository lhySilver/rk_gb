# 任务清单: issue46 收口 GB 默认 codec 来源

## 1. 最小回归
- [√] 1.1 新增回归脚本，先固定 `LocalConfigProvider` 必须读取 `CFG_VIDEO` 且不能继续把 GB codec 写死为 `H.265`

## 2. 代码实现
- [√] 2.1 在 `LocalConfigProvider.cpp` 增加 `CFG_VIDEO -> VideoConf_S` 读取 helper
- [√] 2.2 将 `gb_live.video_codec`、`gb_video.main_codec`、`gb_video.sub_codec` 改为按主辅码流 `enc_type` 映射

## 3. 知识库与验证
- [√] 3.1 更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`
- [√] 3.2 执行回归、语法编译和工作区检查

## 执行结果

- 已新增 `tools/tests/issue46_gb_codec_from_cfg_video_regression.py`，并先在旧代码状态下跑出失败，再在实现后转绿。
- `LocalConfigProvider::InitDefaultLocalConfig()` 现在会读取 `CFG_VIDEO`，将主辅码流 `enc_type` 映射为 GB live/main/sub 的 codec 兜底值，不再固定写死为 `H.265`。
- fresh 验证已通过：`python3 tools/tests/issue46_gb_codec_from_cfg_video_regression.py`、`python3 tools/tests/issue45_gat_enabled_regression.py`、`python3 tools/tests/issue45_gat_online_status_regression.py`、`g++ -std=c++11 -fsyntax-only .../LocalConfigProvider.cpp -IApp -IInclude`、`git diff --check`。
