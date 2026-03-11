#ifndef SMTPCLIENTSDK_H1
#define SMTPCLIENTSDK_H1

#ifdef WIN32

#ifdef SMTP_CLIENT_SDK_EXPORT_API//动态库编译
#define SMTP_CLIENT_SDK  __declspec(dllexport)
#define SMTP_CLIENT_CLASS  __declspec(dllexport)
#else
#ifdef SMTP_CLIENT_SDK_EXPORT_STATIC//静态库编译和使用
#define SMTP_CLIENT_SDK
#define SMTP_CLIENT_CLASS
#else//使用者编译
#define SMTP_CLIENT_SDK  __declspec(dllimport)
#define SMTP_CLIENT_CLASS  __declspec(dllimport)
#endif
#endif

#else
#define SMTP_CLIENT_SDK __attribute__ ((visibility("default")))
#define SMTP_CLIENT_CLASS __attribute__ ((visibility("default")))
#endif

#include <list>
#include "base_type_define.h"

#define DEFAULT_SMTP_PORT 25
#define SSL_SMTP_PORT 465


// 自定义的错误信息对应的错误码,
// 客户端也用到了,慎重修改
typedef enum
{
    // "No error."
    MAIL_ERR_NONE,
    // "device error."
    MAIL_ERR_DEV_ERROR,
    // "parameter wrong."
    MAIL_ERR_PARAM_WRONG,
    // "Can't get the ip from the mail server domain name. Maybe the DNS or the domain name is wrong."
    MAIL_ERR_GET_SMTP_SERVER_ADDR_FAIL,
    // "Can't connect the mail server."
    MAIL_ERR_CONNECT_MAIL_FAIL,
    // "Timeout!"
    MAIL_ERR_SMTP_RECV_TIMEOUT,
    // "Send error!"
    MAIL_ERR_SMTP_SEND_ERROR,
    // "This version is not support SSL!"
    MAIL_ERR_SMTP_NOT_SUPPORT_SSL,
    // <domain> Service ready
    MAIL_ERR_SERVICE_NOT_READY,
    // "the reply msg is wrong"
    MAIL_ERR_REPLY_WRONG
} MAIL_ERR_CODE;

const unsigned char  SSL_AUTH_MODE_NONE = 0;
const unsigned char  SSL_AUTH_MODE_SSL = 1;
const unsigned char  SSL_AUTH_MODE_TLS = 2;

typedef struct _attachment_info
{
    int		iType;
    char	*pAttachName;
    char	*pAttachData;	//附件数据
    int     iLen;			//附件数据长度
}ATTACHMENT_INFO;


typedef struct _smtp_para
{
    unsigned long dwSSL;	//SSL_AUTH_MODE_NONE SSL_AUTH_MODE_SSL SSL_AUTH_MODE_TLS
    unsigned short nPort;	//主机序端口
    char pFromaddr[64];
    char pReplyTo[64];
    char pSenderName[64];  //发件人姓名（别名）
    char pUserName[32];
    char pPassword[32];
    char pServAddr[32];
    char pSubject[64];
    char pTextContent[1024];

    std::list<char*>	*pToAddrList;
    std::list<char*>	*pCcAddrList;
    std::list<ATTACHMENT_INFO*>	*pAttachmentList;
}SMTP_PARA;

SMTP_CLIENT_SDK void SMTP_Init();
SMTP_CLIENT_SDK bool SMTP_Send(SMTP_PARA *pSmtpParam);  // 同步操作

// 异步操作smtp发送
SMTP_CLIENT_SDK bool SMTP_AsycSend(const SMTP_PARA& smtpParam);

SMTP_CLIENT_SDK void SMTP_Quit();

#endif // SMTPCLIENTSDK_H1
