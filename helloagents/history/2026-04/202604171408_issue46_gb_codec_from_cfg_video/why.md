# 变更提案: issue46 收口 GB 实时预览 codec 获取时机

## 需求背景
issue 46 的最新评论明确要求：GB28181 预览 codec 不只是“来源改成 `CFG_VIDEO`”，而是“应该在开始预览的时候，实时获取”。指定来源仍然是本地 `CFG_VIDEO` 配置：
- `VideoConf_S LocalVideoConfig;`
- `g_configManager.getConfig(getConfigName(CFG_VIDEO), table);`

上一版实现虽然把 codec 来源改到了 `CFG_VIDEO`，但落点放在 `LocalConfigProvider::InitDefaultLocalConfig()`，本质上仍是在协议默认配置初始化阶段提前固化 codec，而不是在每次预览建链时实时决定。这样当预览启动时编码运行态 getter 暂时不可用，live sender 仍可能拿到过早缓存的默认值。

## 变更目标
1. 把 `CFG_VIDEO` 的 codec fallback 收口到 GB 实时预览开始建链路径，而不是默认配置初始化阶段。
2. 保持现有 `QueryVideoEncodeStreamState()` 的运行态查询优先级不变，只在 live 预览场景下补实时 `CFG_VIDEO` 兜底。
3. 补一条最小回归，防止后续再次把 issue46 的读取时机改回初始化阶段。

## 非目标
- 不把 `CFG_VIDEO` 读取重新放回 `LocalConfigProvider::InitDefaultLocalConfig()`。
- 不改变 `gb_video.main_codec/sub_codec` 这些静态默认值的既有语义。
- 不扩展到回放、下载、Web 页面、GAT1400 或其他协议模块的 codec 配置逻辑。
