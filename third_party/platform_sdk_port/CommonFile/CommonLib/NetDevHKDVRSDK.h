
///////////////////////////NetDevHKDVRSDK.h

#ifndef _NET_DEVHKDVR_SDK_H
#define _NET_DEVHKDVR_SDK_H

#ifdef WIN32

#ifdef __HKDVRNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_HKDVR_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_HKDVR_SDK  __declspec(dllimport)
#endif

#else
#define _EXPORT_NETDEV_HKDVR_SDK  __attribute__ ((visibility("default")))
#endif

//网络模块的初始化和销毁
_EXPORT_NETDEV_HKDVR_SDK BOOL NET_DEV_HKDVRSDK_Initial();
_EXPORT_NETDEV_HKDVR_SDK void NET_DEV_HKDVRSDK_Quit();

#endif








