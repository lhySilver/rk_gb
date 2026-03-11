
#ifndef __DELAY_TIMER__
#define __DELAY_TIMER__
/************************************************************************/
//延迟定时器
/************************************************************************/
#include "Lock.h"
class CDelayTimer
{
public:
	///////////定时器状态
	typedef enum timer_status
	{
		TIMER_NO_ACTION,		//未启动
		TIMER_TIMER_ING,		//计时中
		TIMER_TIMER_END			//计时完成
	}TIMER_STATUS;


	////构造函数
	CDelayTimer();

	///////析构函数
	virtual ~CDelayTimer();

	TIMER_STATUS GetCurStatus();

	/////////////定时  计时   开始计时  单位为毫秒
	bool StartTiming(bool isTimer);/////永不停止
	TIMER_STATUS StopTiming();

	bool StartDelay(ULONGLONG delayus=~0);//开始倒计时
	void StopDelay();
	TIMER_STATUS Delaykeeper(ULONGLONG ullAbsoluteTimems,ULONGLONG &ulTimems);//计数器减少一个单位

	TIMER_STATUS Timekeeper(const ULONGLONG &ullAbsoluteTimeus, ULONGLONG &ullElapseTimeus);
	TIMER_STATUS Timekeeper(const FILETIME  &ftAbsoluteTime100ns,  ULONGLONG &ullElapseTimeus);
private:
	ULONGLONG GetElapseTimeus(const FILETIME &baseTime,const FILETIME &curTime);
	//variable
	CLock   m_Lock;

	bool m_IsTimer;//是定时器模式，每次往前跳多少毫秒，否则为绝对时间模式，每次给绝对时间
	////状态
	TIMER_STATUS    m_Status;
	FILETIME		m_FirstTime;//100纳秒为单位
	/////逝去的微秒数
	ULONGLONG        m_ElapseTimeus;//微秒为单位
	//倒计时计数器
	ULONGLONG			m_Delayus;//微秒为单位
};

#endif //__DELAY_TIMER__