# GB Register Time Sync TODO

## 背景

GB28181 `REGISTER` 成功后，平台会在 `200 OK` 里返回 `Date` 头。

当前代码已经在注册成功后读取该字段，并进入 `ProtocolManager::PrepareGbRegisterTimeSync()`。
现阶段只做日志和接口预留，不实际修改系统时间，避免影响现网调试和本机其他模块。

## 当前状态

- 已读取 `REGISTER 200 OK` 的 `Date` 头
- 已增加时间同步准备入口：
  - `ProtocolManager::QueryGbRegisterDate(std::string& outDate) const`
  - `ProtocolManager::PrepareGbRegisterTimeSync(const std::string& rawDate)`
- 已增加嵌入式外部对接回调接口：
  - `ProtocolManager::SetGbTimeSyncHook(GbTimeSyncHook hook, void* userData)`
- 当前行为：
  - 解析 `Date` 字段
  - 打印 UTC、本地时间、epoch
  - 若未注册回调，仅打印 `hook not_set`
  - `dry_run=1`，不调用 `settimeofday`

## 已定义接口

代码位置：
- `App/Protocol/ProtocolManager.h`
- `App/Protocol/ProtocolManager.cpp`

### 回调数据结构

```cpp
struct GbTimeSyncInfo
{
    std::string raw_date;
    std::string utc_time;
    std::string local_time;
    int64_t epoch_sec;
    bool dry_run;
};
```

### 回调类型

```cpp
typedef int (*GbTimeSyncHook)(const GbTimeSyncInfo& info, void* userData);
```

### 注册接口

```cpp
void SetGbTimeSyncHook(GbTimeSyncHook hook, void* userData);
```

## 建议联调方式

由嵌入式时间管理模块实现一个 `GbTimeSyncHook`，在回调里完成以下动作：

1. 校验 `raw_date` 是否为平台认可格式
2. 根据 `epoch_sec` 决定是否允许更新时间
3. 调用底层 RTC / system time 设置接口
4. 将结果返回给 `ProtocolManager` 用于日志记录

## 返回值约定建议

- `0`: 接收并处理成功
- `-1`: 输入参数非法
- `-2`: 时间格式不支持
- `-3`: 底层时间模块未就绪
- `-4`: 设置系统时间失败
- `-5`: 设置 RTC 失败

## TODO

- [ ] 和平台确认 `REGISTER 200 OK` 的 `Date` 头最终格式
- [ ] 明确是否统一使用 RFC1123/GMT，还是允许 ISO8601
- [ ] 嵌入式时间模块实现 `GbTimeSyncHook`
- [ ] 确认是否同时更新 RTC 和系统时间
- [ ] 确认时间回拨/跳变的保护策略
- [ ] 增加校时成功/失败后的统一状态上报日志
- [ ] 在联调环境验证 GB 注册后系统时间是否按预期更新

## 现阶段注意事项

- 当前平台抓包里 `Date` 样例为：`2026-03-16T16:12:22.617`
- 这不是当前解析逻辑使用的 RFC1123/GMT 格式，因此现阶段会打印 `parse failed`
- 该问题不影响 GB 注册流程本身，但会影响后续自动校时联调

## 后续建议

如果平台最终继续返回 ISO8601，建议下一步补两套解析：

- RFC1123: `Tue, 16 Mar 2026 08:12:22 GMT`
- ISO8601: `2026-03-16T16:12:22.617`

这样平台和设备侧都更稳，不依赖单一日期格式。
