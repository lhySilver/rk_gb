# 任务清单: GB 实时预览与录像回放并发

目录: `helloagents/plan/202603271900_gb_live_replay_concurrency/`

---

## 1. ProtocolManager 媒体发送状态拆分
- [√] 1.1 在 `App/Protocol/ProtocolManager.h` 中为 live 与 replay 分别引入独立 sender 运行态，验证 why.md#需求-实时预览与录像回放并存
- [√] 1.2 在 `App/Protocol/ProtocolManager.cpp` 中按请求类型重构 sender 初始化、端口/SSRC 读取和关闭流程，验证 why.md#需求-实时预览与录像回放并存，依赖任务1.1

## 2. 会话控制互斥范围收缩
- [√] 2.1 在 `App/Protocol/ProtocolManager.cpp` 中调整 replay 启动与 stop 流程，避免 live 与 replay 互相顶掉，验证 why.md#需求-停流精准命中，依赖任务1.2
- [√] 2.2 在 `App/Protocol/ProtocolManager.cpp` 中校正 ACK、回放发帧和媒体状态通知路径，验证 why.md#需求-实时预览与录像回放并存，依赖任务2.1

## 3. 安全检查
- [√] 3.1 执行安全检查（确认无跨会话资源误释放、无 sender 空指针和句柄误命中）

## 4. 文档更新
- [√] 4.1 更新 `helloagents/wiki/modules/gb28181.md`
- [√] 4.2 更新 `helloagents/CHANGELOG.md`

## 5. 测试
- [√] 5.1 执行编译或最小构建验证，确认改动未引入编译错误
> 备注: 已使用 `g++ -fsyntax-only -std=gnu++11` 对 `App/Protocol/ProtocolManager.cpp` 做语法级编译验证；`make -C cmake-build dgiot` 受生成环境中失效的 `/opt/cmake/.../cmake` 固定路径影响，未完成整工程构建。

---

## 任务状态符号
- `[ ]` 待执行
- `[√]` 已完成
- `[X]` 执行失败
- `[-]` 已跳过
- `[?]` 待确认
