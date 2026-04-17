# 变更提案: issue46 收口 GB 默认 codec 来源

## 需求背景
issue 46 要求把国标里预览码流编码方式 `H.264/H.265` 的固定写死值改成从其他模块读取，指定来源为本地 `CFG_VIDEO` 配置：
- `VideoConf_S LocalVideoConfig;`
- `g_configManager.getConfig(getConfigName(CFG_VIDEO), table);`

当前 `LocalConfigProvider::InitDefaultLocalConfig()` 会把 `gb_live.video_codec`、`gb_video.main_codec`、`gb_video.sub_codec` 默认写成 `h265/H.265`。这会让 GB 配置兜底值在编码运行态 getter 暂时不可用时，仍可能和真实主辅码流编码不一致。

## 变更目标
1. 让 GB live/main/sub 的默认 codec 兜底值跟随 `CFG_VIDEO` 中的主辅码流 `enc_type`。
2. 保持现有 GB 分辨率、码率、帧率默认值不变，只收口 codec 来源。
3. 补一条最小回归，防止后续再次把这三处 codec 改回硬编码字符串。

## 非目标
- 不修改 `ProtocolManager` 已有的运行态媒体查询链路。
- 不改动 `CFG_VIDEO` 默认值本身。
- 不扩展到 Web 页面、GAT1400 或其他协议模块的 codec 配置逻辑。
