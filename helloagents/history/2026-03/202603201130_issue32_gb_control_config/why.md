# 变更提案

## 背景
- issue #32 要求补齐两项 GB28181 能力：远程重启控制、画面翻转配置。
- 当前分支虽然已经有 `HandleGbTeleBootControl()` 和 `HandleGbConfigControl()` 入口，但翻转控制没有接到设备主配置链路，本地配置文件也没有保存翻转字段。

## 根因判断
- `ProtocolManager` 处理 GB 图像翻转时走的是独立 RK ISP 接口，没有复用项目里长期生效的 `CFG_CAMERA_PARAM.rotateAttr` 主链路。
- `LocalConfigProvider` 的 `gb28181.ini` 只保存注册参数，没有保存 `image_flip_mode`，启动后无法从本地配置恢复翻转状态。
- 远程重启命令没有冷却保护，重复触发时缺少最基本的防抖。

## 成功标准
- GB 图像翻转配置写入 `CFG_CAMERA_PARAM.rotateAttr`，由 Camera/Alarm 现有配置回调统一生效。
- `/userdata/conf/Config/gb28181.ini` 支持 `image_flip_mode` 的读取与回写。
- 设备启动和配置热更新时会按 GB 配置恢复翻转状态。
- GB `TeleBoot` 支持冷却保护，短时间重复命令会被拒绝。
