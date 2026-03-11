
#ifndef __UPDATE_OPERATOR__
#define __UPDATE_OPERATOR__
#include "base_type_define.h"
class CLocalNode;
class CConfigPack;

class CUpdateOperator
{
public:
	CUpdateOperator(void) {}
	virtual ~CUpdateOperator(void) {}

	//////添加操作
	virtual void AddItem(const CLocalNode *pNewLocalNode, CConfigPack &configPack) {}

	//////修改操作
	virtual void ModifyItem(const CLocalNode *pOldLocalNode, const CLocalNode *pNewLocalNode, CConfigPack &newNodePack) {}

	//////删除操作
	virtual void DeleteItem(const CLocalNode *pOldLocalNode,  CConfigPack &DelNodePack) {}

	////////物理节点树关系变化通知
	virtual void AddPhysicsNodeTree(const CLocalNode *pParentNode, const CLocalNode *pChildNode, CConfigPack &newNodePack) {}
	virtual void DeletePhysicsNodeTree(const CLocalNode *pParentNode, const CLocalNode *pChildNode, CConfigPack &DelNodePack) {}

	//区域树关系
	virtual void AddAreaTree(const CLocalNode *pParentNode, const CLocalNode *pChildNode, CConfigPack &newNodePack) {}
	virtual void DeleteAreaTree(const CLocalNode *pParentNode, const CLocalNode *pChildNode, CConfigPack &DelNodePack) {}

	//通道录像排程
	virtual void AddChannelRecordSchedule(const CLocalNode *pChannelNode, CConfigPack &NodePack) {}
	virtual void DeleteChannelRecordSchedule(const CLocalNode *pChannelNode, CConfigPack &NodePack) {}

	//通道设置分区组
	virtual void AddChannelRecordPath(const CLocalNode *pChannelNode, CConfigPack &ConfigPack) {}
	virtual void DeleteChannelRecordPath(const CLocalNode *pChannelNode, CConfigPack &ConfigPack) {}

	virtual void UpdateCompleted(void) {}

	virtual void OnAddObserver(void) {}

	//解码器对应的墙
	virtual void AddDecoderWall(const CLocalNode *pDevNode, CConfigPack &NodePack) {}
	virtual void DeleteDecoderWall(const CLocalNode *pDevNode,CConfigPack &NodePack) {}

	//通道组中的通道
	virtual void AddChannelToGroup(const CLocalNode *pChannelGroupNode, CConfigPack &newNodePack){}
	virtual void DelChannelToGroup(const CLocalNode *pChannelGroupNode,  CConfigPack &delNodePack){}

	//报警输出组中的报警输出通道
	virtual void AddAlarmOutToGroup(const CLocalNode *pAlarmOutGroupNode, CConfigPack &newNodePack){}
	virtual void DelAlarmOutToGroup(const CLocalNode *pAlarmOutGroupNode,  CConfigPack &delNodePack){}


	//处警标准操作中的处警动作
	virtual void AddSopActionToSop(const CLocalNode *pSopNode,const char* pSopActionName, CConfigPack &newNodePack){}
	virtual void DelSopActionToSop(const CLocalNode *pSopNode,  CConfigPack &delNodePack){}

	//告警任务组中的任务
	virtual void AddAlarmTaskToGroup(const CLocalNode *pAlarmTaskGroupNode,GUID ResourceID, CConfigPack &newNodePack){}
	virtual void DelAlarmTaskToGroup(const CLocalNode *pAlarmTaskGroupNode,GUID ResourceID, CConfigPack &delNodePack){}

	//人脸检测任务
	//virtual void AddChannelVFDTaskSchedule(const CLocalNode *pChannelNode, CConfigPack &NodePack) {}
	//virtual void DeleteChannelVFDTaskSchedule(const CLocalNode *pChannelNode, CConfigPack &NodePack) {}
	
	//通用报警排程
	virtual void AddNodeAlarmTypeSchedule(const CLocalNode *pChannelNode, tuint32 nAlarmType, CConfigPack &NodePack) {}
	virtual void DeleteNodeAlarmTypeSchedule(const CLocalNode *pChannelNode, tuint32 nAlarmType, CConfigPack &NodePack) {}
	
	//通用任务排程
	virtual void AddNodeTaskTypeSchedule(const CLocalNode *pChannelNode, tuint32 nTaskType, CConfigPack &NodePack) {}
	virtual void DeleteNodeTaskTypeSchedule(const CLocalNode *pChannelNode, tuint32 nTaskType, CConfigPack &NodePack) {}

	//pms相关 车道和摄像机或显示屏的绑定关系
	virtual void AddBindDevice(const CLocalNode *pNode,  CConfigPack &NodePack) {}
	virtual void DelBindDevice(const CLocalNode *pNode,  CConfigPack &NodePack) {}
};

#endif //__UPDATE_OPERATOR__