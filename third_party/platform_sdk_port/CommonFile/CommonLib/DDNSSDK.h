#ifndef DDNS_SDK_HEAD
#define DDNS_SDK_HEAD

#ifdef WIN32

#ifdef DDNS_SDK_EXPORT_API//动态库编译
#define DDNS_SDK  __declspec(dllexport)
#define DDNS_CLASS  __declspec(dllexport)
#else
#ifdef DDNS_SDK_EXPORT_STATIC//静态库编译和使用
#define DDNS_SDK
#define DDNS_CLASS
#else//使用者编译
#define DDNS_SDK  __declspec(dllimport)
#define DDNS_CLASS  __declspec(dllimport)
#endif
#endif

#else
#define DDNS_SDK __attribute__ ((visibility("default")))
#define DDNS_CLASS __attribute__ ((visibility("default")))
#endif

enum SERV_TYPE
{
    DDNS_NONE = 0,
    DDNS_NOIPDNS,
    DDNS_MYQSEE,
    DDNS_MINTDNS,
    DDNS_MEIBU,
    DDNS_EASTERNDNS,
    DDNS_DYNDNS,
    DDNS_DVRDYDNS,
    DDNS_DNS2P,
    DDNS_88IP,
    DDNS_3322,
    DDNS_123DDNS,
    DDNS_AUTODDNS
};

enum DDNS_RET
{
	DDNS_OK = 0,						//连接成功
	DDNS_DNS_ERROR,				//dns解析错误
	DDNS_NETCFG_ERROR,			//网络连接错误
	DDNS_PWD_ERROR,				//帐号密码错误
	DDNS_UNKNOWN_ERROR,   //未知错误
    DDNS_REC_ERROR,				//数据接收错误
    DDNS_HOSTNAME_ALREADY_USED  //域名已经被注册使用了
};


DDNS_SDK bool DDNS_Init(const char* szEthName);
DDNS_SDK void DDNS_Quit();

DDNS_SDK bool DDNS_Start(const char *szCfgXML);
DDNS_SDK bool DDNS_Stop();

DDNS_SDK bool DDNS_CheckLinkStatus();
DDNS_SDK int DDNS_GetServType();

DDNS_SDK int CheckDDNS(const char *szCfgXML);
#endif // DDNSSDK_H
