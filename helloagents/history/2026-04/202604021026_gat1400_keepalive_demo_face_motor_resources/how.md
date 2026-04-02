# 实现方案

## 代码调整
- 将 keepalive demo 入口改为 `SendKeepaliveDemoFaceAndMotorResources()`，并在保活成功后调用。
- demo 读取 `/mnt/sdcard/test.jpeg` 一次，复用同一份 Base64 数据构造：
  - 一条 `FaceObject`，包含场景图 `Type=14` 和抓拍图 `Type=11`。
  - 一条 `MotorVehicleObject`，包含车辆大图 `Type=01`、车牌图 `Type=02`、车辆特写图 `Type=09`。
- 保留“失败回退 demo 计数”的行为，并将上限收口为每次服务启动 `1` 次。

## JSON 对齐
- `FaceObject` 额外输出 `ShotTime`，并在时间字段非空时直接透传 `LocationMarkTime/FaceAppearTime/FaceDisAppearTime`。
- `MotorVehicleObject` 额外输出 `Speed/Direction/VehicleLength/PassTime`，且时间字段不再受 `InfoKind == manual` 限制。
- `SubImage.Type` 统一按两位数字序列化，便于对齐 `01/02/09/11/14` 这类平台样例。
