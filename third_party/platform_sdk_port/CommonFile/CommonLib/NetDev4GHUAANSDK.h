
#ifndef _NET_DEV4GHUAAN_SDK_H
#define _NET_DEV4GHUAAN_SDK_H

#ifdef WIN32
#ifdef __4GHUAANNETPROTOCOL_DLL__//动态库编译 
#define _EXPORT_NETDEV_4GHUAAN_SDK  __declspec(dllexport)
#else 
#define _EXPORT_NETDEV_4GHUAAN_SDK  __declspec(dllimport)
#endif
#define CALLBACK __stdcall
#else
#define _EXPORT_NETDEV_4GHUAAN_SDK  __attribute__ ((visibility("default")))
#define CALLBACK
#endif

#include "base_type_define.h"
#ifndef __ENVIRONMENT_MAC__
#include "HA_CameraParam.h"
#include "module_base.h"

typedef void (CALLBACK *fun_on_device_status_changed)(
	device_status_t status, 
	device_handle_t handle,
	void *user_data,
	char *pSnCode,
	tint32 iSnLength);
typedef void (CALLBACK *fun_on_device_event)(
	device_handle_t handle,
	const device_event_t *event,
	void *user_data);
struct huaan_module_callbacks_t {
	tint32 iHuaanAutoPort;
	fun_on_device_status_changed fDeviceChanged;
	fun_on_device_event fDeviceEvent;
};
//网络模块的初始化和销毁
_EXPORT_NETDEV_4GHUAAN_SDK bool NET_DEV_4GHUAANSDK_Initial(huaan_module_callbacks_t cbHuaan);
_EXPORT_NETDEV_4GHUAAN_SDK void NET_DEV_4GHUAANSDK_Quit();
_EXPORT_NETDEV_4GHUAAN_SDK void NET_DEV_4GHUAANSDK_Startup();
_EXPORT_NETDEV_4GHUAAN_SDK void NET_DEV_4GHUAANSDK_Shutdown();
#endif
#endif //_NET_DEVN9K_SDK_H
