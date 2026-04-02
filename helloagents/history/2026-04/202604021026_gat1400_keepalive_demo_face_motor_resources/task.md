# 任务清单

- [√] 将 keepalive demo 改为直接调用 `PostFaces()` 和 `PostMotorVehicles()`
- [√] 使用 `/mnt/sdcard/test.jpeg` mock 出人脸和机动车样例资源，且只触发 `1` 次
- [√] 补齐 `GAT1400Json.cpp` 中本轮联调所需的 Face/MotorVehicle 字段与两位数 `SubImage.Type`
- [√] 更新 `gat1400` 模块文档、`CHANGELOG` 和历史索引
- [√] 执行 `git diff --check`
- [√] 执行最小语法校验：`GAT1400Json.cpp` 可通过 `g++ -fsyntax-only`；`GAT1400ClientService.cpp` 的单文件校验受工程现有缺失头 `Infra/Mutex.h` 阻断，未能完成完整语法检查
