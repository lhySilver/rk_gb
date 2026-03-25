# 变更提案: GB28181报警上报链路补齐

## 需求背景
当前仓库中，GB28181 SDK 已具备报警订阅、报警通知和报警复位能力，但 App 层真实报警事件并没有接到国标上报出口。

静态审查确认的主要问题有：
- SoC/算法告警只驱动本地录像、警笛和涂鸦通知，没有驱动 `ProtocolManager::NotifyGbAlarm`。
- 平台下发 `ResetAlarm` 时，仅清理了本地报警录像标志，不等价于国标告警状态复位。
- SDK 报警订阅句柄管理存在实现瑕疵，重订阅后可能残留旧 timer/handle。
- 报警 XML 编码未携带 `AlarmTypeParam` 等扩展信息，`AlarmID`/`ExtendInfo` 也未真正进入线上的 XML。

这导致链路现状更接近“本地报警联动可用，国标平台报警上报不可用”，不能满足从 SoC 触发到国标平台告警闭环的要求。

## 变更内容
1. 将 App 侧真实报警事件接到 GB28181 报警上报入口，形成最小可用闭环。
2. 为本地报警类型补充到 GB28181 视频报警类型的明确映射，并输出可联调的描述信息。
3. 修正报警订阅句柄删除错误和报警 XML 字段承载缺口。
4. 将报警复位从“只清录像标志”收敛为“清录像标志 + 清国标报警状态”。

## 影响范围
- **模块:** `Media/Alarm`、`ProtocolManager`、GB28181 SDK Client、GB28181 XML 编解码
- **文件:** `rk_gb/App/Media/Alarm.*`、`rk_gb/App/Main.*`、`rk_gb/App/Protocol/ProtocolManager.*`、`rk_gb/third_party/platform_sdk_port/CommonFile/CommonLib/GB28181Defs.h`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/GB28181Client/*`、`rk_gb/third_party/platform_sdk_port/CommonLibSrc/GB28181SDK/include/XML/*`
- **API:** `ProtocolManager::NotifyGbAlarm`、`ResetAlarm`、GB28181 `NOTIFY Alarm`
- **数据:** `AlarmNotifyInfo`、报警订阅上下文、报警 XML `Info/AlarmTypeParam`

## 核心场景

### 需求: SoC报警触发后上报GB28181平台
**模块:** `Media/Alarm` / `ProtocolManager`
设备本地检测到移动、人形等报警时，应在本地联动之外，同步触发国标报警通知。

#### 场景: 本地检测到移动或人形报警
算法回调产生移动侦测结果，`CAlarm::ThreadProc` 聚合后判定告警开始。
- 设备继续执行本地录像、警笛和其他本地联动
- 同时构造 `AlarmNotifyInfo` 并调用 `ProtocolManager::NotifyGbAlarm`
- 平台已订阅报警时，设备向平台发送 `NOTIFY Alarm`

#### 场景: 报警结束
告警状态从触发态恢复为非触发态。
- 设备停止本地报警录像联动
- 设备向平台发送 `AlarmState=0` 的报警结束通知

### 需求: 平台报警复位后设备状态一致
**模块:** `ProtocolManager`
平台下发 `ResetAlarm` 后，设备应同时收敛本地录像标志和 GB 告警状态，而不仅是清录像。

#### 场景: 平台下发 ResetAlarm
平台已收到设备报警并执行复位控制。
- 设备清理本地报警录像触发标志
- 设备清理当前缓存的 GB 报警状态
- 后续新告警仍可正常再次触发和上报

### 需求: 报警通知报文满足最小联调要求
**模块:** GB28181 SDK / XML
报警通知需要携带最小必要字段，便于平台按类型识别。

#### 场景: 报警通知编码
设备已收到平台报警订阅。
- 报文中包含 `AlarmPriority`、`AlarmMethod`、`AlarmType`、`AlarmState`、`AlarmTime`、`AlarmDescription`
- 报文中包含 `Info/AlarmTypeParam`，用于承载本地细分类型描述
- 订阅重建后旧句柄不会残留在订阅管理器中

## 风险评估
- **风险:** 报警上报加入到 `Alarm.cpp` 后，可能导致无订阅场景下频繁调用国标出口。
- **缓解:** `ProtocolManager::NotifyGbAlarm` 保持“无订阅直接返回”的轻量行为，并只在状态变化时触发。
- **风险:** 扩展 `AlarmNotifyInfo` 会影响 SDK 共享结构和 XML 编解码。
- **缓解:** 只补最小必要字段，保持旧字段语义不变，并同步更新编码/解码逻辑。
- **风险:** 本地报警类型与 GB 标准视频报警类型并非一一对应。
- **缓解:** 统一落到 `VideoMotion`，再用 `AlarmTypeParam/Description` 区分 `motion/human/vehicle/non_vehicle`。
