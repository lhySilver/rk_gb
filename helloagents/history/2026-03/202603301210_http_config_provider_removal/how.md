# 技术设计: 删除 HttpConfigProvider 死代码并同步知识库

## 技术方案
### 核心技术
- 直接删除 `HttpConfigProvider.h/.cpp`
- 保留 `IExternalConfigProvider`，因为 `LocalConfigProvider` 与 `ProtocolManager::m_provider` 仍在使用
- 更新活跃 wiki，使其从“HTTP + 本地双提供者”改为“当前仅本地配置提供者”

## 实现要点
### 1. 代码删除
- 删除 `App/Protocol/config/HttpConfigProvider.h`
- 删除 `App/Protocol/config/HttpConfigProvider.cpp`
- 保持其余源码不动；如果全局搜索确认没有引用，就不做额外适配

### 2. 知识库同步
- `arch.md`
  - 从总体架构图和核心流程中移除 `HttpConfigProvider`
  - 将架构约束改为“协议配置当前仅依赖本地配置提供者”
- `api.md`
  - 移除 `GET/POST /openapi/v1/ipc/protocol/config*` 这组已无代码承接的配置服务描述
- `data.md`
  - 将配置来源改为 `LocalConfigProvider -> ProtocolExternalConfig -> ProtocolManager / GAT1400ClientService`
- `zero_config.md`、`gb28181-talk-review.md`、`terminal_requirements.md`
  - 删除“HttpConfigProvider 已支持”的表述，改成当前代码事实

## 验证方案
- 全仓搜索 `HttpConfigProvider`，确认源码层只剩归档历史和构建缓存引用，不再有活源码/活 wiki 依赖
- 用宿主 `g++ -fsyntax-only` 对 `ProtocolManager.cpp`、`LocalConfigProvider.cpp` 做最小语法检查，确认删除死文件后现有编译单元无悬空头文件依赖

## 预期结果
- 协议配置行为不变
- 仓库删除两份未使用实现文件
- 活跃知识库与当前代码现状一致
