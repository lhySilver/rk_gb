
///////////////////////////NodeManagerDll.h

#ifndef _NODE_MANAGER_DLL_H
#define _NODE_MANAGER_DLL_H

#ifdef WIN32

#ifdef NODE_MANAGER_DLL_EXPORT_API//动态库编译
#define _EXPORT_NODEMANAGER_CLASS __declspec(dllexport)
#define _EXPORT_NODEMANAGER_SDK  __declspec(dllexport)
#else
#ifdef NODE_MANAGER_SDK_STATIC//静态库编译和引用
#define _EXPORT_NODEMANAGER_CLASS
#define _EXPORT_NODEMANAGER_SDK
#else 
#define _EXPORT_NODEMANAGER_CLASS __declspec(dllimport)
#define _EXPORT_NODEMANAGER_SDK  __declspec(dllimport)
#endif
#endif

#else
#define _EXPORT_NODEMANAGER_CLASS __attribute__ ((visibility("default")))
#define _EXPORT_NODEMANAGER_SDK 	__attribute__ ((visibility("default")))
#endif 

#include "LocalNode.h"
#include "Iterator.h"
#include "Observer.h"
#include "ConfigPack_Data_Item_ID_Def.h"
#include <string>

class CConfigPack;
class CUpdateOperator;
class Interlocked;

//const tuint32 FIRST_PARTITION_GROUP_ID =1 ;//每个存储服务器创建的第一个分区组ID，其它分区组一直往上加，

// enum NODEMANAGER_ERROR_CODE
// {
// 	NODEMANAGER_ERROR_SUCCESS=0x0,
// 	NODEMANAGER_ERROR_BASE=0x20000000, //用户错误都需要这个位为1
// };

//网络模块的初始化和销毁
_EXPORT_NODEMANAGER_SDK bool NODE_MANAGER_Initial(Interlocked *pNodeID);
_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_Quit(void);

_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_ClearNodeInfo(void);
_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_UpdateNodeInfo(const CConfigPack &ConfigPack);//节点资源更新观察者

_EXPORT_NODEMANAGER_SDK bool NODE_MANAGER_AddNodeUpdateObserver(CUpdateOperator *pUpdateOperator);//节点资源更新观察者
_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_DelNodeUpdateObserver(CUpdateOperator *pUpdateOperator);//节点资源更新观察者

_EXPORT_NODEMANAGER_SDK bool NODE_MANAGER_GetAllNodeInfo(CConfigPack &ConfigPack);//得到所有节点信息

_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_UpdateOtherInfo(CConfigPack *pNodeInfoPack);//公共资源更新观察者
_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_AddOtherUpdateObserver(CObserver<CConfigPack *> *pObserver);//除节点之外的公共资源更新观察者
_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_DelOtherUpdateObserver(CObserver<CConfigPack *> *pObserver);//除节点之外的公共资源更新观察者

_EXPORT_NODEMANAGER_SDK bool NODE_MANAGER_SetServiceIPPort(GUID nodeID, const std::string *pInIP,const std::string *pOutIP,int ipcount, tuint16 port, CConfigPack *pConfigPack);
//_EXPORT_NODEMANAGER_SDK bool NODE_MANAGER_GetNodeCfgInfo(tuint16 itemID, CConfigPack &ConfigPack,tuint32 nodeID);

_EXPORT_NODEMANAGER_SDK bool NODE_MANAGER_GetConfigPackByID(CFGPACK_DEFINE::DATA_ITEM_ID ItemID, CConfigPack &ConfigPack);

_EXPORT_NODEMANAGER_SDK void NODE_MANAGER_ExplainCfgPack(CConfigPack *pConfigPack, CUpdateOperator *pObserver);			//配置包,解析结果更新对象

_EXPORT_NODEMANAGER_SDK GUID NODE_MANAGER_GetRSUNodeGUID();//得到唯一的一个认证服务器的节点ID
_EXPORT_NODEMANAGER_SDK GUID NODE_MANAGER_GetADUNodeGUID();//得到唯一的一个报警服务器的节点ID
_EXPORT_NODEMANAGER_SDK GUID NODE_MANAGER_GetEMapNodeGUID();//得到唯一的一个电子地图服务器的节点ID

#endif








