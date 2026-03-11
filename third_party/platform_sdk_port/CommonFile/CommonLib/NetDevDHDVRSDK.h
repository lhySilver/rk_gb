
///////////////////////////NetDevDHDVRSDK.h

#ifndef _NET_DEVDHDVR_SDK_H
#define _NET_DEVDHDVR_SDK_H

#include "dhnetsdk.h"

#ifdef WIN32

#ifdef __DHDVRNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_DHDVR_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_DHDVR_SDK  __declspec(dllimport)
#endif

#else
#define _EXPORT_NETDEV_DHDVR_SDK  __attribute__ ((visibility("default")))
#endif

//网络模块的初始化和销毁
_EXPORT_NETDEV_DHDVR_SDK BOOL NET_DEV_DHDVRSDK_Initial();
_EXPORT_NETDEV_DHDVR_SDK void NET_DEV_DHDVRSDK_Quit();

// 异步搜索设备回调(pDevNetInfo内存由SDK内部申请释放)
typedef void (CALLBACK *fSearchDevicesCB)(DEVICE_NET_INFO_EX *pDevNetInfo, void* pUserData);
//开始搜索设备,成功返回句柄，失败返回0
_EXPORT_NETDEV_DHDVR_SDK long NET_DEV_DHDVRSDK_StartSearchDevices(fSearchDevicesCB cbSearchDevices, void* pUserData);
//停止搜索设备
_EXPORT_NETDEV_DHDVR_SDK bool NET_DEV_DHDVRSDK_StopSearchDevices(long lSearchHandle);

#endif








