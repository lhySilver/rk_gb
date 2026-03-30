# 技术设计: ProtocolManager 死包装方法瘦身

## 技术方案
- 从 `ProtocolManager.h` 删除：
  - `GBManager_Start()`
  - `GBManager_Stop()`
  - `OnGbConfigChanged()`
- 从 `ProtocolManager.cpp` 删除对应实现。
- 将 `ClearGbUpgradePendingState(protocol::ProtocolManager* manager, ...)` 收口为不带 `manager` 参数的静态 helper，并同步修改调用点。

## 实现要点
### 1. 删除未调用包装方法
- `GBManager_Start()` 只是打印一行日志再转调 `Start()`
- `GBManager_Stop()` 只是打印一行日志再转调 `Stop()`
- `OnGbConfigChanged()` 只是拉配置、校验、覆盖缓存后 `Stop()/Start()`
- 当前仓库内没有任何调用点，因此直接删除，避免继续把这些路径编进 `ProtocolManager.o`

### 2. 删除未使用参数
- `ClearGbUpgradePendingState()` 的 `manager` 参数没有任何使用点，属于纯冗余入参
- 删除该参数后同步修改唯一调用点，清掉编译 warning

## 验证方案
- 全仓搜索 `GBManager_Start|GBManager_Stop|OnGbConfigChanged`，确认已无活源码引用
- 用宿主 `g++ -std=c++11 -fsyntax-only` 检查 `ProtocolManager.cpp`
- 对比同编译宏下 `ProtocolManager.o` 的 `dec` 变化，确认继续缩小

## 验证结果
- 活源码搜索后，`GBManager_Start()`、`GBManager_Stop()`、`OnGbConfigChanged()` 已无剩余引用
- `ProtocolManager.cpp` 已通过最小语法检查
- `ProtocolManager.o` 的 `dec` 从 `236017` 收缩到 `234806`，继续减少 `1211` 字节
