
///////////////////////////EMapSDK.h

#ifndef _NET_EMAP_SDK_H
#define _NET_EMAP_SDK_H

#ifdef WIN32

#ifdef EMAP_SDK_EXPORT_API//动态库编译
#define _EXPORT_EMAP_CLASS __declspec(dllexport)
#define _EXPORT_EMAP_SDK  __declspec(dllexport)
#else
#ifdef EMAP_SDK_EXPORT_STATIC//静态库编译和使用
#define _EXPORT_EMAP_CLASS 
#define _EXPORT_EMAP_SDK 
#else //动态库使用
#define _EXPORT_EMAP_CLASS __declspec(dllimport)
#define _EXPORT_EMAP_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_EMAP_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_EMAP_SDK __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include "EMapIterator.h"

class CEMapUpdateOperator;
class CConfigPack;
class Interlocked;

//网络模块的初始化和销毁
_EXPORT_EMAP_SDK bool EMAP_Initial(Interlocked *pNodeID, const char *szMapPath);
_EXPORT_EMAP_SDK void EMAP_Quit(void);
_EXPORT_EMAP_SDK tuint32 EMAP_GetLastError(void); //返回_ErrorCodeDef.h里面的值

_EXPORT_EMAP_SDK void EMap_ClearAllInfo(void);

_EXPORT_EMAP_SDK void EMap_UpdateInfo(const CConfigPack &ConfigPack);

_EXPORT_EMAP_SDK void EMAP_AddEMAPUpdateObserver(CEMapUpdateOperator *pEMAPUpdateObserver);
_EXPORT_EMAP_SDK void EMAP_DelEMAPUpdateObserver(CEMapUpdateOperator *pEMAPUpdateObserver);

_EXPORT_EMAP_SDK bool EMap_GetAllInfo(CConfigPack &ConfigPack);

#endif //_NET_EMAP_SDK_H








