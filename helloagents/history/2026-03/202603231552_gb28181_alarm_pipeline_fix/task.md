# 任务清单: GB28181报警上报链路补齐

目录: `helloagents/history/2026-03/202603231552_gb28181_alarm_pipeline_fix/`

---

## 1. App报警事件接线
- [√] 1.1 在 `rk_gb/App/Media/Alarm.cpp` 与 `rk_gb/App/Main.*` 中接入 `ProtocolManager` 访问路径，并在报警开始/结束边沿触发 GB28181 报警通知，验证 `why.md#需求-soc报警触发后上报gb28181平台-场景-本地检测到移动或人形报警`
- [√] 1.2 在 `rk_gb/App/Protocol/ProtocolManager.*` 中补充本地视频报警状态缓存、类型映射和对外通知入口，验证 `why.md#需求-soc报警触发后上报gb28181平台`

## 2. SDK与XML缺口修正
- [√] 2.1 在 `rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h` 中为 `AlarmNotifyInfo` 补充 `AlarmTypeParam` 等最小必要字段，验证 `why.md#需求-报警通知报文满足最小联调要求`
- [√] 2.2 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/alarm_notify.*` 与 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/GB28181XmlParser.cpp` 中补齐 `AlarmTypeParam` 编解码，并让 `AlarmID/ExtendInfo` 至少不再只停留在内存层，验证 `why.md#需求-报警通知报文满足最小联调要求-场景-报警通知编码`
- [√] 2.3 在 `rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/GBClientImpl.cpp` 中修正报警订阅句柄删除错误，验证 `why.md#需求-报警通知报文满足最小联调要求-场景-报警通知编码`

## 3. 报警复位收敛
- [√] 3.1 在 `rk_gb/App/Protocol/ProtocolManager.cpp` 中将 `ResetAlarm` 收敛为“清本地录像标志 + 清 GB 报警状态”，验证 `why.md#需求-平台报警复位后设备状态一致-场景-平台下发-resetalarm`

## 4. 验证与文档同步
- [√] 4.1 执行编译验证，至少覆盖 `rk_gb/App` 目标或相关受影响目标
- [√] 4.2 更新 `helloagents/wiki/modules/gb28181.md`、`helloagents/CHANGELOG.md`、`helloagents/history/index.md`
- [√] 4.3 将本方案包迁移到 `helloagents/history/2026-03/` 并写入最终执行结论

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认

## 执行结论
- 已补齐 `CAlarm -> ProtocolManager -> GB28181 SDK/XML` 的本地视频报警最小上报闭环，并让 `ResetAlarm` 同步清理 GB 报警状态缓存。
- 已同步更新 `helloagents/wiki/modules/gb28181.md`、`helloagents/wiki/modules/terminal_requirements.md`、`helloagents/CHANGELOG.md` 与 `helloagents/history/index.md`。
- 当前环境无法完成目标交叉编译验证：`cmake-build/Makefile` 依赖不存在的 `/opt/cmake/...`，且本机缺少 `arm-rockchip830-linux-uclibcgnueabihf-g++`。
- 已改为对受影响源文件执行主机侧 `g++ -fsyntax-only` 语法校验；`Alarm.cpp`、`ProtocolManager.cpp`、`GB28181XmlParser.cpp`、`alarm_notify.cpp`、`GBClientImpl.cpp` 均通过。
