
#ifndef __STATE_OBSERVALBE__
#define __STATE_OBSERVALBE__

//////////包含头文件 
#include "NetProtocolDefine_ForNVMS.h"
#include "base_type_define.h"


class CConnectStateObserver
{
public:
	CConnectStateObserver(void){};
public:
	virtual ~CConnectStateObserver(void){};

	virtual void UpdateState(bool isLocal,tuint32 nodeType,GUID nodeID, bool connectState)=0;
};

class CRecordStateObserver
{
public:
	CRecordStateObserver(void){};
public:
	virtual ~CRecordStateObserver(void){};

	virtual void UpdateRecordState(bool isLocal, GUID storageID, GUID chID, tuint32 RecordState)=0;
};

class CDevRecordStateObserver
{
public:
	CDevRecordStateObserver(void){};
public:
	virtual ~CDevRecordStateObserver(void){};

	virtual void UpdateDevRecordState(bool isLocal, GUID chID, tuint32 RecordState)=0;
};

class ALARM_INDEX;
class CAlarmStateObserver
{
public:
	CAlarmStateObserver(void){};
public:
	virtual ~CAlarmStateObserver(void){};

	virtual void UpdateState(bool isLocal,tuint32 nodeType,ALARM_INDEX alarmIndex, BYTE AlarmState,const NVMS_NET_PROTOCOL::ECMS_ALARM_INFO AlarmInfo)=0;
};

//定义报警索引对象，以节点、报警类型、索引号来唯一确定一个索引
class ALARM_INDEX
{
public:
	ALARM_INDEX(GUID nodeID, tuint32 alarmType)
	{
		m_NodeID=nodeID;
		m_AlarmType=alarmType;
	};
	virtual ~ALARM_INDEX(){};

	//////////////////////////////////////////////////////////////////////////////
	friend __inline bool operator == (const ALARM_INDEX& alarm1,const ALARM_INDEX& alarm2)
	{
		if((alarm1.m_NodeID == alarm2.m_NodeID) && (alarm1.m_AlarmType == alarm2.m_AlarmType) )
			return true;
		else 
			return false;
	}

	friend __inline bool operator < (const ALARM_INDEX& alarm1,const ALARM_INDEX& alarm2)
	{
		if(alarm1.m_NodeID < alarm2.m_NodeID)
			return true;
		else if(alarm1.m_NodeID > alarm2.m_NodeID)
			return false;
		else 
		{
			if(alarm1.m_AlarmType < alarm2.m_AlarmType)
				return true;
			else if(alarm1.m_AlarmType > alarm2.m_AlarmType)
				return false;
			else
			{
				return false;
			}
		}
	}

	friend __inline bool operator > (const ALARM_INDEX& alarm1,const ALARM_INDEX& alarm2)
	{
		if(alarm1.m_NodeID > alarm2.m_NodeID)
			return true;
		else if(alarm1.m_NodeID < alarm2.m_NodeID)
			return false;
		else 
		{
			if(alarm1.m_AlarmType > alarm2.m_AlarmType)
				return true;
			else if(alarm1.m_AlarmType < alarm2.m_AlarmType)
				return false;
			else
			{
				return false;
			}
		}
	}
	GUID m_NodeID;//设备节点
	tuint32  m_AlarmType;//报警类型
	ALARM_INDEX()
	{
		m_NodeID = GUID_NULL;
		m_AlarmType = 0;
	}
private:
};

class CSystemDoorStateObserver
{
public:
	CSystemDoorStateObserver(void){};
public:
	virtual ~CSystemDoorStateObserver(void){};

	virtual void UpdateSystemDoorState(const void *pFrameData, tuint32 frameDataLen)=0;//更新门禁系统的门的状态
};

#endif //__STATE_OBSERVALBE__