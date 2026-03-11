#ifndef SIPDEFS_H
#define SIPDEFS_H

#ifdef WIN32
#ifdef SIP_EXPORTS
#define EXPORT_SIP_SDK  __declspec(dllexport)
#else
#define EXPORT_SIP_SDK __declspec(dllimport)
#endif

#else
#define EXPORT_SIP_SDK  __attribute__ ((visibility("default")))
#endif
#include <stdint.h>
#include <string>
#include "WarningDisable.h"
typedef void* SipSessionHandle;
typedef void* SipDialogHandle;

enum SipErrorCode
{

    kSipSuccess = 0,
    kSipFail,
    kSipBadParam,
    kSipListenThreadFail,
    kSipThreadPoolFail,
    kSipInitFail,
    kSipListenFail,
    kSipWaitTimeout,
    kSipMessageSendFailed,
    kSipSessionNoExist,
    kSipBuildRegisterFailed,
    kSipBuildByeFailed,
    kSipBuildAnswerFailed,
    kSipBuildAckFailed,
    kSipBuildInviteFailed,
    kSipBuildSubscriFailed,
    kSipBuildNotifyFailed,
    kSipBuildRequestFailed,
    kSipBuildInfoFailed,
};


enum SipReponseCode
{

    kOperationFail = 199,
    kSuccessRequest = 200,
    kBadRequest = 400,
    kUnauthorized = 401,
    kForbidden = 403,
    kNotFound = 404,
    kNotAllowed = 405,
    kRequestTimeout = 408,
    kBadMedia = 415,
    kSipTranscationNotExist = 481,
    kMessageCycle = 482,
    kSystemBusy = 486,
    kNotAcceptable = 488,
    kVesionUnsupport = 505

};


enum SipContentType
{

    kSipContentEmpty,                //no content
    kSipContentUnknown,           // unknown type
    kSipContentSDP,                  //SDP
    kSipContentMANSCDP_XML,  //MANSCDP_XML
    kSipContentMANSRTSP,        //MANSRTSP
    //kSipContentData ,               // yyyy-mm-dd hh:mm:ss
    kSipContentRTSP = 7,          //RTSP

};

enum SipMethod
{

    kSipInvaildMethod= 0,
    kSipRegisterMethod,
    kSipInviteMethod,
    kSipInfoMethod,
    kSipNotifyMethod,
    kSipSubscribeMethod,
    kSipCancleMethod,
    kSipByeMethod,
    kSipInfoexMethod,
    kSipNotifyexMethod,
    kSipMessageMethod,
    kSipAckMethod,
	kSipMessageWithCallMethod,
	kSipRefreshSubscribeMethod,
	kSipCancelSubscribeMethod,
};

enum SipTransportType
{
    kSipOverTCP,
    kSipOverUDP
};

struct NetAddress
{
    char*        ip;
    uint16_t     port;
	NetAddress()
	{
		port = 0;
	}
};

 struct SipRegistParam
{

    bool                 new_reg;
    bool                 auth_flag;
    unsigned int      expires;
    char*               user_name;
    char*               password;

};

 struct SipConnectParam
{
    uint16_t      port;
    char*          ip;
    char*          sip_code;
};

 struct SipMessage
 {

     SipReponseCode code;
     SipMethod  Method;
     int      Expirse;
     int      CSeq;
     long    RcvTime;

     char*  Subject;
     char*  Date;
     char*  Event;
     char*  From;
     char*  To;
     char*  reason_phrase;

     SipContentType  content_type;
     char*   content;

	 

 };


struct SipDialogKey
 {
    int   CallId;
    int   DialogId;
    int   TransId;
 };


struct SipData
 {
   SipDialogKey Dialog;
   SipMessage  messgae;
 };



 class SipClientHandler
 {
public:
    SipClientHandler(){}
    virtual ~SipClientHandler(){}
    virtual void OnRequest(SipMethod  method,const SipData* data ){}
    virtual void OnResponse(SipMethod  method,const SipData* data ){}
 };


 class SipLogHandler
 {
public:
    virtual void OnLogInfo(bool request, SipMethod  method, const char* data )=0;
 };


 class SipServerHandler
 {
public:
    SipServerHandler(){}
    virtual ~SipServerHandler(){}
    virtual void OnRequest(SipSessionHandle handle,SipMethod  method,const SipData* data){}
    virtual void OnResponse(SipSessionHandle handle,SipMethod  method,const SipData* data){}
    virtual int  OnClientRegist(SipSessionHandle handle, const SipRegistParam* regist, const SipConnectParam* param){ return true;}

 };

#endif // SIPDEFS_H
