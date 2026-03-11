
///////////////////////////NetDevMDVRSDK.h

#ifndef _NET_DEVIPC_SDK_H
#define _NET_DEVIPC_SDK_H

#ifdef WIN32 

#ifdef __IPCNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_IPC_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_IPC_SDK  __declspec(dllimport)
#endif
#else
#define _EXPORT_NETDEV_IPC_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

_EXPORT_NETDEV_IPC_SDK bool NET_DEV_IPCSDK_Initial();/////支持的最低版本号限制
_EXPORT_NETDEV_IPC_SDK void NET_DEV_IPCSDK_Quit();

#endif








