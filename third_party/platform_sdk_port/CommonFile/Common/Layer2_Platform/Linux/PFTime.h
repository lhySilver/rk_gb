
#ifndef __PF_TIME__
#define __PF_TIME__

#include <time.h>
class CPFTime
{
public:
	/////构造
	CPFTime(void);//构造当前系统时间
	CPFTime(time_t timet);//以time_t表达的UTC时间
	CPFTime(FILETIME ft);//以FILETIME表达的UTC时间
	CPFTime(tint32 nYear, tint32 nMonth, tint32 nDay, tint32 nHour, tint32 nMin, tint32 nSec, bool bUTC = false);	//bUTC:是否UTC时间
	CPFTime(const char *pTime,bool bUTC = false);//时间从2014-05-01 11:11:11  转成time_t
	~CPFTime(void);

	tint32 GetYear(void);//2008这种
	tint32 GetUTCYear(void);//2008这种

	tint32 GetMonth(void);//1-12
	tint32 GetUTCMonth(void);//1-12

	tint32 GetDay(void);//1-31
	tint32 GetUTCDay(void);//1-31

	tint32 GetHour(void);//0-23
	tint32 GetUTCHour(void);//0-23

	tint32 GetMinute(void);//0-59
	tint32 GetUTCMinute(void);//0-59

	tint32 GetSecond(void);//0-59
	tint32 GetUTCSecond(void);//0-59

	tint32 GetDayOfWeek(void);//0-6
	tint32 GetUTCDayOfWeek(void);//0-6

    tint32 GetMilliSecond(void);//0-999

	tuint32 Get100ns(void);		//获取秒后的100纳秒数

	void GetFileTime(FILETIME &ft);//得到格林威治时间的FILETIME,100纳秒为单位,基于1601年1月1日0点
	void GetLocalTime_100ns(tuint64 &ullLocalTime);//得到本地时间,100纳秒为单位,基于1601年1月1日0点
	time_t GetTime();		//UTC时间,基于1970年1月1日0点

	//修改操作系统时间
	bool SetOSLocalTime(tint32 utcYear, tint32 utcMonth, tint32 utcDay, tint32 utcHour, tint32 utcMin, tint32 utcSec);
    tuint64 GetTimeMs();   //UTC时间,基于1970年1月1日0点,毫秒为单位
	// 公历转换波斯历
	static bool GregorianToPersian(tuint16 gYear,tuint16 gMonth,tuint16 gDay,tuint16 &pYear,tuint16 &pMonth,tuint16 &pDay);
private:
	time_t m_time;	//精确到秒,基于1970年1月1日0点
	tuint32 m_mmTime;//精确到秒之后的100纳秒数
};

#endif