
///////////////////////////NetDevDVRSDK.h

#ifndef _NET_DEVDVR_SDK_H
#define _NET_DEVDVR_SDK_H

#ifdef WIN32
#ifdef __DVRNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_DVR_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_DVR_SDK  __declspec(dllimport)
#endif
#else
#define _EXPORT_NETDEV_DVR_SDK  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

//网络模块的初始化和销毁
_EXPORT_NETDEV_DVR_SDK bool NET_DEV_DVRSDK_Initial(tuint16 customerNO=0);
_EXPORT_NETDEV_DVR_SDK void NET_DEV_DVRSDK_Quit();

#endif








