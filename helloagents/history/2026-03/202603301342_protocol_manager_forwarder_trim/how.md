# 技术设计: ProtocolManager 无调用转发方法瘦身

## 技术方案
- 从 `ProtocolManager.h/.cpp` 删除当前树内零调用的 3 个公开转发方法：
  - `PushListenAudioFrame()`
  - `ApplyGbBroadcastSdpOffer()`
  - `SetGbBroadcastPcmCallback()`
- 从 `ProtocolManager.cpp` 删除重复的前置声明：
  - `static std::string TrimWhitespaceCopy(const std::string& text);`
  - `static std::string NormalizeGbOsdTextTemplate(const std::string& textIn);`

## 实现要点
### 1. 删除零调用公开转发方法
- `PushListenAudioFrame()` 只做 `m_listen.PushAudioFrame(...)` 转发。
- `ApplyGbBroadcastSdpOffer()` 只做 `m_broadcast.ApplySdpOffer(...)` 转发。
- `SetGbBroadcastPcmCallback()` 只做 `m_broadcast.SetPcmFrameCallback(...)` 转发。
- 当前仓库内这 3 个方法均仅剩声明和定义，没有任何调用点。

### 2. 删除重复前置声明
- `TrimWhitespaceCopy()` 与 `NormalizeGbOsdTextTemplate()` 都已在声明前给出完整定义。
- 这两行前置声明没有提供额外语义，属于历史拼接残留。

## 验证方案
- 全仓搜索上述 3 个方法名，确认删除后无悬空引用。
- 全仓搜索两处 helper 名称，确认只保留真实定义和调用。
- 尽量对 `ProtocolManager.cpp` 做最小语法检查；如本地单文件检查缺少工程头路径，则在结果中明确说明。

## 验证结果
- `PushListenAudioFrame()`、`ApplyGbBroadcastSdpOffer()`、`SetGbBroadcastPcmCallback()` 在当前源码树内已无活引用
- `TrimWhitespaceCopy()` 与 `NormalizeGbOsdTextTemplate()` 的重复前置声明已删除，只保留实际定义
- 本地尝试执行 `ProtocolManager.cpp` 的最小语法检查时，因缺少 `libavcodec/avcodec.h` 头文件而中止，已记录为环境阻塞
