# 技术设计: issue46 收口 GB 实时预览 codec 获取时机

## 技术方案
1. 先把回归目标从 `LocalConfigProvider.cpp` 改到 `ProtocolManager.cpp`，明确 issue46 的正确落点是 live preview start 路径：
   - `ProtocolManager.cpp` 中必须存在 `CFG_VIDEO -> VideoConf_S` 读取链
   - 回归目标固定为 `ReconfigureGbLiveSender()`
   - `LocalConfigProvider.cpp` 中不应再承担 `CFG_VIDEO` 读取
2. 在 `ProtocolManager.cpp` 增加本地 helper：
   - 读取 `CFG_VIDEO`
   - 解析 `VideoConf_S`
   - 将 `enc_type=0/1` 映射为 `h264/h265`
3. 收口 `ReconfigureGbLiveSender()` 的 codec 选择顺序：
   - 优先读取 `media::QueryVideoEncodeStreamState()` 的运行态 codec
   - 若是 `kLiveStream` 且运行态 codec 不可用，则实时读取 `CFG_VIDEO`
   - 仍不可用时，再回退到现有 `gb_live` 配置的归一化值
4. 回退 `LocalConfigProvider::InitDefaultLocalConfig()` 中上一版 issue46 的 `CFG_VIDEO` 注入逻辑，只保留普通默认值初始化。
5. 同步更新 `helloagents/wiki`、`CHANGELOG.md`、`history/index.md`。

## 设计取舍
- **采用方案:** 在 `ReconfigureGbLiveSender()` 的 live 建链路径实时读取 `CFG_VIDEO` 做 fallback。
  - 优点: 与“开始预览时实时获取”的反馈一致，同时不破坏已有 runtime getter 优先级。
  - 缺点: live 预览路径会额外触发一次本地配置读取，但频率和影响面可控。
- **未采用方案:** 继续在 `LocalConfigProvider` 初始化默认值时读取 `CFG_VIDEO`。
  - 原因: 来源虽然对，但时机不对，无法满足最新评论要求的“开始预览时实时获取”。

## 验证计划
- 先运行 issue46 新回归脚本，确认旧实现失败。
- 实现后重新运行 issue46 回归至通过。
- 再执行 issue45 相关回归、`git diff --check` 和语法/编译校验。
