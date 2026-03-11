#ifndef UPNP_SDK_HEAD
#define UPNP_SDK_HEAD

#ifdef WIN32

#ifdef UPNP_SDK_EXPORT_STATIC//静态库编译
#define UPNP_SDK extern "C"
#else
#ifdef UPNP_SDK_EXPORT_API//动态库编译
#define UPNP_SDK extern "C" __declspec(dllexport)
#else//使用者编译
#define UPNP_SDK extern "C" __declspec(dllimport)
#endif
#endif

#else
#define UPNP_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include <string>

enum UPNP_STATUS
{
    //以下是针对单个upnp 设置的状态
    UPNP_NOT_FOUND_RECORD,
    UPNP_NOT_READY,
    UPNP_ACTIVED
};

UPNP_SDK bool Upnp_Init(const char *szEthName);
UPNP_SDK void Upnp_Quit(void);

UPNP_SDK bool Upnp_Start(void);
UPNP_SDK void Upnp_Stop(void);

UPNP_SDK bool Upnp_RegisterMappingPort(tuint16 localPort, tuint16 ExternalPort);  
UPNP_SDK std::string Upnp_GetExternalIP(void);
UPNP_SDK UPNP_STATUS Upnp_GetStatus(tuint16 localPort); /////需要定义一套枚举值， 表示状态
UPNP_SDK bool Upnp_UnRegisterMappingPort(tuint16 localPort);	 

#endif // UPNPSDK_H
