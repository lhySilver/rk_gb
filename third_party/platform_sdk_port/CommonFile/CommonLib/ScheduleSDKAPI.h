
///////////////////////////EMapSDK.h

#ifndef _NET_SCHEDULE_SDK_H
#define _NET_SCHEDULE_SDK_H

#ifdef WIN32

#ifdef SCHEDULE_SDK_EXPORT_STATIC//静态库编译
#define _EXPORT_SCHEDULE_CLASS 
#define _EXPORT_SCHEDULE_SDK 
#else
#ifdef SCHEDULE_SDK_EXPORT_API//动态库编译
#define _EXPORT_SCHEDULE_CLASS __declspec(dllexport)
#define _EXPORT_SCHEDULE_SDK  __declspec(dllexport)
#else
#define _EXPORT_SCHEDULE_CLASS __declspec(dllimport)
#define _EXPORT_SCHEDULE_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_SCHEDULE_SDK   __attribute__ ((visibility("default")))
#define _EXPORT_SCHEDULE_CLASS __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include "ScheduleIterator.h"
#include "UpdateCfgPack_No_Operator.h"
class CConfigPack;
class Interlocked;

//网络模块的初始化和销毁
_EXPORT_SCHEDULE_SDK bool SCHE_Initial(Interlocked *pNodeID);
_EXPORT_SCHEDULE_SDK void SCHE_Quit();
_EXPORT_SCHEDULE_SDK tuint32 SCHE_GetLastError(); //返回_ErrorCodeDef.h里面的值

_EXPORT_SCHEDULE_SDK void SCHE_AddObserver(CUpdateCfgPack_No_Operator *pObserver);
_EXPORT_SCHEDULE_SDK void SCHE_DeleteObserver(CUpdateCfgPack_No_Operator *pDelObserver);
_EXPORT_SCHEDULE_SDK void SCHE_ClearAllInfo();
_EXPORT_SCHEDULE_SDK bool SCHE_GetAllNodeInfo(CConfigPack &ConfigPack);

//_EXPORT_SCHEDULE_SDK bool SCHE_IsInSchedule(tuint32 sheduleID);
_EXPORT_SCHEDULE_SDK bool SCHE_IsInSchedule(const GUID &ScheduleGUID,const tuint64 nowTime=0);

_EXPORT_SCHEDULE_SDK void SCHE_UpdateInfo(const CConfigPack &ConfigPack);

#endif








