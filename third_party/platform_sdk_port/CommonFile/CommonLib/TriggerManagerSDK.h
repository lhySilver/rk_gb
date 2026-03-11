
///////////////////////////TriggerManagerSDK.h

#ifndef _TRIGGER_MANAGER_DLL_H
#define _TRIGGER_MANAGER_DLL_H

#ifdef WIN32


#ifdef TRIGGER_MANAGER_DLL_EXPORT_API//动态库编译
#define _EXPORT_TRIGGERMANAGER_CLASS __declspec(dllexport)
#define _EXPORT_TRIGGERMANAGER_SDK  __declspec(dllexport)
#else
#ifdef TRIGGER_MANAGER_SDK_EXPORT_STATIC//静态库编译和使用
#define _EXPORT_TRIGGERMANAGER_CLASS
#define _EXPORT_TRIGGERMANAGER_SDK
#else//使用者编译
#define _EXPORT_TRIGGERMANAGER_CLASS __declspec(dllimport)
#define _EXPORT_TRIGGERMANAGER_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_TRIGGERMANAGER_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_TRIGGERMANAGER_SDK  __attribute__ ((visibility("default")))
#endif

#include "TriggerNode.h"
#include "AlarmTriggerIterator.h"

class CConfigPack;
class Interlocked;

class CTrigggerUpdate
{
public:
	CTrigggerUpdate(void) {}
	virtual ~CTrigggerUpdate(void) {}

	virtual void AddAlarmTrigger(CTriggerNode *pTriggerNode) = 0;
	virtual void DelAlarmTrigger(CTriggerNode *pTriggerNode) = 0;
	virtual void TriggerUpdateCompleted(void) {}
};

_EXPORT_TRIGGERMANAGER_SDK bool TRIGGER_MANAGER_Initial(Interlocked *pNodeID);
_EXPORT_TRIGGERMANAGER_SDK void TRIGGER_MANAGER_Quit(void);

_EXPORT_TRIGGERMANAGER_SDK void TRIGGER_MANAGER_AddTriggerUpdateObserver(CTrigggerUpdate *pTriggerUpdate);//添加观察者
_EXPORT_TRIGGERMANAGER_SDK void TRIGGER_MANAGER_DelTriggerUpdateObserver(CTrigggerUpdate *pTriggerUpdate);//删除观察者

_EXPORT_TRIGGERMANAGER_SDK void TRIGGER_MANAGER_ClearInfo(void);					//清除所有
_EXPORT_TRIGGERMANAGER_SDK void TRIGGER_MANAGER_UpdateInfo(const CConfigPack &ConfigPack);	//更新信息

_EXPORT_TRIGGERMANAGER_SDK bool TRIGGER_MANAGER_GetAllTriggerInfo(CConfigPack &ConfigPack);//得到所有节点信息
_EXPORT_TRIGGERMANAGER_SDK bool TRIGGER_MANAGER_GetAllTriggerInfo_Ex(CConfigPack &ConfigPack);//得到所有节点信息(网络协议版本6用到)

#endif








