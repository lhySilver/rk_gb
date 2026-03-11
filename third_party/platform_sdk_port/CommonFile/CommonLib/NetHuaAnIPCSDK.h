
///////////////////////////NetHuaAnIPCSDK.h

#ifndef _NET_HUAANIPC_SDK_H
#define _NET_HUAANIPC_SDK_H

#ifdef WIN32 

#ifdef __IPCHUAANPROTOCOL_DLL__//动态库编译 
#define _EXPORT_HUAAN_IPC_SDK  __declspec(dllexport)
#else 
#define _EXPORT_HUAAN_IPC_SDK  __declspec(dllimport)
#pragma comment(lib, "NetHuaAnIPC.lib")
#endif
#else
#define _EXPORT_HUAAN_IPC_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

_EXPORT_HUAAN_IPC_SDK bool NET_HUAAN_IPCSDK_Initial();/////支持的最低版本号限制
_EXPORT_HUAAN_IPC_SDK void NET_HUAAN_IPCSDK_Quit();

#endif
