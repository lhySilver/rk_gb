# 变更提案: 收口 LocalConfigProvider 的空抽象层

## 需求背景
在移除 `HttpConfigProvider` 之后，协议配置提供者只剩 `LocalConfigProvider` 一个实现，但代码里仍保留：
- `IExternalConfigProvider` 抽象接口
- `ProtocolManager::m_provider` 对抽象接口的持有
- `LocalConfigProvider` 中未被活代码调用的 `QueryCapabilities`、`SubscribeChange`、`SetMockConfig`

这类“单实现 + 空抽象层 + 未调用方法”不会改变行为，却会继续给 `dgiot` 带来额外代码体积和认知负担。

## 变更目标
1. 删除只剩单实现的 `IExternalConfigProvider`。
2. 让 `ProtocolManager` 直接持有 `LocalConfigProvider`。
3. 删除 `LocalConfigProvider` 中活代码未调用的方法，继续做无行为变化的瘦身。

## 影响范围
- `rk_gb/App/Protocol/config/IExternalConfigProvider.h`
- `rk_gb/App/Protocol/config/LocalConfigProvider.h`
- `rk_gb/App/Protocol/config/LocalConfigProvider.cpp`
- `rk_gb/App/Protocol/ProtocolManager.h`

## 风险评估
- 风险: 旧构建缓存仍会记录 `IExternalConfigProvider.h` 的历史依赖。
- 缓解: 以源码和最小语法检查为准验证；不改 `cmake-build/` 生成文件。
