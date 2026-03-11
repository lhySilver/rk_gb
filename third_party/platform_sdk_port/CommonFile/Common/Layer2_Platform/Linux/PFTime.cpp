
#include<time.h>
#include<sys/time.h>
#include "base_type_define.h"
#include "PFTime.h"

class stDateData
{
public:
	tuint16 year;
	tuint8 month;
	tuint8 day;

	stDateData()
	{
		reset(0,0,0);
	}
	stDateData(tuint16 y,tuint8 m,tuint8 d)
	{
		reset(y,m,d);
	}

	void reset(tuint16 y,tuint8 m,tuint8 d)
	{
		year = y;
		month = m;
		day = d;
	}
	bool operator < (const stDateData &d) const
	{
		return encode() < d.encode();
	}
	bool operator > (const stDateData &d) const
	{
		return encode() > d.encode();
	}
	bool operator == (const stDateData &d) const
	{
		return encode() == d.encode();
	}
	bool operator != (const stDateData &d) const
	{
		return encode() != d.encode();
	}
private:
	tuint32 encode() const
	{
		tuint32 d = ((year << 16) & 0xFFFF0000) | ((month << 8) & 0xFF00) | day;
		return d;
	}

	void decode(tuint32 d)
	{
		year = d >> 16 & 0xFFFF;
		month = d >> 8 & 0xFF;
		day = d & 0xFF;
	}
};

//1394,10,11,          2016,1,1

bool IsLeapyear(tuint16 year,bool persian)
{
	if (persian)
	{
		int res = (int)((year + 37) * 31 / 128.0 * 100) % 100;
		if (res < 31)
		{
			return false;
		}
		res = (int)((year + 38) * 31 / 128.0 * 100) % 100;
		if (res >= 31)
		{
			return false;
		}
		return true;
	}else
	{
		if ((year % 100 != 0 && year % 4 == 0) || year % 400 == 0)
		{
			return true;
		}

		return false;
	}
}

tuint16 DaysInMonth(tuint16 year,tuint16 month,bool persian)
{
	if (persian)
	{
		tuint16 days[12] = {31,31,31,31,31,31,30,30,30,30,30,29};
		if (IsLeapyear(year,true))
		{
			days[11] = 30;
		}
		return days[(month - 1) % 12];
	}else
	{
		tuint16 days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		if (IsLeapyear(year,false))
		{
			days[1] = 29;
		}
		return days[(month - 1) % 12];
	}
}

tuint16 DaysToNextMonth(tuint16 year,tuint16 month,tuint16 day,bool persian)
{
	tuint16 days = DaysInMonth(year,month,persian);
	return days - day + 1;
}

tuint16 DaysToNextYear(tuint16 year,tuint16 month,tuint16 day,bool persian)
{
	tuint16 days = 0;
	for (int i = month; i < 13; ++i)
	{
		days += DaysToNextMonth(year,i,day,persian);
		day = 1;
	}

	return days;
}

// 获取公历gYear-gMonth-gDay到2016-1-1的天数
int GregorianDaysTo(tuint16 gYear,tuint16 gMonth,tuint16 gDay)
{
	// 2016,1,1
	bool negative = false;
	stDateData early(2016,1,1);
	stDateData later(gYear,gMonth,gDay);
	if (early > later)
	{
		stDateData temp;
		temp = early;early = later;later = temp;
		negative = true;
	}
	int days = 0;
	while (early != later)
	{
		if (early.year < later.year)
		{
			// 处理年份不相同，先计算年份相同时的days（年份不同，先计算early到later同年1月1号需要的days）
			if (early.month != 1 || early.day != 1)
			{
				days += DaysToNextYear(early.year,early.month,early.day,false);
				++early.year;
				early.month = early.day = 1;
			}else
			{
				days += 365;
				if (IsLeapyear(early.year,false))
				{
					++days;
				}
				++early.year;
			}
		}else if (early.month < later.month)
		{
			// 同年，计算到同月的days（月份不同，先计算early到later同月1号需要的days）
			if (early.day != 1)
			{
				days += DaysToNextMonth(early.year,early.month,early.day,false);
				++early.month;
				early.day = 1;
			}else
			{
				days += DaysInMonth(early.year,early.month,false);
				++early.month;
			}
		}else
		{
			// 同年同月，计算到同日的days
			days += later.day - early.day;
			early.day = later.day;
		}
	}

	if (negative)
	{
		days = -days;
	}

	return days;
}

// 波斯历stDateData减一天
void DecreaseOneDay( stDateData &d )
{
	if (d.day > 1)
	{
		--d.day;
	}else
	{
		if (d.month > 1)
		{
			--d.month;
			d.day = DaysInMonth(d.year,d.month,true);
		}else
		{
			--d.year;
			d.month = 12;
			d.day = 29;
			if (IsLeapyear(d.year,true))
			{
				d.day = 30;
			}
		}
	}
}
// 波斯历stDateData加一天
void IncreaseOneDay( stDateData &d )
{
	int monthDay = DaysInMonth(d.year,d.month,true);
	if (d.day < monthDay)
	{
		++d.day;
	}else
	{
		if (d.month < 12)
		{
			++d.month;
			d.day = 1;
		}else
		{
			++d.year;
			d.month = d.day = 1;
		}
	}
}

// 获取波斯历1394-10-11增加days后的日期由pYear-pMonth-pDay传回
void PersianAddDays(tuint16 &pYear,tuint16 &pMonth,tuint16 &pDay,int days)
{
	//1394,10,11,
	stDateData temp(1394,10,11);
	if (days > 0)
	{
		while (days > 0)
		{
			if (days >= 365)
			{
				// 处理days大于1年
				if (IsLeapyear(temp.year,true))
				{
					DecreaseOneDay(temp);
				}
				++temp.year;
				days -= 365;
			}else if (days >= 29)
			{
				// 处理days接近大于1个月
				int monthDay = DaysInMonth(temp.year,temp.month,true);
				days -= 29;
				temp.day += 29;
				if (temp.day > monthDay)
				{
					++temp.month;
					if (temp.month > 12)
					{
						++temp.year;
						temp.month = 1;
					}
					temp.day -= monthDay;
				}
			}else
			{
				// 处理days小于1个月
				IncreaseOneDay(temp);
				--days;
			}
		}
	}else
	{
		days = -days;
		while (days > 0)
		{
			if (days >= 365)
			{
				// 处理days大于1年
				if (IsLeapyear(temp.year-1,true))
				{
					IncreaseOneDay(temp);
				}
				--temp.year;
				days -= 365;
			}else if (days >= 29)
			{
				// 处理days接近大于1个月
				int year = temp.year,month=temp.month;
				if (temp.month <= 1)
				{
					--year;
					month = 12;
				}else
				{
					--month;
				}
				int monthDay = DaysInMonth(year,month,true);
				days -= 29;
				if (temp.day > 29)
				{
					temp.day -= 29;
				}
				else
				{
					--temp.month;
					if (temp.month < 1)
					{
						--temp.year;
						temp.month = 12;
					}
					temp.day += monthDay-29;
				}
			}else
			{
				// 处理days小于1个月
				DecreaseOneDay(temp);
				--days;
			}
		}
	}
	pYear = temp.year;pMonth = temp.month;pDay = temp.day;
}

CPFTime::CPFTime(void)
{
	tzset();
	timeval tp;
	gettimeofday(&tp, NULL);
    m_time=tp.tv_sec;
	m_mmTime=tp.tv_usec*10;
}

CPFTime::CPFTime(time_t timet)//以time_t表达的时间
{
	tzset();
	m_time=timet;
	m_mmTime=0;
}

CPFTime::CPFTime(FILETIME ft)//以FILETIME表达的时间
{
	tzset();
	tuint64 ull = 0;
	memcpy(&ull,&ft,sizeof(tuint64));
	m_mmTime=ull%10000000;
	m_time=(ull-116444736000000000ULL)/10000000;
}

CPFTime::CPFTime(tint32 nYear, tint32 nMonth, tint32 nDay, tint32 nHour, tint32 nMin, tint32 nSec, bool bUTC/* = false*/)
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
		m_time = timegm(&tmp);
	}
	else
	{
		m_time=mktime(&tmp);
	}

	m_mmTime=0;
}

CPFTime::CPFTime(const char *pTime,bool bUTC/* = false*/)//时间从2014-05-01 11:11:11  转成time_t
{
	tzset();
	struct tm tmp;
	memset(&tmp,0,sizeof(tmp));
	sscanf(pTime, "%4d-%2d-%2d %2d:%2d:%2d",     
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
		m_time = timegm(&tmp);
	}
	else
	{
		m_time=mktime(&tmp);
	}

	m_mmTime=0;
}

CPFTime::~CPFTime(void)
{
}

tint32 CPFTime::GetYear(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return 1900+ptm.tm_year;
}

tint32 CPFTime::GetUTCYear(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return 1900+ptm.tm_year;
}

tint32 CPFTime::GetMonth(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return ptm.tm_mon+1;
}

tint32 CPFTime::GetUTCMonth(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return ptm.tm_mon+1;
}

tint32 CPFTime::GetDay(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return ptm.tm_mday;
}

tint32 CPFTime::GetUTCDay(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return ptm.tm_mday;
}

tint32 CPFTime::GetHour(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return ptm.tm_hour;
}

tint32 CPFTime::GetUTCHour(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return ptm.tm_hour;
}

tint32 CPFTime::GetMinute(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return ptm.tm_min;
}

tint32 CPFTime::GetUTCMinute(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return ptm.tm_min;
}

tint32 CPFTime::GetSecond(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return ptm.tm_sec;
}

tint32 CPFTime::GetUTCSecond(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return ptm.tm_sec;
}

tint32 CPFTime::GetDayOfWeek(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	localtime_r(&m_time,&ptm); 
	return ptm.tm_wday;
}

tint32 CPFTime::GetUTCDayOfWeek(void)
{
	struct tm ptm;
	memset(&ptm,0,sizeof(ptm));
	gmtime_r(&m_time,&ptm); 
	return ptm.tm_wday;
}

tint32 CPFTime::GetMilliSecond(void)
{
    return m_mmTime/10000;
}

tuint32 CPFTime::Get100ns(void)
{
	return m_mmTime;
}

void CPFTime::GetFileTime(FILETIME &ft)//得到格林威治时间的FILETIME
{	
	tuint64 ull = (tuint64)m_time * 10000000 + 116444736000000000ULL;
	ull += m_mmTime;
	memcpy(&ft,&ull,sizeof(tuint64));
}

extern long timezone;
//做成一样吧
void CPFTime::GetLocalTime_100ns(tuint64 &ullLocalTime)//得到本地时间
{
	tzset();
	tuint64 dst=timezone;
 	tuint64 ull = (tuint64)m_time * 10000000 + 116444736000000000ULL;
	ull += m_mmTime;
 	ull +=dst*10000000;
	ullLocalTime=ull;
}

time_t CPFTime::GetTime()
{
	return m_time;
}


bool CPFTime::SetOSLocalTime(tint32 utcYear, tint32 utcMonth, tint32 utcDay, tint32 utcHour, tint32 utcMin, tint32 utcSec)
{
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
}

tuint64 CPFTime::GetTimeMs()
{
    tuint64 tmMs = m_time *1000;
    tmMs+=(m_mmTime/10/1000);
    return tmMs;
}

bool CPFTime::GregorianToPersian( tuint16 gYear,tuint16 gMonth,tuint16 gDay,tuint16 &pYear,tuint16 &pMonth,tuint16 &pDay )
{
	if (gDay > DaysInMonth(gYear,gMonth,false))
	{
		return false;
	}
	int adds = GregorianDaysTo(gYear,gMonth,gDay);
	PersianAddDays(pYear,pMonth,pDay,adds);
	return true;
}
