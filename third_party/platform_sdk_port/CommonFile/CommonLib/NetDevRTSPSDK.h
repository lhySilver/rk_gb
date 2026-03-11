
///////////////////////////NetDevRTSPSDK.h

#ifndef _NET_DEV_RTSP_SDK_H
#define _NET_DEV_RTSP_SDK_H

#ifdef WIN32 

#ifdef __IPCHUAANPROTOCOL_DLL__//动态库编译 
#define _EXPORT_DEV_RTSP_SDK  __declspec(dllexport)
#else 
#define _EXPORT_DEV_RTSP_SDK  __declspec(dllimport)
#pragma comment(lib, "RtspClient.lib")
#endif
#else
#define _EXPORT_DEV_RTSP_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"

_EXPORT_DEV_RTSP_SDK bool NET_Dev_RTSP_SDK_Initial();/////支持的最低版本号限制
_EXPORT_DEV_RTSP_SDK void NET_Dev_RTSP_SDK_Quit();

#endif
