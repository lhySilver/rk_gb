# 技术设计: GB ConfigControl 重复日志收口

## 技术方案
- 在 `ProtocolManager.cpp` 新增两个局部 helper：
  - `LogGbImageFlipApplyDecision(...)`
  - `LogGbVideoApplyDecision(...)`
- `HandleGbConfigControl()` 中原先两处重复的 `printf` 展开改为调用 helper。
- helper 内部分别保留原先的两套日志格式：
  - `skip_apply` 不带 `ret`
  - `dispatch` 保留 `ret=%d`

## 实现要点
### 1. image flip 日志
- 收口：
  - `gb config image_flip skip_apply ...`
  - `gb config image_flip dispatch ret=...`
- 保持 `current/value/gb` 三个字段来源不变。

### 2. video_param_attribute 日志
- 收口：
  - `gb config video_param_attribute skip_apply ...`
  - `gb config video_param_attribute dispatch ret=...`
- 保持当前码流运行态字段与目标值字段完全不变。

## 验证方案
- 使用与 `cmake-build/App/CMakeFiles/dgiot.dir/flags.make` 一致的关键编译宏：
  - `BLE_AIC8800DL`
  - `PROTOCOL_HAS_GB28181_CLIENT_SDK=1`
  - `RC0240_LGV10`
- 对“修改前快照”和“当前文件”分别执行 `g++ -std=c++11 -O3 -c`
- 使用 `size` 对比两个对象文件的 `dec` 结果，确认继续收缩

## 预期结果
- 行为不变、日志文本不变
- `ProtocolManager.o` 继续减小
