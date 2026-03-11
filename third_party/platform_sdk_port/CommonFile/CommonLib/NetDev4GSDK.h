
///////////////////////////NetDevN9KSDK.h

#ifndef _NET_DEV4G_SDK_H
#define _NET_DEV4G_SDK_H

#ifdef WIN32
#ifdef __4GNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_4G_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_4G_SDK  __declspec(dllimport)
#endif
#else
#define _EXPORT_NETDEV_4G_SDK  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

//网络模块的初始化和销毁
_EXPORT_NETDEV_4G_SDK bool NET_DEV_4GSDK_Initial();
_EXPORT_NETDEV_4G_SDK void NET_DEV_4GSDK_Quit();

#endif //_NET_DEVN9K_SDK_H