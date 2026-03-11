#ifndef _UPPER_GAT1400_SDK_H_
#define _UPPER_GAT1400_SDK_H_

#include "Lock.h"
#include "CMS1400Struct.h"
#include <map>
#ifdef WIN32
#ifdef GAT1400_EXPORTS
#define EXPORT_GAT1400_SDK  __declspec(dllexport)
#else
#define EXPORT_GAT1400_SDK __declspec(dllimport)
#endif

#else
#define EXPORT_GAT1400_SDK  __attribute__ ((visibility("default")))
#endif


class CGATServerManager;

class EXPORT_GAT1400_SDK  UpperGAT1400SDK
{
public:
    UpperGAT1400SDK();
    ~UpperGAT1400SDK();
    void init();
    void Uninit();
    void Start(int iPort, std::string userName, std::string strPwd, std::string GBCode);//启动
    void Stop();//停止
private:
    CGATServerManager* m_impl;
};

class EXPORT_GAT1400_SDK GAT1400Observer
{
public:
    GAT1400Observer(){};
    virtual ~GAT1400Observer(){};
    //
    // @brief: 获取string格式的Http消息（针对不需要格式解析的场景）
    // @params:strUrl 下级网关发来的URL， iHttpMethod HTTP请求方式， strHttpMessage 下级网关发送的消息体
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    virtual void InformHttpJson(GUID chlGuid, std::string& strUrl, HTTP_MSG_TYPE_H iHttpMethod, std::string& strHttpMessage)
    {
        (void)strUrl;
        (void)iHttpMethod;
        (void)strHttpMessage;
    }
    //
    // @brief: 下级网关注册的国标ID(内部已经处理注册加密)
    // @params:strCode 国标ID, retCode 返回的系统状态
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    virtual void InformRegister(const std::string strCode, STATUS_CODE_H& retCode)
    {
        (void)strCode;
        (void)retCode;
    }
    //
    // @brief: 下级网关注销的国标ID
    // @params:strCode 国标ID, retCode 返回的系统状态
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    virtual void InformUnRegister(const std::string strCode, STATUS_CODE_H& retCode)
    {
        (void)strCode;
        (void)retCode;
    }
    //
    // @brief: 下级网关保活的国标ID
    // @params:strCode 国标ID, retCode 返回的系统状态
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    virtual void InformKeepLive(const std::string strCode, STATUS_CODE_H& retCode)
    {
        (void)strCode;
        (void)retCode;
    };

};
class CGat1400ServerInform;
class EXPORT_GAT1400_SDK CGat1400Server
{

public:
    CGat1400Server();
    ~CGat1400Server();
    //
    // @brief: 添加需要http消息的观察者
    // @params:obersrver 观察者
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    void AddHttpJsonObserver(GAT1400Observer* obersrver);
    //
    // @brief: 删除需要http消息的观察者
    // @params:obersrver 观察者
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    void RemoveHttpJsonObserver(GAT1400Observer* obersrver);

    //
    // @brief: 添加需要下级设备注册状态的观察者
    // @params:obersrver 观察者
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    void AddRegisterStatusObserver(GAT1400Observer* obersrver);
    //
    // @brief: 删除需要下级设备注册状态的观察者
    // @params:obersrver 观察者
    // @ret:void
    // @birth:created by fanph on 2020/6/4
    //
    void RemoveRegisterStatusObserver(GAT1400Observer* obersrver);

};

#endif