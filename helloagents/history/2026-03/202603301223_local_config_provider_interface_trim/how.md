# 技术设计: 删除 LocalConfigProvider 冗余抽象层

## 技术方案
- 删除 `IExternalConfigProvider.h`
- `LocalConfigProvider` 改为普通具体类，不再继承抽象接口
- `ProtocolManager::m_provider` 改为 `std::unique_ptr<LocalConfigProvider>`
- 删除未被活代码调用的方法：
  - `QueryCapabilities`
  - `SubscribeChange`
  - `SetMockConfig`

## 实现要点
### 1. 类型收口
- `ProtocolManager` 当前已经直接 `#include "LocalConfigProvider.h"`，因此把成员类型从接口改成具体类不会引入新的编译依赖。
- `m_provider` 当前没有共享所有权场景，因此进一步收口为 `unique_ptr`，顺便去掉不必要的引用计数代码。

### 2. 未使用方法清理
- 全仓搜索确认 `QueryCapabilities`、`SubscribeChange`、`SetMockConfig` 在活代码中没有调用点。
- 删除声明和定义，避免继续保留无效字符串拼装与日志代码。

## 验证方案
- 全仓搜索确认活源码不再引用 `IExternalConfigProvider`
- 用宿主 `g++ -std=c++11 -fsyntax-only` 检查：
  - `ProtocolManager.cpp`
  - `LocalConfigProvider.cpp`
- 对比同编译宏下的：
  - `LocalConfigProvider.o`
  - `ProtocolManager.o`
  确认这轮修改继续收缩目标文件体积；本次实测 `LocalConfigProvider.o` 的 `dec` 从 `76121` 降到 `72816`，`ProtocolManager.o` 的 `dec` 从 `236499` 降到 `236017`
