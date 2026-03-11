#ifndef _NET_DEV_THINKFORCE_SDK_H
#define _NET_DEV_THINKFORCE_SDK_H

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

_EXPORT_NETDEV_IPC_SDK bool NET_DEV_THINKFORCESDK_Initial();
_EXPORT_NETDEV_IPC_SDK void NET_DEV_THINKFORCESDK_Quit();

#endif








