
///////////////////////////DEVICESEARCH.h

#ifndef _DEVICE_SEARCH_SDK_H
#define _DEVICE_SEARCH_SDK_H

#ifdef DEVICE_SEARCH_SDK_EXPORT_API//动态库编译
#define _EXPORT_DEVICE_SEARCH_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_DEVICE_SEARCH_SDK __declspec(dllimport)
#endif

#define DEV_SEARCH_ADD    1 //设备上线
#define DEV_SEARCH_DEC    2 //设备下线
#define DEV_SEARCH_UPDATE 3 //设备信息变化了

_EXPORT_DEVICE_SEARCH_SDK BOOL DEV_SEARCH_Start(void(CALLBACK *HandleDataCallBack)(char* hwaddr,char* praddr,char* subnetmask,char *route,char *dns1,char *dns2,unsigned int dev_type,unsigned int port, char* sfwversion, int opt));
_EXPORT_DEVICE_SEARCH_SDK BOOL DEV_SET_CONFIG(char* hwaddr,char* praddr,char* subnetmask,char *route,char *dns1,char *dns2,char *pPwd);
_EXPORT_DEVICE_SEARCH_SDK void DEV_SEARCH_Stop(void);			

#endif