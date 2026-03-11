
////////////////////

#ifndef _NET_ADAPTER_ONVIF_SDK_H_
#define _NET_ADAPTER_ONVIF_SDK_H_

#ifdef WIN32 

#ifdef _NET_PROTOCOL_ONVIF_DLL_//动态库编译 
#define _EXPORT_NETDEV_ONVIF  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_ONVIF  __declspec(dllimport)
#endif

#else
#define _EXPORT_NETDEV_ONVIF __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

_EXPORT_NETDEV_ONVIF bool NET_DEV_ONVIF_Initial();
_EXPORT_NETDEV_ONVIF void NET_DEV_ONVIF_Quit();

#endif   ///////_NET_ADAPTER_ONVIF_SDK_H_








