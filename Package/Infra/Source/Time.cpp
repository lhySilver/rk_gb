#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
//#include <time.h>
#include <assert.h>
#include <string.h>
#include <Infra/Time.h>

#ifdef WIN32
	#include <Windows.h>
#elif defined(LINUX)
	#include <sys/time.h>
	#include <sys/times.h>
	#include <sys/select.h>
	#include <unistd.h>
#ifdef CONFIG_CKCORE
	#define sysconf
	#define _SC_CLK_TCK 100
#else
//	_syscall1(clock_t, times, struct tms *, buf); // 直接调用系统函数times,避免tick值跳变
#endif
#else
	#error "Unknown Platform"
#endif

int CTime::sm_dateFormat = DF_YYMMDD;
int CTime::sm_timeFormat = fmtNormal;
int CTime::sm_timeSpliter = DS_DOT;
const CTime CTime::minTime(2000, 1, 1, 0, 0, 0);
const CTime CTime::maxTime(2038, 1, 1, 0, 0, 0);


#ifdef WIN32
#include <windows.h>
static double ddFreq;
static double ddBaseTime;
class WinTimeStarter // make something happen for windows
{
public:
	WinTimeStarter()
	{
		LARGE_INTEGER c, f;
		QueryPerformanceFrequency(&f);
		ddFreq = (double)f.QuadPart / 1000000;
		QueryPerformanceCounter(&c);
		ddBaseTime = (double)c.QuadPart / ddFreq;
	}
}starter;
#endif

CTime::CTime()
{
}

CTime::CTime(const SystemTime &time)
{
	year = time.year;
	month = time.month;
	day = time.day;
	hour = time.hour;
	minute = time.minute;
	second = time.second;

	breakTime(makeTime());
}

CTime::CTime( int vyear, int vmonth, int vday, int vhour, int vmin, int vsec)
{
	struct tm t;

	year = vyear;
	t.tm_year = year - 1900;
	month = vmonth;
	t.tm_mon = vmonth - 1;
	day = t.tm_wday = vday;
	hour = t.tm_hour = vhour;
	minute = t.tm_min = vmin;
	second = t.tm_sec = vsec;

	breakTime(makeTime()); // 值校验，规范化，生成星期
}

CTime CTime::getCurrentTime()
{
#ifdef WIN32
	SYSTEMTIME time;

	GetLocalTime(&time);
	return CTime(time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

#elif defined(LINUX)
	CTime t;
	struct timeval tv;

	gettimeofday(&tv, 0);
	t.breakTime(tv.tv_sec);

	return t;

#elif defined(__TCS__)
	ulong date, time, ticks;

	tm_get(&date, &time, &ticks);
	return CTime((date >> 16), ((date >> 8) & 0xff), (date & 0xff),
		(time >> 16), ((time >> 8) & 0xff), (time & 0xff));

#endif
}

bool CTime::getCurrentTime(SystemTime &time)		// zzb 2013.8.20 add
{
	CTime t = getCurrentTime();

	time.year = t.year;
	time.month = t.month;
	time.day = t.day;
	time.wday = t.wday;
	time.hour = t.hour;
	time.minute = t.minute;
	time.second = t.second;
	time.isdst = t.isdst;

	return true;
}

void CTime::setCurrentTime(const CTime& time)
{
#ifdef WIN32
	SYSTEMTIME t;

	t.wYear = time.year;
	t.wMonth = time.month;
	t.wDay = time.day;
	t.wHour = time.hour;
	t.wMinute = time.minute;
	t.wSecond = time.second;
	SetLocalTime(&t);

#elif defined(LINUX)
	struct timeval tv;

	tv.tv_sec = time.makeTime();
	tv.tv_usec = 0;

	settimeofday(&tv, 0);

	// 同步到rtc
//<shang>	system("hwclock -w");

#elif defined(__TCS__)
	tm_set((time.year << 16) + (time.month << 8) + time.day,
		(time.hour << 16) + (time.minute << 8) + time.second,
		0);

#endif
}

void CTime::setCurrentTime(SystemTime &time)		// zzb 2013.8.20 add
{
	CTime t;

	t.year = time.year;
	t.month = time.month;
	t.day = time.day;
	t.wday = time.wday;
	t.hour = time.hour;
	t.minute = time.minute;
	t.second = time.second;
	t.isdst = time.isdst;

	setCurrentTime(t);
}

//uint64 CTime::getCurrentMillSecond()	// ms	// zzb 2013.8.21 add
uint64 CTime::getCurrentMicroSecond()	// ms: 毫秒	// zzb 2013.8.21 add
{
#ifdef WIN32
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	return (uint64)(((double)c.QuadPart / ddFreq - ddBaseTime) / 1000);

#elif defined(LINUX)
#if 0
	static uint64 m_tickoffs = 0;
    uint64 tick = (uint64)times(NULL); // 产生从0开始的tick计数，大约可以坚持20个月不溢出
	if(m_tickoffs == 0)
	{
		m_tickoffs = 0 - tick;
	}
	return (tick + m_tickoffs) * (1000 / sysconf(_SC_CLK_TCK));
#else
//	uint64 tick = time(NULL) * 1000;
//	return tick;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((uint64)tv.tv_sec * 1000 + tv.tv_usec / 1000);

#endif
#elif defined(__TCS__)
	ulong hi, lo;

	tm_getticks(&hi, &lo);
	return (((uint64)hi << 32) + lo) * (1000 / KC_TICKS2SEC);

#else
	#error "Unknown Platform"
#endif
}

//uint64 CTime::getCurrentMicroSecond()	// us
uint64 CTime::getCurrentMillSecond()	// us: 微秒
{
#ifdef WIN32
	LARGE_INTEGER c;
	QueryPerformanceCounter(&c);
	return (uint64)(((double)c.QuadPart / ddFreq) - ddBaseTime);

#elif defined(LINUX)
	struct timeval tv;
#if 0									// zzb 2013.9.16 test
	struct timezone	tz;
	gettimeofday(&tv, &tz);
#else
	gettimeofday(&tv, NULL);
#endif
	return ((uint64)tv.tv_sec * 1000 * 1000 + tv.tv_usec);

#elif defined(__TCS__)
	return getCurrentMillSecond() * 1000;
#else
	#error "Unknown Platform"
#endif
}

void CTime::sleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#elif defined(LINUX)
	#if 0
		struct timespec t;

		t.tv_sec = ms / 1000;
		t.tv_nsec = ((ms % 1000) * 1000000); // 0 to 999 999 999
		while(nanosleep(&t, &t) != 0);
	#else // uclinux下select更准确，且延时时间越长越准确
		struct timeval t;

		t.tv_sec = ms / 1000;
		t.tv_usec = (ms % 1000) * 1000;
		while(select(0, NULL, NULL, NULL, &t) != 0);
	#endif
#elif defined(__TCS__)
	struct timeval t;

	t.tv_sec = ms / 1000;
	t.tv_usec = (ms % 1000) * 1000;
	while(select(0, NULL, NULL, NULL, &t) != 0);

#else
	#error "Unknown Platform"
#endif
}

int CTime::getDateFormat()
{
	return sm_dateFormat;
}

//uint64 CTime::makeTime() const	// zzb 2013.8.20 modify
time_t CTime::makeTime() const
{
	time_t		tt;
	struct tm	t;

	t.tm_sec	= second;
	t.tm_min	= minute;
	t.tm_hour	= hour;
	t.tm_mday	= day;
	t.tm_mon	= month - 1;
	t.tm_year	= year - 1900;
	t.tm_isdst = 0;

	tt = mktime(&t);

	//assert(tt > 0);
	if (tt < 0)		// zzb 2013.8.23 modify
	//if (tt == -1)		// error occured		// zzb 2013.9.9 test
		tt = 0;

	return tt;
}

void CTime::breakTime(uint64 time)
{
	struct tm t;
	time_t tt = (time_t)time; // linux下可能是32位，会溢出
	if(time != (uint64)tt)
	{
		printf("CTime::breakTime overflowed!\n");
	}

	//need protect ???
#if defined(WIN32) || defined(__TCS__)
	t = *localtime(&tt);
#else
	localtime_r(&tt, &t);
#endif
	year =  t.tm_year + 1900;
	month = t.tm_mon + 1;
	wday = t.tm_wday;
	day = t.tm_mday;
	hour = t.tm_hour;
	minute = t.tm_min;
	second = t.tm_sec;
}

CTime CTime::operator + ( int64 seconds ) const
{
	CTime time;
	time.breakTime(makeTime() + seconds);

	return time;
}

CTime CTime::operator - ( int64 seconds ) const
{
	CTime time;
	time.breakTime(makeTime() - seconds);

	return time;
}

int64 CTime::operator - (const CTime& time ) const
{
	return ((int64)makeTime() - (int64)time.makeTime());
}

CTime& CTime::operator += (int64 seconds)
{
	breakTime(uint64((int64)makeTime() + seconds));

	return *this;
}

CTime& CTime::operator -= (int64 seconds)
{
	breakTime(uint64((int64)makeTime() - seconds));

	return *this;
}

bool CTime::operator == (const CTime& time) const
{
	return (makeTime() == time.makeTime());
}

bool CTime::operator != (const CTime& time) const
{
	return (makeTime() != time.makeTime());
}

bool CTime::operator > (const CTime& time) const
{
	return (makeTime() > time.makeTime());
}

bool CTime::operator >= (const CTime& time) const
{
	return (makeTime() >= time.makeTime());
}

bool CTime::operator < (const CTime& time) const
{
	return (makeTime() < time.makeTime());
}

bool CTime::operator <= (const CTime& time) const
{
	return (makeTime() <= time.makeTime());
}


// ----------------- zzb 2013.8.20 add begin ---------------------
CTime& CTime::operator = (const CTime& time)
{
	breakTime(time.makeTime());
	return *this;
}

std::string CTime::format(uint fmt/* = fmtNormal*/) const
{
	char buf[50] = {0};
	char sep = '-';

	switch(sm_timeSpliter)
	{
		case DS_DOT:
			sep = '.';
			break;
		case DS_DASH:
			sep = '-';
			break;
		case DS_SLASH:
			sep = '/';
			break;
	}

	int h = hour;
	if(sm_timeFormat == TF_12)
	{
		if(h > 12)
			h -= 12;
		else if(h == 0)
			h = 12;
	}
	if(fmt & fmtOnlyTime)
		sprintf(buf, "%02d:%02d:%02d", h, minute, second);
	else
	{
		switch(sm_dateFormat)
		{
		case DF_YYMMDD:
			if(fmt & fmtHalfYear)
				sprintf(buf, "%02d%c%02d%c%02d %02d:%02d:%02d", year-2000, sep, month, sep, day, h, minute, second);
			else if(fmt & fmtNoSign)
				sprintf(buf, "%04d%02d%02d%02d%02d%02d", year, month, day, h, minute, second);
			else if(fmt & fmtOnlyDate)
				sprintf(buf, "%04d%c%02d%c%02d", year, sep, month, sep, day);
			else
				sprintf(buf, "%04d%c%02d%c%02d %02d:%02d:%02d", year, sep, month, sep, day, h, minute, second);
			break;

		case DF_MMDDYY:
			if(fmt & fmtHalfYear)
				sprintf(buf, "%02d%c%02d%c%02d %02d:%02d:%02d", month, sep, day, sep, year-2000, h, minute, second);
			else if(fmt & fmtNoSign)
				sprintf(buf, "%02d%02d%04d%02d%02d%02d", month, day, year, h, minute, second);
			else if(fmt & fmtOnlyDate)
				sprintf(buf, "%02d%c%02d%c%04d", month, sep, day, sep, year);
			else
				sprintf(buf, "%02d%c%02d%c%04d %02d:%02d:%02d", month, sep, day, sep, year, h, minute, second);
			break;

		case DF_DDMMYY:
			if(fmt & fmtHalfYear)
				sprintf(buf, "%02d%c%02d%c%02d %02d:%02d:%02d", day, sep, month, sep, year-2000, h, minute, second);
			else if(fmt & fmtNoSign)
				sprintf(buf, "%02d%02d%04d%02d%02d%02d", day, month, year, h, minute, second);
			else if(fmt & fmtOnlyDate)
				sprintf(buf, "%02d%c%02d%c%04d", day, sep, month, sep, year);
			else
				sprintf(buf, "%02d%c%02d%c%04d %02d:%02d:%02d", day, sep, month, sep, year, h, minute, second);
			break;
		}
	}
	if(fmt & fmtNoSecond)
		buf[strlen(buf)-3] = '\0';
	if(sm_timeFormat == TF_12 && !(fmt & fmtOnlyDate))
		strcat(buf, hour/12 ? "PM" : "AM");

	return std::string(buf);
}

void CTime::setTimeFormat(int dateFormat, int timeFormat, int timeSpliter)
{
	sm_dateFormat = dateFormat;
	sm_timeFormat = timeFormat;
	sm_timeSpliter = timeSpliter;
}

int CTime::getTimeFormat()
{
	return sm_timeFormat;
}

int CTime::getTimeSpliter()
{
	return sm_timeSpliter;
}

int CTime::getDaysOfMonth(int year, int month)
{
	const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if(month > 0 && month <= 12)
	{
		int d = days[month - 1];
		if(month == 2)
			d += CTime::isLeapYear(year);
		return d;
	}

	return 0;
}

/*
	function:	将12小时制的hour转换成24小时制的hour
	return:		24小时制的hour
	ampm: 		0-AM, 1-PM
*/
int CTime::hour12To24(int hour12, int ampm)
{
	if(hour12 < 12 && ampm)
		return (hour12 + 12);
	else if(hour12 == 12 && ampm == 0)
		return 0;
	else
		return hour12;
}

int CTime::hour24To12(int hour24)
{
	if(hour24 > 12)
		return (hour24 - 12);
	else if(hour24 == 0)
		return 12;
	else
		return hour24;
}

int CTime::hour24ToAMPM(int hour24)
{
	return (hour24 < 12 ? 0 : 1);
}

bool CTime::isLeapYear(int year)
{
	if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		return true;
	else
		return false;
}

// -------- Global Function, maybe depending on CTime Class ------
void FormatTimeString(SystemTime *p, char* buf, int flag)
{
	if(p && buf)
	{
		CTime t(*p);
		std::string s = t.format(flag);		// 直接调用CTime::format()
		strcpy(buf, s.c_str());
	}
}

void FormatTimeStringDate(DateTime *p, char* buf, int flag)
{
	if(p && buf)
	{
		SystemTime systime;
		TimeDate2Sys(&systime, p);
		FormatTimeString(&systime, buf, flag);
	}
}

void TimeDate2Sys(SystemTime * systime, DateTime * dateTime)	// DateTime -> SystemTime
{
	assert(dateTime && systime);

	systime->year = dateTime->year + 2000;
	systime->month = dateTime->month;
	systime->day = dateTime->day;
	systime->hour = dateTime->hour;
	systime->minute = dateTime->minute;
	systime->second = dateTime->second;
}

void TimeSys2Date(DateTime * dateTime, SystemTime * systime)	// SystemTime -> DateTime
{
	assert(dateTime && systime);

	if(systime->year >= 2000 && systime->year <= 2063)
		dateTime->year = systime->year - 2000;
	else
		dateTime->year = 0;
	dateTime->month = systime->month;
	dateTime->day = systime->day;
	dateTime->hour = systime->hour;
	dateTime->minute = systime->minute;
	dateTime->second = systime->second;
}

int TimeCompare(SystemTime * pSysTime1, SystemTime * pSysTime2)
{
#if 0
	CTime t1(*pSysTime1);
	CTime t2(*pSysTime2);

	if(t1 < t2)
		return -1;
	else if(t1 == t2)
		return 0;
	else if(t1 > t2)
		return 1;
#else
#define COMPARE(m, n) ((m) < (n) ? -1 : ((m) > (n) ? 1 : 0))
#define SECONDS(h, m, s) ((h) * 60 * 60 + (m) * 60 + s)
	int f;
	if( (f = COMPARE(pSysTime1->year, pSysTime2->year)) != 0)
		return f;
	else if( (f = COMPARE(pSysTime1->month, pSysTime2->month)) != 0)
		return f;
	else if( (f = COMPARE(pSysTime1->day, pSysTime2->day)) != 0)
		return f;
	else
	{
		int s1, s2;
		s1 = SECONDS(pSysTime1->hour, pSysTime1->minute, pSysTime1->second);
		s2 = SECONDS(pSysTime2->hour, pSysTime2->minute, pSysTime2->second);
		return COMPARE(s1, s2);
	}
#undef COMPARE
#undef SECONDS
#endif
	return 0;
}

void TimeAdd(SystemTime * pOldTime, SystemTime * pNewTime, int second)
{
	CTime t(*pOldTime);

	t += second;
	pNewTime->year = t.year;
	pNewTime->month = t.month;
	pNewTime->day = t.day;
	pNewTime->wday = t.wday;
	pNewTime->hour = t.hour;
	pNewTime->minute = t.minute;
	pNewTime->second = t.second;
}

int Time2Second(SystemTime * pSysTime1, SystemTime * pSysTime2)
{
	CTime t1(*pSysTime1);
	CTime t2(*pSysTime2);

	return (int)(t2 - t1);
}

int GetCurDateTime(DateTime* ptime)
{
	SystemTime systime;

	if(CTime::getCurrentTime(systime))
	{
		TimeSys2Date(ptime, &systime);
		return 0;
	}

	return -1;
}

uint IsZeroDateTime(DateTime t)
{
	SystemTime systime;
	CTime dt0(2000, 1, 1, 0, 0, 0);		// zero datetime

	TimeDate2Sys(&systime, &t);
	CTime tnew(systime);

	return tnew.makeTime() == dt0.makeTime();
}

int CompareDateTime(DateTime t1, DateTime t2)
{
	SystemTime s1, s2;

	TimeDate2Sys(&s1, &t1);
	TimeDate2Sys(&s2, &t2);

	return TimeCompare(&s1, &s2);
}

int DateTimeChangeSecond(DateTime end_time, DateTime start_time)
{
	SystemTime s1, s2;

	TimeDate2Sys(&s1, &start_time);
	TimeDate2Sys(&s2, &end_time);

	return Time2Second(&s1, &s2);
}

DateTime AddDateTime(DateTime time, int second)
{
	DateTime dt;
	SystemTime s1, s2;

	TimeDate2Sys(&s1, &time);
	TimeAdd(&s1, &s2, second);
	TimeSys2Date(&dt, &s2);

	return dt;
}

/*
	功能:			将秒数转换成具体的时间, 存放在SystemTime结构体中并返回
	return:			SystemTime类型的变量
	seconds:		秒数(相对于1970-1-1 08:00:00, 对中国而言是8点)
*/
SystemTime SecondToSystemTime(long seconds)
{
	SystemTime sysTime = {0};
	tm tp;
	localtime_r(&seconds, &tp);

	sysTime.year = tp.tm_year + 1900;
	sysTime.month = tp.tm_mon + 1;
	sysTime.day = tp.tm_mday;
	sysTime.hour = tp.tm_hour;
	sysTime.minute = tp.tm_min;
	sysTime.second = tp.tm_sec;

	return sysTime;
}

long SystemTimeToSecond(SystemTime *pSys)
{
	struct tm p;
	p.tm_year = pSys->year - 1900;
	p.tm_mon = pSys->month - 1;
	p.tm_mday = pSys->day;
	p.tm_hour = pSys->hour;
	p.tm_min = pSys->minute;
	p.tm_sec = pSys->second;
	
	time_t s = mktime(&p);
	return s;
}

/*
	功能:			获取某年某月有多少天
	return:			天数
*/

int GetDayOfYearMonth(int year, int month)
{
	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			if( (year%400 == 0) || (year%4 == 0 && year%100 != 0) )
				return 29;
			else
			return 28;
		default:
			return -1;
	}
}

uint64 GetSystemTime_ms()
{
	uint64 u64SysTime_ms = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	u64SysTime_ms = (uint64)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return u64SysTime_ms;
}

uint64 GetSystemUptime_ms()
{
	uint64 u64SysTime_ms = 0;
    struct timespec stTime;
    clock_gettime(CLOCK_MONOTONIC, &stTime);
	u64SysTime_ms = (uint64)stTime.tv_sec * 1000 + stTime.tv_nsec / 1000000;
	return u64SysTime_ms;
}

int GetSystemUptime_s()
{
    struct timespec stTime;
    clock_gettime(CLOCK_MONOTONIC, &stTime);
	return stTime.tv_sec;
}

