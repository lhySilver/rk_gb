# 需求说明

## 背景
- 当前 keepalive demo 虽然由 1400 模块内部触发，但对平台实际走的是 `Faces/Images/Data` 资源上传链路，不是 1400 的告警消息接口。
- 联调口径已经明确：这次 demo 关注的是“和 1400 平台走告警通知路径”，而不是验证抓拍资源 CRUD 链路。

## 目标
- 将 keepalive demo 的平台通信改为 `POST /VIID/DispositionNotifications`。
- 保留本地图片 `/mnt/sdcard/test.jpeg`，避免引入视频或额外磁盘读取逻辑。
- 保持 demo 简单，继续限制每次服务启动后最多触发 `5` 次。
