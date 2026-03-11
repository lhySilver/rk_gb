
#ifndef __TRIGGER_NODE__
#define __TRIGGER_NODE__

#include "TriggerManagerSDK.h"
#include "ConfigPack_Data_Item_ID_Def.h"

class CConfigPack;

class _EXPORT_TRIGGERMANAGER_CLASS CTriggerNode
{
public:
	virtual tuint32 GetNodeID(void) const = 0;					//获取资源节点的ID
	virtual const GUID &GetNodeGUID(void) const = 0;

	virtual const GUID &GetAlarmNodeID(void) const = 0;			//获取产生报警的节点ID

	virtual tuint32 GetAlarmType(void) const = 0;

	virtual const GUID &GetTriggerNodeID(void) const = 0;		//获取联动对象的节点ID
	virtual tuint32 GetTriggerType(void) const = 0;

	virtual bool GetTriggerInfoEx(char *buf, int buflen) const = 0;

	virtual void GetAlarmTriggerInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperateType) const = 0;

protected:
	CTriggerNode(void) {}
	virtual ~CTriggerNode(void) {}
};

#endif //__TRIGGER_NODE__