
///////////////////////////NetDevN9KSDK.h

#ifndef _NET_DEVPR_SDK_H
#define _NET_DEVPR_SDK_H

#ifdef WIN32
#ifdef __PRNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_PR_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_PR_SDK  __declspec(dllimport)
#endif
#else
#define _EXPORT_NETDEV_PR_SDK  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

//网络模块的初始化和销毁
_EXPORT_NETDEV_PR_SDK bool NET_DEV_PrSDK_Initial();
_EXPORT_NETDEV_PR_SDK void NET_DEV_PrSDK_Quit();

#endif //_NET_DEVN9K_SDK_H