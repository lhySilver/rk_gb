
////////////////////

#ifndef _NET_DEV_YITU_PAD_SDK_H_
#define _NET_DEV_YITU_PAD_SDK_H_

#ifdef WIN32 

#ifdef _NET_PROTOCOL_YITU_PAD_DLL_//动态库编译 
#define _EXPORT_NETDEV_YITU_PAD  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_YITU_PAD  __declspec(dllimport)
#endif

#else
#define _EXPORT_NETDEV_YITU_PAD __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

_EXPORT_NETDEV_YITU_PAD bool NET_DEV_YITU_PAD_Initial();
_EXPORT_NETDEV_YITU_PAD void NET_DEV_YITU_PAD_Quit();

#endif   ///////_NET_DEV_YITU_SDK_H_








