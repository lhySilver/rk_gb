
#ifndef _EVENT_DICTIONARY_
#define _EVENT_DICTIONARY_

#include "base_type_define.h"
#include "NVMS_Define.h"
#include "NetProtocolDefine_ForNVMS.h"
#include "TypeCheck.h"
#include<list>
#include<map>
#include<string>
#include "Singleton.h"
#include "NVMS_Version.h"
#include "SysHelper.h"
using namespace std;

class CEventDictionary
{
	friend class CSingleton<CEventDictionary>;

protected:
	CEventDictionary(void);
	~CEventDictionary(void){};
public:
	//获取报警事件字典
	void GetAlarmEventDictionary(std::map< tuint32,std::list< std::pair<tuint32, string> > >& mapNodeType_AlarmType);
	//获取任务事件字典
	void GetTaskEventDictionary(std::map< tuint32,std::map< tuint32 , string > >& mapNodeType_TaskType);

	//获取所有日志类型和翻译
	void GetAllLogTypeAndIDCS(std::list< std::pair<tuint32, string> >& mapAllLogTypeAndIDCS );

	//根据日志类型获取日志翻译
	string GetIDCSByLogType(tuint32 nLogType);
	//是否为报警日志
	bool IsAlarmLogType(tuint32 nLogType);
	//是否为操作日志
	bool IsOperateLogType(tuint32 nLogType);
	//是否为配置日志
	bool IsConfigLogType(tuint32 nLogType);
	//是否为信息日志
	bool IsInfoLogType(tuint32 nLogType);
	//是否为异常日志
	bool IsExceptionLogType(tuint32 nLogType);
	//是否为状态日志
	bool IsStatuLogType(tuint32 nLogType);
	//是否为报警开始和停止为非成对出现的
	bool IsNoEndAlarmType(tuint32 nAlarmType);

	//是否为传感器报警
	bool IsSensorAlarmType(tuint32 nAlarmType);
	//是否为监控点报警
	bool IsChannelAlarmType(tuint32 nAlarmType);
	//是否为设备报警(编码器)
	bool IsDeviceAlarmType(tuint32 nAlarmType);
	//是否为门禁
	bool IsDoorAlarmType(tuint32 nAlarmType);
	//是否为考勤
	bool IsAcsAttAlarmType(tuint32 nAlarmType);
	//当前报警类型是否需要配置联动信息--false不需要
	bool IsNeedConfigTriggerInfo(tuint32 nAlarmType);
	//是否是节点下线报警类型
	bool IsNodeOfflineAlarmType(tuint32 nAlarmType);
	//是否是智能分析服务器上报过来的报警
	bool IsAnsReportAlarm(tuint32 nAlarmType);
	//是否是服务器报警
	bool IsServerAlarmType(tuint32 nAlarmType);
	//是否是报警任务
    bool IsAlarmTaskType(tuint32 nAlarmType);
	//是否是防区报警
	bool IsZoneAlarmType(tuint32 nAlarmType);
	//是否是子系统
	bool IsSubSystemAlarmType(tuint32 nAlarmType);
    //是否是停车场报警
    bool IsPmsParkAlarmType(tuint32 nAlarmType);
private:
	bool IsV2_0();
	bool IsV2_1();
	bool IsV2_1_1();
	//大于等于2.1.2的版本
	bool IsHigher2_1_2();
	//大于等于2.2的版本
	bool IsGeV2_2();
	std::map< tuint32,std::list< std::pair<tuint32, string> > > m_mapNodeType_AlarmType;//key:节点类型 值map:<报警类型,翻译IDCS>
	std::map< tuint32,std::map<tuint32 , string > > m_mapNodeType_TaskType;//key:节点类型 值map:<任务类型,翻译IDCS>

	std::list< std::pair<tuint32, string> > m_mapAllLogTypeAndIDCS;////key:日志类型 值:翻译 日志类型由报警类型\配置\操作\信息组成，详见NVMS_Define.h ECMS_ALARM_TYPE
};

#endif
