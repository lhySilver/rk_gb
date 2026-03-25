# 变更提案: 禁用 GAT1400 校时

## 背景
- 当前 `GAT1400ClientService::Start()` 在注册后会主动调用 `GetTime()` 拉取 `/VIID/System/Time`。
- 系统内已经由 GB28181 路径负责设备校时，不希望再由 GAT1400 并行管理时间，避免多头同步。

## 目标
- 将 GAT1400 校时实现改为空操作，不再对平台发起校时请求。
- 保留现有注册、保活、订阅和数据上报链路不变。

## 范围
- `App/Protocol/gat1400/GAT1400ClientService.cpp`
- `helloagents/wiki/modules/protocol.md`
- `helloagents/CHANGELOG.md`
- `helloagents/history/index.md`
