
///////////////////////////NetDevN9KSDK.h

#ifndef _NET_DEVN9K_SDK_H
#define _NET_DEVN9K_SDK_H

#ifdef WIN32
#ifdef __N9KNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_N9K_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_N9K_SDK  __declspec(dllimport)
#pragma comment(lib, "NetDeviceN9K.lib")
#endif
#else
#define _EXPORT_NETDEV_N9K_SDK  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

//网络模块的初始化和销毁
_EXPORT_NETDEV_N9K_SDK bool NET_DEV_N9KSDK_Initial(tuint16 customerNO=0);
_EXPORT_NETDEV_N9K_SDK void NET_DEV_N9KSDK_Quit();

#endif //_NET_DEVN9K_SDK_H