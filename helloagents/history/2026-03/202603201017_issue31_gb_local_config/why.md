# 变更提案

## 背景
- `silver` 分支 issue #31 要求将 GB28181 接入参数从代码默认值改为配置文件读取。
- 设备侧需要把配置文件固定放到 `/userdata/conf/Config`，并在文件缺失时自动生成默认配置。

## 根因判断
- 当前 `LocalConfigProvider` 只有内存默认值，启动后不会读取或持久化本地文件。
- `ProtocolManager` 也没有 GB28181 总开关，无法在不改代码的前提下完全禁用 GB 生命周期。

## 成功标准
- 启动时优先读取 `/userdata/conf/Config/gb28181.ini`。
- 文件不存在时使用代码默认值并自动生成默认配置文件。
- 配置文件存在时，以文件中的 `enable/username/server_ip/server_port/device_id/password` 为准。
- 当 `enable=0` 时，设备跳过 GB 注册生命周期且不再触发 GB 重注册。
