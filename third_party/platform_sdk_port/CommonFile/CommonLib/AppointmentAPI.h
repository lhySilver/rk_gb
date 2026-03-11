// c++ 风格 API, 非通用API, 需要保证 动态库的 libc 与 可执行程序的libc一致, 否则可能出现指针错误
#pragma once

#ifdef __WINDOWS__
#define W_EXPORT __declspec(dllexport)
#else
#define W_EXPORT __attribute__ ((visibility("default")))
#endif

#include <stdint.h>
#include <iostream>
#include <map>
#include "AppointmentProtocol.h"
#include <memory>
// 初始化
W_EXPORT void wechat_env_init(const std::string& dbPath);
W_EXPORT void wechat_env_destroy();

enum VersionState {
    VersionVisitRequest = 1,          // 收到 访客预约信息, 初始状态(nvms 不管这个状态)
    VersionVisitSynced = 2,    // 访客预约信息已同步至nvms, nvms初次收到访客预约数据时标记为该值
    VersionAuditRequest = 3,  // 收到访客审核信息
    VersionAuditResult = 4,    // 访客审核信息已同步至nvms, nvms完成预约审核(下发目标，添加访客记录)时，标记为该值
};

enum AuditErrorCode {
    AuditErrOK = 200,
    AuditErrAtEndTime,             // 超期
    AuditErrNoReceiver = 0x2101,   // 没有找到接收者
    AuditErrNoFace     = 0x2102,   // 添加人脸库失败
    AuditErrAddRecord  = 0x2104,   // 添加人脸访问记录失败
};

class W_EXPORT AppointmentAPI
{
public:
    AppointmentAPI();
    virtual ~AppointmentAPI();

    // @brief 连接到服务, 使用 wensocket 协议, 该接口路径较长, 仅提供异步接口
    // 函数返回之后，需要根据wsConnected的回调判断是否连接成功
    // 当 wsConnected(isConn = true) 表明连接成功
    // 在任何时候, 当 wsConnected(isConn = false) 都表示连接已经断开
    bool connectHost(const std::string& ipAddr, short port, const std::string& uri = "/ws");
    bool connectDomain(const std::string& domain, const std::string& url = "/ws");
    // 断线重连
    bool disconnect();
    bool isConnected();

    std::string getURL();

    // 订阅
    // @param timeout,
    //  当timeout == -1, 函数立即返回, 使用 wEventState 接收命令的状态
    //  当timeout != -1, 函数将最多阻塞timeout seconds
    // @return
    //  -1 失败
    //  其他 消息的ID
    // 该命令发起之后，需要监听 wsAppointment 这个回调，以接收新的访客数据
    int64_t requestSubscribeAppointment(int timeout = 5);

    // 退订
    // @param timeout,
    //  当timeout == -1, 函数立即返回, 使用 wEventState 接收命令的状态
    //  当timeout != -1, 函数将最多阻塞timeout seconds
    // @return
    //  -1 失败
    //  其他 消息的ID
    int64_t requestUnsubscribeAppointment(int timeout = 5);

    // 请求同步
    // @param timeout,
    //  当timeout == -1, 函数立即返回, 使用 wEventState 接收命令的状态
    //  当timeout != -1, 函数将最多阻塞timeout seconds
    // @return
    //  -1 失败
    //  其他 消息的ID
    int64_t requestSynchronizeRecord(int timeout = 5);

    // 上传访客业务审核结果
    // @param timeout,
    //  当timeout == -1, 函数立即返回, 使用 wEventState 接收命令的状态
    //  当timeout != -1, 函数将最多阻塞timeout seconds
    // @return
    //  -1 失败
    //  其他 消息的ID
    int64_t requestAuditAppointment(const visitor::Operation& op, int timeout = 5);

    // 更新设备状态, 设备列表 & 设备状态
    // @param timeout,
    //  当timeout == -1, 函数立即返回, 使用 wEventState 接收命令的状态
    //  当timeout != -1, 函数将最多阻塞timeout seconds
    // @return
    //  -1 失败
    //  其他 消息的ID
    int64_t requestUpdateDevice(const visitor::Area& area, int timeout = 5);

public:
    bool getVersionedAppointment(uint32_t startTime, uint32_t endTime, visitor::AppointmentState status, std::map<std::string, int64_t>& versionedAppointMap);
    bool getAppointment(const std::string& visitID, visitor::Appointment& appoint);
    bool getPreAuditAppointment(std::map<std::string, int64_t>& versionedAppointMap);

    // 更新访客预约记录的版本号
    // 被动接收公众号服务器的推送，主动推送至微信公众号
    // 接收到访客预约信息时
    bool updateAppointmentVersion(const std::string& visitID, int64_t version);
    bool updateAppointmentAudit(const visitor::RequestAuditResult& result);

    bool getVisitTarget(const std::string& visitID, uint32_t& targetID);
    bool setVisitTarget(const std::string& visitID, uint32_t targetID);

    bool getVisitRecordID(const std::string& visitID, std::string& record);
    bool setVisitRecordID(const std::string& visitID, const std::string& record);
public:
    // 回调 连接状态
    // @brief wsConnected, websocket 的连接&握手状态
    // @param isConn,
    //      true表明握手成功
    //      false表示socket连接断开
    // @param errMsg, 回调出来的一些提示信息
    virtual bool connectState(bool isConn, const std::string& errMsg) = 0;

    // 回调 命令处理状态
    // @brief 发出去的命令在使用非阻塞接口时回调
    // @param msgID, 主动调用类中接口的返回值，唯一标识一个消息
    // @param msg, 返回信息， 成功/失败
    virtual bool eventState(int64_t msgID, std::shared_ptr<visitor::VisitError> msg) = 0;

    // 回调 访客预约详情推送
    // @brief 访客预约数据, 微信服务器收到访客预约信息之后，通过该接口调用到应用服务
    // @param msgID, 消息的ID
    // @param msg, 访客预约消息的内容
    virtual bool requestAppointment(int64_t msgID, std::shared_ptr<visitor::Appointment> msg) = 0;

    // 回调 在web上的审核请求
    // @brief 接收访客验证审核请求，推动到应用服务，应用服务进行后续的请求处理(该接口需要快速的返回，尽量不要阻塞)
    // @param msgID: 消息的ID， 返回消息中带上该ID
    // @param req: 请求信息
    // @param result: 处理结果
    virtual bool requestAudit(int64_t msgID, std::shared_ptr<visitor::RequestAuditParam> req,
                              std::shared_ptr<visitor::RequestAuditResult>& result) = 0;

private:
    class ClientImpl* m_cliImpl;
};
