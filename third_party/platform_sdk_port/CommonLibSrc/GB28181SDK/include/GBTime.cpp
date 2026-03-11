
#include<time.h>

#include "GBTime.h"
#ifndef WIN32
#include <sys/time.h>
#endif

CGBTime::CGBTime(void)
{
	tzset();
#ifdef WIN32
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	struct tm tmp;
	memset(&tmp,0,sizeof(tmp));
	tmp.tm_year=sysTime.wYear-1900;
	tmp.tm_mon=sysTime.wMonth-1;
	tmp.tm_mday=sysTime.wDay;
	tmp.tm_hour=sysTime.wHour;
	tmp.tm_min=sysTime.wMinute;
	tmp.tm_sec=sysTime.wSecond;
	tmp.tm_isdst = -1;				//必须为-1,否则在夏令时区会快一个小时
	m_time=mktime(&tmp);
	m_mmTime=sysTime.wMilliseconds;
	m_mmTime*=10000;
#else
	timeval tp;
	gettimeofday(&tp, NULL);
	m_time=tp.tv_sec;
#endif

}

CGBTime::CGBTime(time_t timet)//以time_t表达的时间
{
	tzset();
	m_time=timet;
	m_mmTime=0;
}

CGBTime::CGBTime(const FILETIME &ft)//以FILETIME表达的时间
{
	tzset();
	tuint64 ull = 0;
	memcpy(&ull,&ft,sizeof(tuint64));
	m_mmTime=ull%10000000;
	m_time=(ull-116444736000000000ULL)/10000000;
}

CGBTime::CGBTime(tint32 nYear, tint32 nMonth, tint32 nDay, tint32 nHour, tint32 nMin, tint32 nSec, bool bUTC/* = false*/)
{
	tzset();
	struct tm tmp;
	memset(&tmp,0,sizeof(tmp));
	tmp.tm_year=nYear-1900;
	tmp.tm_mon=nMonth-1;
	tmp.tm_mday=nDay;
	tmp.tm_hour=nHour;
	tmp.tm_min=nMin;
	tmp.tm_sec=nSec;
	tmp.tm_isdst = -1;

	if (bUTC)
	{
#ifdef WIN32
		m_time = _mkgmtime(&tmp);					//tmp为UTC时间,与时区无关
#else 
		m_time = timegm(&tmp);
#endif
	}
	else
	{
		m_time = mktime(&tmp);						//tmp为本地时间,与时区相关
	}

	m_mmTime=0;
}

CGBTime::CGBTime(const char *pTime,bool bUTC/* = false*/)//时间从2014-05-01T11:11:11  转成time_t
{
	tzset();
	struct tm tmp;
	memset(&tmp,0,sizeof(tmp));
	sscanf(pTime, "%4d-%2d-%2dT%2d:%2d:%2d",     
		&tmp.tm_year,  
		&tmp.tm_mon,  
		&tmp.tm_mday,  
		&tmp.tm_hour,  
		&tmp.tm_min,  
		&tmp.tm_sec);               
	tmp.tm_year -= 1900;  
	tmp.tm_mon --;  
	tmp.tm_isdst=-1;  

	if (bUTC)
	{
#ifdef WIN32
		m_time = _mkgmtime(&tmp);					//tmp为UTC时间,与时区无关
#else 
		m_time = timegm(&tmp);
#endif
	}
	else
	{
		m_time = mktime(&tmp);						//tmp为本地时间,与时区相关
	}

	m_mmTime=0;
}

CGBTime::~CGBTime(void)
{
}

tint32 CGBTime::GetYear(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	localtime_s(&ptm,&m_time);
#else 
	localtime_r(&m_time,&ptm); 
#endif
	
	return 1900+ptm.tm_year;
}

tint32 CGBTime::GetUTCYear(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif

	return 1900+ptm.tm_year;
}

tint32 CGBTime::GetMonth()
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	localtime_s(&ptm,&m_time);
#else 
	localtime_r(&m_time,&ptm); 
#endif
	return ptm.tm_mon+1;
}

tint32 CGBTime::GetUTCMonth(void)
{
    struct tm ptm;
    memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
    gmtime_s(&ptm,&m_time);
#else
    gmtime_r(&m_time,&ptm);
#endif
    return ptm.tm_mon+1;
}

tint32 CGBTime::GetDay(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif
	return ptm.tm_mday;
}

tint32 CGBTime::GetUTCDay(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif 
	return ptm.tm_mday;
}

tint32 CGBTime::GetHour(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	localtime_s(&ptm,&m_time);
#else 
	localtime_r(&m_time,&ptm); 
#endif
	return ptm.tm_hour;
}

tint32 CGBTime::GetUTCHour(void)
{
	struct tm ptm;
	memset(&ptm, 0, sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif 
	return ptm.tm_hour;
}

tint32 CGBTime::GetMinute(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	localtime_s(&ptm,&m_time);
#else 
	localtime_r(&m_time,&ptm); 
#endif 
	return ptm.tm_min;
}

tint32 CGBTime::GetUTCMinute(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif 
	return ptm.tm_min;
}

tint32 CGBTime::GetSecond(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	localtime_s(&ptm,&m_time);
#else 
	localtime_r(&m_time,&ptm); 
#endif
	return ptm.tm_sec;
}

tint32 CGBTime::GetUTCSecond(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif 
	return ptm.tm_sec;
}

tint32 CGBTime::GetDayOfWeek(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	localtime_s(&ptm,&m_time);
#else 
	localtime_r(&m_time,&ptm); 
#endif 
	return ptm.tm_wday;
}

tint32 CGBTime::GetUTCDayOfWeek(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
#ifdef WIN32
	gmtime_s(&ptm,&m_time); 
#else 
	gmtime_r(&m_time,&ptm);  
#endif 
	return ptm.tm_wday;
}

tuint32 CGBTime::Get100ns(void)
{
	return m_mmTime;
}

void CGBTime::GetFileTime(FILETIME &ft)//得到格林威治时间的FILETIME
{	
	tuint64 ull = (tuint32)m_time;
	ull *= 10000000;
	ull += 116444736000000000ULL;
	ull += m_mmTime;
	memcpy(&ft,&ull,sizeof(tuint64));
}

void CGBTime::GetLocalTime_100ns(tuint64 &ullLocalTime)//得到本地时间的FILETIME
{
#ifdef WIN32
	FILETIME   ft;
	GetFileTime(ft)	;
	FILETIME localFt;
	FileTimeToLocalFileTime(&ft, &localFt);
	memcpy(&ullLocalTime,&localFt,sizeof(tuint64));	
#else 
	tzset();
	tuint64 dst=timezone;
	tuint64 ull = (tuint64)m_time * 10000000 + 116444736000000000ULL;
	ull += m_mmTime;
	ull +=dst*10000000;
	ullLocalTime=ull;
#endif
}

time_t CGBTime::GetTime()
{
	return m_time;
}

//修改操作系统时间
bool CGBTime::SetOSLocalTime(tint32 utcYear, tint32 utcMonth, tint32 utcDay, tint32 utcHour, tint32 utcMin, tint32 utcSec)
{
#ifdef WIN32
	SYSTEMTIME systime;
	memset(&systime, 0, sizeof(SYSTEMTIME));
	GetSystemTime(&systime);
	systime.wYear = utcYear;
	systime.wMonth = utcMonth;
	systime.wDay = utcDay;
	systime.wHour = utcHour;
	systime.wMinute = utcMin;
	systime.wSecond = utcSec;
	if (::SetSystemTime(&systime) == 0)
	{
		return false;
	}
	return true;
#else 
	struct tm tmp;
	memset(&tmp,0,sizeof(tmp));
	tmp.tm_year=utcYear-1900;
	tmp.tm_mon=utcMonth-1;
	tmp.tm_mday=utcDay;
	tmp.tm_hour=utcHour;
	tmp.tm_min=utcMin;
	tmp.tm_sec=utcSec;
	tmp.tm_isdst = -1;

	time_t timep = timegm(&tmp);

	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv,&tz);//获取时区保存在tz中
	tv.tv_sec = timep;
	tv.tv_usec = 0;
	if(settimeofday (&tv, &tz) < 0)
	{
		return false;
	}

	return true; 
#endif
	return false;
}

std::string CGBTime::GetStr()
{
	char tmp[128] = {0};
	struct tm time;
	memset(&time,0,sizeof(time));
#ifdef WIN32
	localtime_s(&time,&m_time);
#else 
	localtime_r(&m_time,&time); 
#endif
	sprintf(tmp, "%04d-%02d-%02dT%02d:%02d:%02d",     
		time.tm_year+1900,  
		time.tm_mon+1,  
		time.tm_mday,  
		time.tm_hour,  
		time.tm_min,  
		time.tm_sec);  

	return tmp;
}
