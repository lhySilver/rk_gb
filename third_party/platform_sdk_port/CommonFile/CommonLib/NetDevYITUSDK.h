
////////////////////

#ifndef _NET_DEV_YITU_SDK_H_
#define _NET_DEV_YITU_SDK_H_

#ifdef WIN32 

#ifdef _NET_PROTOCOL_YITU_DLL_//动态库编译 
#define _EXPORT_NETDEV_YITU  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_YITU  __declspec(dllimport)
#endif

#else
#define _EXPORT_NETDEV_YITU __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

_EXPORT_NETDEV_YITU bool NET_DEV_YITU_Initial();
_EXPORT_NETDEV_YITU void NET_DEV_YITU_Quit();

#endif   ///////_NET_DEV_YITU_SDK_H_








