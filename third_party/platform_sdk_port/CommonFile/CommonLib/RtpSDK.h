#ifndef CRTPSDK_H
#define CRTPSDK_H

#include "RtpDefs.h"
class CRtpSessionManager;

class EXPORT_RTP_SDK CRtpSDK
{
public:
    CRtpSDK();
    ~CRtpSDK();
    //************************************
    // Method:     Init
    // Access:     public
    // Returns:    int
    // Qualifier:  SDK初始化接口,初始化资源
    // Parameter:  设置RTP的使用的端口资源
    //************************************
    int Init( RtpPort* port) ;

    //************************************
    // Method:     UnInit
    // Access:     public
    // Returns:    bool
    // Qualifier:  SDK释放接口
    // Parameter:  NULL
    //************************************
    bool UnInit();


    //************************************
    // Method:     CreateSession
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。创建一个RTP会话接口
    // Parameter:  RtpSessionParam*，用户设置该RTP会话具体参数
    // Parameter:  SESSION_HANDLE*，用户传入该RTP会话的句柄指针
    //             创建会话失败, 句柄为NULL;
    //             成功，句柄为具体值，唯一标识该RTP会话
    //************************************
    int CreateSession(RtpSessionParam* param, SESSION_HANDLE* session_handle);

    //************************************
    // Method:     Connect0
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。连接对端rtp会话。rtp传输模式为UDP或TCP client时使用
    // Parameter:  hHandle  rtp会话句柄
    // Parameter:  *dst_ip  对端IP
    // Parameter:  port     对端端口
    //************************************
    int  Connect(SESSION_HANDLE hHandle, const char* dst_ip, uint16_t port);



    //************************************
    // Method:     GetPort
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。
    // Parameter:  hHandle  rtp会话句柄
    // Parameter:  *port    设置该RTP会话的使用端口
    //************************************
    int GetPort(SESSION_HANDLE hHandle,uint16_t* port);


    //************************************
    // Method:     GetSSRC
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。
    // Parameter:  hHandle  rtp会话句柄
    // Parameter:  *port    设置该RTP会话的SSRC
    //************************************
    int GetSSRC(SESSION_HANDLE hHandle,uint32_t* ssrc);



    //************************************
    // Method:     DestorySession
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。销毁RTP接口
    // Parameter:  hHandle  rtp会话句柄
    //************************************
    int  DestorySession(SESSION_HANDLE hHandle);


    //************************************
    // Method:     SendData
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。发送一帧数据接口。
    //             特别注意：不允许对一个RTP会话并发发送数据
    //                             不同RTP会话可以并发发送
    // Parameter:  hHandle  rtp会话句柄
    // Parameter:  frame    一帧RTP帧
    //************************************
    int   SendData(SESSION_HANDLE hHandle, RtpFrame* frame, uint32_t frameInterval = 3600/*rtp时间戳帧间隔*/);


    //************************************
    // Method:     SetReceiver
    // Access:     public
    // Returns:    返回错误码。错误码具体值参考RtpErrorCode
    // Qualifier:  线程安全。设置回调接口。
    // Parameter:  hHandle  rtp会话句柄
    // Parameter:  MediaReciever*    RTP帧回调，是一个完整的帧
    // Parameter:  RTPEventReciever  RTP事件回调，如丢包，连接断开等
    //************************************
    int   SetReceiver(SESSION_HANDLE hHandle , MediaReciever*, RTPEventReciever*);

private:
    CRtpSDK(const CRtpSDK &);
    CRtpSDK &operator= (const CRtpSDK &);
private:
    CRtpSessionManager* m_rtp_impl;
};


#endif // CRTPSDK_H




