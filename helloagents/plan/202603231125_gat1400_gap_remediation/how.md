# 技术设计: GAT1400完成度补齐

## 技术方案
### 核心技术
- C++ GAT1400 lower client
- `GAT1400Json` 编解码
- HTTP/Digest 认证
- 本地配置与远端配置同步
- 观察者回调与业务事件接线

### 实现要点
- 先补“lower client 联调闭环”，包括注册恢复、失败补传、业务接线、订阅通知，而不是先做完整 `/VIAS/*`。
- 把传输层地址模型从“`server_ip + server_port`”提升为“协议/地址/端口/路径/超时”的统一表达，避免后续每个请求单独拼接。
- 所有 `Post*` 接口最终统一走一个“发送 -> 判定失败 -> 入队 -> 重放”的可靠性框架，不再各自散落重试逻辑。
- 订阅处理要继续保留本地 `/VIID/Subscribes` 服务能力，但需补业务层观察者接线和必要的安全/鉴权边界。
- `/VIAS/*` 和完整分析系统服务面不纳入本阶段强制交付，只作为扩展选项保留。

## 架构决策 ADR
### ADR-20260323-01: 先做 lower client 完整度，不先做完整 `/VIAS/*`
**上下文:** 当前仓库已具备大量 `/VIID/*` 对上客户端能力，但没有完整 `/VIAS/*` 服务面，实现范围继续扩大将显著增加周期和不确定性。
**决策:** 第一阶段只把当前 lower client 做到可联调、可重试、可补传、可接业务；`/VIAS/*` 放入远期扩展。
**理由:** 这部分最贴近现有代码结构，也最容易形成可验证交付。
**替代方案:** 直接补齐 `/VIAS/*` 任务、状态、能力查询服务 → 拒绝原因: 现有仓库没有对应服务骨架，属于新增大模块。
**影响:** 1400 能力完成度会优先偏向“设备侧/下级侧”，而不是完整分析系统。

### ADR-20260323-02: 建立统一待发送队列，再挂接所有 `Post*` 接口
**上下文:** 当前 `PostJsonWithResponseList` / `PostJsonWithResponseStatus` 只有即时重试，没有跨注册周期的缓存补传。
**决策:** 新增统一的待发送抽象，所有结构化对象上报、通知上报和 `Data` 上传都复用这套机制。
**理由:** 便于统一重试、注册恢复、日志和持久化策略。
**替代方案:** 为每种对象分别加缓存逻辑 → 拒绝原因: 容易造成语义不一致，后期难维护。
**影响:** 需要为上报消息定义最小封装格式和状态机。

## 数据模型
```text
GatRegisterParam:
  + scheme/http_or_https
  + base_path
  + request_timeout_ms
  + retry_backoff_policy

GatUploadParam:
  + batch_size
  + flush_interval_ms
  + retry_policy
  + queue_dir
  + max_pending_count
  + replay_interval_sec

PendingUploadItem:
  + method
  + path
  + content_type
  + body
  + category
  + object_id
  + attempt_count
  + last_error
  + enqueue_time
```

## 分阶段路线

### P0 | 联调阻塞项
- 统一 GAT1400 传输地址模型，补齐协议、路径和超时表达。
- 把保活失败重注册与待发送队列打通，实现最小“失败缓存 + 重注册后补传”闭环。
- 明确 `PostFaces/PostImages/PostVideoSlices/PostSubscribeNotifications` 等能力的业务接线入口。
- 确认 `LowerGAT1400SDK` 对外导出面是否需要由本仓补实现。

### P1 | 增强项
- 根据平台要求补齐 HTTPS/TLS 支持。
- 完善订阅本地服务的访问控制、鉴权或最小来源校验。
- 扩展更多对象类型的真实业务上报路径，并补足更细粒度的日志和状态统计。
- 增加 1400 协议 smoke test / mock server 回归。

### P2 | 远期扩展
- `/VIAS/*` 分析系统服务面。
- 更完整的任务、状态、能力查询支持。
- 离线文件级缓存和上报结果审计工具。

## 安全与性能
- **安全:** 配置中的用户名、密码、可能新增的 TLS 材料不能在日志中明文打印。
- **安全:** 本地 `/VIID/Subscribes` 服务若暴露在非可信网络，应至少限制来源或增加基本鉴权。
- **性能:** 待发送队列默认先按轻量持久化或内存+文件混合实现，避免影响实时抓拍主链路。

## 测试与部署
- **测试:** 建议建立本地 mock VIID 服务，覆盖 `200/401/超时/断链/重注册/补传`。
- **测试:** 需要至少覆盖 `Faces`、`Images`、`VideoSlices`、`SubscribeNotifications` 四类代表性对象。
- **部署:** 先在联调环境验证 P0；P1 的 HTTPS 和更多对象接线在平台明确要求后启用。
