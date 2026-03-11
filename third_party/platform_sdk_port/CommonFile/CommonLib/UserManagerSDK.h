
///////////////////////////UserManagerDll.h

#ifndef _USER_MANAGER_DLL_H
#define _USER_MANAGER_DLL_H

#ifdef WIN32 
#ifdef USER_MANAGER_SDK_EXPORT_STATIC//静态库编译
#define _EXPORT_USERMANAGER_CLASS
#define _EXPORT_USERMANAGER_SDK
#else 
#ifdef USER_MANAGER_DLL_EXPORT_API//动态库编译
#define _EXPORT_USERMANAGER_CLASS __declspec(dllexport)
#define _EXPORT_USERMANAGER_SDK  __declspec(dllexport)
#else//使用者编译
#define _EXPORT_USERMANAGER_CLASS __declspec(dllimport)
#define _EXPORT_USERMANAGER_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_USERMANAGER_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_USERMANAGER_SDK __attribute__ ((visibility("default")))
#endif

#include "UserIterator.h"
#include "UpdateCfgPack_No_Operator.h"

class CConfigPack;
class Interlocked;

_EXPORT_USERMANAGER_SDK bool USER_MANAGER_Initial(Interlocked *pNodeID);
_EXPORT_USERMANAGER_SDK void USER_MANAGER_Quit(void);

_EXPORT_USERMANAGER_SDK void USER_MANAGER_ClearNodeInfo(void);

_EXPORT_USERMANAGER_SDK void USER_MANAGER_UpdateNodeInfo(const CConfigPack &ConfigPack);//资源更新
_EXPORT_USERMANAGER_SDK bool USER_MANAGER_GetAllUserInfo(CConfigPack &ConfigPack);

_EXPORT_USERMANAGER_SDK void USER_MANAGER_AddUpdateObserver(CUpdateCfgPack_No_Operator *pUpdateOperator);//资源更新观察者
_EXPORT_USERMANAGER_SDK void USER_MANAGER_DelUpdateObserver(CUpdateCfgPack_No_Operator *pUpdateOperator);//资源更新观察者

#endif //_USER_MANAGER_DLL_H








