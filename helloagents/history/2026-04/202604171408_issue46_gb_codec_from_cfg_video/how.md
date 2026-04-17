# 技术设计: issue46 收口 GB 默认 codec 来源

## 技术方案
1. 在 `App/Protocol/config/LocalConfigProvider.cpp` 增加本地 helper：
   - 读取 `CFG_VIDEO`
   - 解析 `VideoConf_S`
   - 将 `enc_type=0/1` 分别映射为 `h264/h265` 与 `H.264/H.265`
2. `InitDefaultLocalConfig()` 里不再直接写死三处 codec 字符串，而是先给出兼容默认值，再调用 helper 用 `CFG_VIDEO` 覆盖：
   - `cfg.gb_live.video_codec` 取主码流
   - `cfg.gb_video.main_codec` 取主码流
   - `cfg.gb_video.sub_codec` 取辅码流
3. 新增文本级回归脚本，校验：
   - 文件中存在 `CFG_VIDEO` 读取链
   - 文件中存在 `TExchangeAL<VideoConf_S>::getConfig(...)`
   - 三处 GB codec 不再直接赋值为固定 `h265/H.265`
4. 同步更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`。

## 设计取舍
- **采用方案:** 在 `LocalConfigProvider` 读取 `CFG_VIDEO` 做兜底覆盖。
  - 优点: 与 issue 指定来源一致，改动最小，不影响现有运行态查询优先级。
  - 缺点: 仍依赖本地配置和真实编码器保持一致。
- **未采用方案:** 把默认值继续固定成 `H.265`，仅依赖 `ProtocolManager` 运行态 getter 覆盖。
  - 原因: 与 issue 要求不符，而且 getter 失败时仍会回到错误兜底值。

## 验证计划
- 先运行 issue46 新回归脚本，确认旧代码失败。
- 实现后重新运行 issue46 回归至通过。
- 再执行 issue45 相关回归、`git diff --check` 和 `LocalConfigProvider.cpp` 语法编译。
