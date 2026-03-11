
///////////////////////////FileSDKAPI.h

#ifndef _FILE_SDK_H
#define _FILE_SDK_H

#ifdef WIN32

#ifdef FILE_SDK_EXPORT_STATIC//静态库编译
#define _EXPORT_FILE_CLASS 
#define _EXPORT_FILE_SDK 
#else
#ifdef FILE_SDK_EXPORT_API//动态库编译
#define _EXPORT_FILE_CLASS __declspec(dllexport)
#define _EXPORT_FILE_SDK  __declspec(dllexport)
#else
#define _EXPORT_FILE_CLASS __declspec(dllimport)
#define _EXPORT_FILE_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_FILE_SDK   __attribute__ ((visibility("default")))
#define _EXPORT_FILE_CLASS __attribute__ ((visibility("default")))
#endif

#include "base_type_define.h"
#include "CommonFileIterator.h"
#include "UpdateCfgPack_No_Operator.h"
class CConfigPack;
class Interlocked;

//网络模块的初始化和销毁
_EXPORT_FILE_SDK bool ComFile_Initial(Interlocked *pNodeID, const char *szFilePath);
_EXPORT_FILE_SDK void ComFile_Quit();
_EXPORT_FILE_SDK tuint32 ComFile_GetLastError(); //返回_ErrorCodeDef.h里面的值

_EXPORT_FILE_SDK void ComFile_AddObserver(CUpdateCfgPack_No_Operator *pObserver);
_EXPORT_FILE_SDK void ComFile_DeleteObserver(CUpdateCfgPack_No_Operator *pDelObserver);
_EXPORT_FILE_SDK void ComFile_ClearAllInfo();
_EXPORT_FILE_SDK bool ComFile_GetAllNodeInfo(CConfigPack &ConfigPack);

_EXPORT_FILE_SDK void ComFile_UpdateInfo(const CConfigPack &ConfigPack);

#endif//_FILE_SDK_H








