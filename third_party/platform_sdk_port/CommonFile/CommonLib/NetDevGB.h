
///////////////////////////NetDevGB.h

#ifndef _NET_DEV_GB_H
#define _NET_DEV_GB_H

#ifdef WIN32
#ifdef __GBDEV_NETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_GB_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_GB_SDK  __declspec(dllimport)
#pragma comment(lib, "NetDeviceGB.lib")
#endif
#else
#define _EXPORT_NETDEV_GB_SDK  __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include "map"

typedef void (*GBDevReport_Callback)(tuint32 devID);

//网络模块的初始化和销毁
_EXPORT_NETDEV_GB_SDK bool NET_DEV_GB_Initial(tuint16 customerNO=0);
_EXPORT_NETDEV_GB_SDK void NET_DEV_GB_Quit();
_EXPORT_NETDEV_GB_SDK void NET_GET_GBPlat_Status(std::map<GUID, bool> &platStatusMap);//获取下级平台在现场状态
_EXPORT_NETDEV_GB_SDK void NET_SET_GBDevReport_CallbackFunc(GBDevReport_Callback func);//获取下级平台在现场状态
_EXPORT_NETDEV_GB_SDK bool NET_DEV_GB_SyncLowerCascadePlatformSrc(GUID platformGUID);//手动同步下级平台资源
#endif //_NET_DEV_GB_H