#include "DateTime.h"
#include "base_type_define.h"
#include <math.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PFTime.h"


static inline uint msecsFromDecomposed(int hour, int minute, int sec, int msec = 0)
{
    return MSECS_PER_HOUR * hour + MSECS_PER_MIN * minute + 1000 * sec + msec;
}
static inline PDate fixedDate(int y, int m, int d)
{
    PDate result(y, m, 1);
    result.setDate(y, m, min(d, result.daysInMonth()));
    return result;
}
static void getDateFromJulianDay(uint julianDay, int *year, int *month, int *day)
{
    int y, m, d;

    if (julianDay >= 2299161) {
        // Gregorian calendar starting from October 15, 1582
        // This algorithm is from Henry F. Fliegel and Thomas C. Van Flandern
        uint64 ell, n, i, j;
        ell = uint64(julianDay) + 68569;
        n = (4 * ell) / 146097;
        ell = ell - (146097 * n + 3) / 4;
        i = (4000 * (ell + 1)) / 1461001;
        ell = ell - (1461 * i) / 4 + 31;
        j = (80 * ell) / 2447;
        d = ell - (2447 * j) / 80;
        ell = j / 11;
        m = j + 2 - (12 * ell);
        y = 100 * (n - 49) + i + ell;
    }
    else {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        julianDay += 32082;
        int dd = (4 * julianDay + 3) / 1461;
        int ee = julianDay - (1461 * dd) / 4;
        int mm = ((5 * ee) + 2) / 153;
        d = ee - (153 * mm + 2) / 5 + 1;
        m = mm + 3 - 12 * (mm / 10);
        y = dd - 4800 + (mm / 10);
        if (y <= 0)
            --y;
    }
    if (year)
        *year = y;
    if (month)
        *month = m;
    if (day)
        *day = d;
}
static inline uint julianDayFromGregorianDate(int year, int month, int day)
{
    // Gregorian calendar starting from October 15, 1582
    // Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
    return (1461 * (year + 4800 + (month - 14) / 12)) / 4
        + (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
        - (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
        + day - 32075;
}
static uint julianDayFromDate(int year, int month, int day)
{
    if (year < 0)
        ++year;

    if (year > 1582 || (year == 1582 && (month > 10 || (month == 10 && day >= 15)))) {
        return julianDayFromGregorianDate(year, month, day);
    }
    else if (year < 1582 || (year == 1582 && (month < 10 || (month == 10 && day <= 4)))) {
        // Julian calendar until October 4, 1582
        // Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
        int a = (14 - month) / 12;
        return (153 * (month + (12 * a) - 3) + 2) / 5
            + (1461 * (year + 4800 - a)) / 4
            + day - 32083;
    }
    else {
        // the day following October 4, 1582 is October 15, 1582
        return 0;
    }
}
static const char monthDays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };



PDate::PDate(int y, int m, int d)
{
    setDate(y, m, d);
}

int PDate::year() const
{
    int y;
    getDateFromJulianDay(jd, &y, 0, 0);
    return y;
}
int PDate::month() const
{
    int m;
    getDateFromJulianDay(jd, 0, &m, 0);
    return m;
}
int PDate::day() const
{
    int d;
    getDateFromJulianDay(jd, 0, 0, &d);
    return d;
}
int PDate::dayOfWeek() const
{
    return (jd % 7) + 1;
}
int PDate::dayOfYear() const
{
    return jd - julianDayFromDate(year(), 1, 1) + 1;
}
int PDate::daysInMonth() const
{
    int y, m, d;
    getDateFromJulianDay(jd, &y, &m, &d);
    if (m == 2 && isLeapYear(y))
        return 29;
    else
        return monthDays[m];
}
int PDate::daysInYear() const
{
    int y, m, d;
    getDateFromJulianDay(jd, &y, &m, &d);
    return isLeapYear(y) ? 366 : 365;
}
int PDate::weekNumber(int *yearNumber) const
{
    if (!isValid())
        return 0;

    int year = PDate::year();
    int yday = dayOfYear() - 1;
    int wday = dayOfWeek();
    if (wday == 7)
        wday = 0;
    int w;

    for (;;) {
        int len;
        int bot;
        int top;

        len = isLeapYear(year) ? 366 : 365;
        /*
        ** What yday (-3 ... 3) does
        ** the ISO year begin on?
        */
        bot = ((yday + 11 - wday) % 7) - 3;
        /*
        ** What yday does the NEXT
        ** ISO year begin on?
        */
        top = bot - (len % 7);
        if (top < -3)
            top += 7;
        top += len;
        if (yday >= top) {
            ++year;
            w = 1;
            break;
        }
        if (yday >= bot) {
            w = 1 + ((yday - bot) / 7);
            break;
        }
        --year;
        yday += isLeapYear(year) ? 366 : 365;
    }
    if (yearNumber != 0)
        *yearNumber = year;
    return w;
}

bool PDate::setDate(int year, int month, int day)
{
    if (!isValid(year, month, day)) {
        jd = 0;
    }
    else {
        jd = julianDayFromDate(year, month, day);
    }
    return jd != 0;
}
void PDate::getDate(int *year, int *month, int *day)
{
    getDateFromJulianDay(jd, year, month, day);
}

PDate PDate::addDays(int ndays) const
{
    PDate d;
    // this is basically "d.jd = jd + ndays" with checks for integer overflow
    if (ndays >= 0)
        d.jd = (jd + ndays >= jd) ? jd + ndays : 0;
    else
        d.jd = (jd + ndays < jd) ? jd + ndays : 0;
    return d;
}
PDate PDate::addMonths(int nmonths) const
{
    if (!isValid())
        return PDate();
    if (!nmonths)
        return *this;

    int old_y, y, m, d;
    getDateFromJulianDay(jd, &y, &m, &d);
    old_y = y;

    bool increasing = nmonths > 0;

    while (nmonths != 0) {
        if (nmonths < 0 && nmonths + 12 <= 0) {
            y--;
            nmonths += 12;
        }
        else if (nmonths < 0) {
            m += nmonths;
            nmonths = 0;
            if (m <= 0) {
                --y;
                m += 12;
            }
        }
        else if (nmonths - 12 >= 0) {
            y++;
            nmonths -= 12;
        }
        else if (m == 12) {
            y++;
            m = 0;
        }
        else {
            m += nmonths;
            nmonths = 0;
            if (m > 12) {
                ++y;
                m -= 12;
            }
        }
    }

    // was there a sign change?
    if ((old_y > 0 && y <= 0) ||
        (old_y < 0 && y >= 0))
        // yes, adjust the date by +1 or -1 years
        y += increasing ? +1 : -1;

    // did we end up in the Gregorian/Julian conversion hole?
    if (y == 1582 && m == 10 && d > 4 && d < 15)
        d = increasing ? 15 : 4;

    return fixedDate(y, m, d);
}
PDate PDate::addYears(int nyears) const
{
    if (!isValid())
        return PDate();

    int y, m, d;
    getDateFromJulianDay(jd, &y, &m, &d);

    int old_y = y;
    y += nyears;

    // was there a sign change?
    if ((old_y > 0 && y <= 0) ||
        (old_y < 0 && y >= 0))
        // yes, adjust the date by +1 or -1 years
        y += nyears > 0 ? +1 : -1;

    return fixedDate(y, m, d);
}
int PDate::daysTo(const PDate &d) const
{
    return jd - d.jd;
}

PDate PDate::currentDate()
{
    PDate d;
    time_t rawtime = ::time(NULL);
    tm* st = localtime(&rawtime);
    d.jd = julianDayFromDate(st->tm_year + 1900, st->tm_mon + 1, st->tm_mday);
    return d;
}

// 仅支持yyyy-mm-dd

PDate PDate::fromString(const std::string &s)
{
    assert(!s.empty());

    int y = atoi(s.substr(0, 4).c_str());
    int m = atoi(s.substr(5, 2).c_str());
    int d = atoi(s.substr(8, 2).c_str());
    PDate date;
    date.setDate(y, m, d);
    date.jd = julianDayFromDate(y, m, d);
    return date;
}

string PDate::toString()
{
    int y, m, d;
    getDate(&y, &m, &d);
    char ptr[11] = { 0 };
    snprintf(ptr, sizeof(ptr), "%04d-%02d-%02d", y, m, d);
    return string(ptr);
}
bool PDate::isLeapYear(int y)
{
    if (y < 1582) {
        if (y < 1) {  // No year 0 in Julian calendar, so -1, -5, -9 etc are leap years
            ++y;
        }
        return y % 4 == 0;
    }
    else {
        return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
    }
}
bool PDate::isValid(int year, int month, int day)
{
    if (year < -4713
        || (year == -4713 &&
            (month < 1
             || (month == 1 && day < 2)))
        || year == 0) // there is no year 0 in the Julian calendar
        return false;

    // passage from Julian to Gregorian calendar
    if (year == 1582 && month == 10 && day > 4 && day < 15)
        return 0;

    return (day > 0 && month > 0 && month <= 12) &&
        (day <= monthDays[month] || (day == 29 && month == 2 && isLeapYear(year)));
}
uint PDate::gregorianToJulian(int y, int m, int d)
{
    return julianDayFromDate(y, m, d);
}
void PDate::julianToGregorian(uint jd, int &y, int &m, int &d)
{
    getDateFromJulianDay(jd, &y, &m, &d);
}



PTime::PTime(int h, int m, int s, int ms)
{
    setHMS(h, m, s, ms);
}


bool PTime::isValid() const
{
    return mds > NullTime && mds < MSECS_PER_DAY;
}

int PTime::hour() const
{
    return ds() / MSECS_PER_HOUR;
}
int PTime::minute() const
{
    return (ds() % MSECS_PER_HOUR) / MSECS_PER_MIN;
}
int PTime::second() const
{
    return (ds() / 1000) % SECS_PER_MIN;
}
int PTime::msec() const
{
    return ds() % 1000;
}

string PTime::toString()
{
    int h = hour();
    int m = minute();
    int s = second();
    char ptr[11] = { 0 };
    snprintf(ptr, sizeof(ptr), "%02d:%02d:%02d", h, m, s);
    return string(ptr);
}

bool PTime::setHMS(int h, int m, int s, int ms)
{
    if (!isValid(h, m, s, ms)) {
        mds = NullTime;                // make this invalid
        return false;
    }
    mds = (h*SECS_PER_HOUR + m*SECS_PER_MIN + s) * 1000 + ms;
    return true;
}

PTime PTime::addSecs(int s) const
{
    return addMSecs(s * 1000);
}
int PTime::secsTo(const PTime &t) const
{
    return (ds() - t.ds()) / 1000;
}
PTime PTime::addMSecs(int ms) const
{
    PTime t;
    if (ms < 0) {
        // % not well-defined for -ve, but / is.
        int negdays = (MSECS_PER_DAY - ms) / MSECS_PER_DAY;
        t.mds = (ds() + ms + negdays * MSECS_PER_DAY) % MSECS_PER_DAY;
    }
    else {
        t.mds = (ds() + ms) % MSECS_PER_DAY;
    }

    return t;
}
int PTime::msecsTo(const PTime &t) const
{
    return t.ds() - ds();
}

PTime PTime::currentTime()
{
    PTime ct;
    time_t rawtime = ::time(NULL);
    tm* st = localtime(&rawtime);

    ct.mds = msecsFromDecomposed(st->tm_hour, st->tm_min, st->tm_sec, 0);

    return ct;
}
// 仅支持hh:mm:ss
PTime PTime::fromString(const std::string &s)
{
    assert(!s.empty());
    int h = atoi(s.substr(0, 2).c_str());
    int m = atoi(s.substr(3, 2).c_str());
    int ss = 0;
    if (s.length() > 6) {
        std::string str = s.substr(6, 2);
        if (str.length() == 2) {
            ss = atoi(s.substr(6, 2).c_str());
        }
    }
    return PTime(h, m, ss, 0);
}
bool PTime::isValid(int h, int m, int s, int ms)
{
    return (uint)h < 24 && (uint)m < 60 && (uint)s < 60 && (uint)ms < 1000;
}

void PTime::start()
{
    *this = currentTime();
}
int PTime::restart()
{
    PTime t = currentTime();
    int n = msecsTo(t);
    if (n < 0)                                // passed midnight
        n += 86400 * 1000;
    *this = t;
    return n;
}

int PTime::elapsed() const
{
    int n = msecsTo(currentTime());
    if (n < 0)                                // passed midnight
        n += 86400 * 1000;
    return n;
}


time_t  PDateTime::deltaTime = 0;
PDateTime::PDateTime() : d(0)
{
    memset(&_dateTime, 0, sizeof(_dateTime));
}
PDateTime::PDateTime(const PDate &dt)
{
    memset(&_dateTime, 0, sizeof(_dateTime));
    setDate(dt);
}
PDateTime::PDateTime(const PDate &dt, const PTime &tmm)
{
    memset(&_dateTime, 0, sizeof(_dateTime));
    setDate(dt);
    setTime(tmm);
}
PDateTime::PDateTime(const PDateTime &other)
{
    _date = other.date();
    _time = other.time();
    d = other.d;
    _dateTime = other._dateTime;
    //setDate(other.date());
    //setTime(other.time());
}
PDateTime::~PDateTime()
{

}

PDateTime& PDateTime::operator=(const PDateTime &other)
{
    if (this == &other)
        return *this;
    _date = other.date();
    _time = other.time();
    d = other.d;
    _dateTime = other._dateTime;
    return *this;
}

bool PDateTime::operator==(const PDateTime &other) const
{
    return ((this->date() == other.date()) && (this->time() == other.time()) && (this->d == other.d));
}

bool PDateTime::operator!=(const PDateTime &other) const
{
    return !(*this == other);
}

bool PDateTime::operator<(const PDateTime &other) const
{
    return this->d < other.d;
}

bool PDateTime::operator<=(const PDateTime &other) const
{
    return !(other < *this);
}
bool PDateTime::operator>(const PDateTime &other) const
{
    return other < *this;
}

bool PDateTime::operator>=(const PDateTime &other) const
{
    return !(*this < other);
}

bool PDateTime::isNull() const
{
    return date().isNull() && time().isNull();
}
bool PDateTime::isValid() const
{
    return date().isValid() && time().isValid();
}

PDate PDateTime::date() const
{
    return _date;
}
PTime PDateTime::time() const
{
    return _time;
}

int64 toMSecsSinceEpoch_helper(int64 jd, int msecs)
{
    int64 days = jd - JULIAN_DAY_FOR_EPOCH;
    int64 retval = (days * MSECS_PER_DAY) + msecs;
    return retval;
}
time_t SystemTimeToTimet(tm* st)
{
    return mktime(st);
}

int64 PDateTime::toMSecsSinceEpoch() const
{
    // utc time
    PDate utcDate;
    PTime utcTime;
    int var = date().year();
    var = date().month();
    var = date().day();

    var = time().hour();
    var = time().minute();
    var = time().second();
    utcDate.setDate(date().year(), date().month(), date().day());
    utcTime.setHMS(time().hour(), time().minute(), time().second(), time().msec());
    int64 vardd = toMSecsSinceEpoch_helper(utcDate.jd, utcTime.ds());
    return vardd;
}

int64 PDateTime::toTime_t() const
{
    CPFTime tmpTime(date().year(), date().month(), date().day(), time().hour(), time().minute(), time().second());
    return tmpTime.GetTime();
}
void PDateTime::setDate(const PDate &date)
{
    _date = date;
    _dateTime.tm_year = date.year() - 1900;
    _dateTime.tm_mon = date.month() - 1;
    _dateTime.tm_mday = date.day();
    this->d = mktime(&_dateTime);
}
void PDateTime::setTime(const PTime &time)
{
    _time = time;
    _dateTime.tm_hour = time.hour();
    _dateTime.tm_min = time.minute();
    _dateTime.tm_sec = time.second();
    this->d = mktime(&_dateTime);
}


void PDateTime::setTime_t(time_t t)
{
    *this = PDateTime::fromTime_t(t);
}

PDateTime PDateTime::fromTime_t(time_t t)
{
    CPFTime tmpTime(t);
    PDateTime dateTime;
    dateTime.setDate(PDate(tmpTime.GetYear(), tmpTime.GetMonth(), tmpTime.GetDay()));
    dateTime.setTime(PTime(tmpTime.GetHour(), tmpTime.GetMinute(), tmpTime.GetSecond(), 0));
    return dateTime;
}

PDateTime PDateTime::fromString(const string &s)
{
    if (s.empty()) {
        return PDateTime();
    }

    assert(!s.empty());
    PDate date = PDate::fromString(s.substr(0, 10));
    PTime time = PTime::fromString(s.substr(11, s.length()));
    PDateTime dt;
    dt.setDate(date);
    dt.setTime(time);
    return dt;
}

std::string PDateTime::toString()
{
    std::string dateStr = _date.toString();
    if (dateStr.empty())
        return "";

    std::string timeStr = _time.toString();
    if (timeStr.empty())
        return "";

    return dateStr + " " + timeStr;
}

PDateTime PDateTime::addDays(int days) const
{
    PDate dt = _date.addDays(days);
    PTime tmm = _time;
    return PDateTime(dt, tmm);
}
PDateTime PDateTime::addMonths(int months) const
{
    return PDateTime(this->_date.addMonths(months), this->_time);
}
PDateTime PDateTime::addYears(int years) const
{
    return PDateTime(this->_date.addYears(years), this->_time);
}
PDateTime PDateTime::addSecs(int secs) const
{
    int days = secs / SECS_PER_DAY;
    secs = secs % SECS_PER_DAY;
    return PDateTime(this->_date.addDays(days), this->_time.addSecs(secs));
}

PDateTime PDateTime::currentDateTime()
{
    // 获取系统时间与UTC事件的差值
    PDateTime dateTime;
    time_t rawtime = ::time(NULL);

    tm* st = localtime(&rawtime);

    dateTime.d = SystemTimeToTimet(st);

    dateTime.setDate(PDate(st->tm_year + 1900, st->tm_mon + 1, st->tm_mday));
    dateTime.setTime(PTime(st->tm_hour, st->tm_min, st->tm_sec, 0));

    return dateTime;
}

//int PDateTime::daysTo(const PDateTime &) const
//{
//
//}
//int PDateTime::secsTo(const PDateTime &) const
//{
//
//}




































//
//DateTime::DateTime(const time_t seconds)
//{
//	this->seconds = seconds;
//	this->date = *localtime(&this->seconds);
//}
//DateTime::DateTime(int year,int month,int day)
//{
//	tm t;
//	t.tm_year = year-1900;
//	t.tm_mon = month-1;
//	t.tm_mday = day;
//	t.tm_hour = 0;
//	t.tm_min = 0;
//	t.tm_sec = 0;
//	this->seconds = mktime(&t);
//	this->date = *localtime(&this->seconds);
//}
// 
//DateTime::DateTime(int year,int month,int day,int hour,int minute,int second)
//{
//	tm t;
//	t.tm_year = year-1900;
//	t.tm_mon = month-1;
//	t.tm_mday = day;
//	t.tm_hour = hour;
//	t.tm_min = minute;
//	t.tm_sec = second;
//	this->seconds = mktime(&t);
//	this->date = *localtime(&this->seconds);
// 
//}
// 
////DateTime::DateTime(std::string datetimeStr) //日期字符串格式 月/日/年 时:分:秒 例:02/03/2008 9:30:20 出错返回 01/01/1970 00:00:00
////{
////    this->InitByStr(datetimeStr,"%d/%d/%d %d:%d:%d");
////}
//
//DateTime::DateTime(std::string dateTimeStr)
//{
//
//	std::string strYear = dateTimeStr.substr(0, 4);
//	std::string strMonth = dateTimeStr.substr(5, 2);
//	std::string strDay = dateTimeStr.substr(8, 2);
//
//	tm t;
//	t.tm_year = atoi(strYear.c_str());
//	t.tm_mon = atoi(strMonth.c_str());
//	t.tm_mday = atoi(strDay.c_str());
//	t.tm_hour = 0;
//	t.tm_min = 0;
//	t.tm_sec = 0;
//}
//
//DateTime::DateTime(std::string datetimeStr,std::string formaterStr)
//{
//    this->InitByStr(datetimeStr,formaterStr);
//}
//DateTime DateTime::Parse(std::string datetimeStr)
//{
//    DateTime datetime(datetimeStr);
//    return datetime;
//}
//DateTime DateTime::Parse(const std::string datetimeStr,const std::string formaterStr)
//{
//    DateTime datetime(datetimeStr,formaterStr);
//    return datetime;
//}
// 
// 
//void DateTime::InitByStr(std::string dateTimeStr,std::string formaterStr)
//{
//	int year,month,day,hour,minutes,seconds;
//	sscanf(dateTimeStr.c_str(),formaterStr.c_str(),&year,&month,&day,&hour,&minutes,&seconds);    
//	if(year <1900)
//		year = 1970;
//	if(month <0)
//		month = 1;
//	if(day <0)
//		day = 1;
//	if(hour <0)
//		hour = 0;
//	if(minutes <0)
//		minutes = 0;
//	if(seconds <0)
//		seconds = 0;
//
//	tm t;
//	t.tm_year = year-1900;
//	t.tm_mon = month-1;
//	t.tm_mday = day;
//	t.tm_hour = hour;
//	t.tm_min = minutes;
//	t.tm_sec = seconds;
//	this->seconds = mktime(&t);
//	this->date = *localtime(&this->seconds);
//}
//
//int DateTime::DayOfWeek()
//{
//	return this->date.tm_wday;
//}
//int DateTime::DayOfYear()
//{
//	return this->date.tm_yday;
//}
//int DateTime::DaysInMonth(const int year,const int months)
//{
//	return 0;
//}
//
//bool DateTime::Equals(const DateTime *value1,const DateTime *value2)
//{
//	if(value1->seconds == value2->seconds)
//		return true;
//	return false;
//}
//int DateTime::GetDay()
//{    
//	return this->date.tm_mday;
//}
//
//int DateTime::GetHour()
//{
//	return this->date.tm_hour;
//}
//int DateTime::GetMinute()
//{
//	return this->date.tm_min;
//}
//int DateTime::GetMonth()
//{
//	return this->date.tm_mon;
//}
//
//DateTime DateTime::GetNow()
//{
//	DateTime datetime(time(0));
//	return datetime;
//}
//int DateTime::GetSecond()
//{
//	return this->date.tm_sec;
//}
//int DateTime::GetYear()
//{
//	return this->date.tm_year+1900;
//}
//
//void DateTime::AddYears(const time_t years)
//{
//	this->date.tm_year = this->date.tm_year+years;
//	this->seconds = mktime(&this->date);
//}
//void DateTime::AddMonths(const time_t months)
//{
//	int a  =(int)((this->date.tm_mon+months)/12);
//	this->date.tm_year = this->date.tm_year+a;
//	this->date.tm_mon = this->date.tm_mon+(int)((this->date.tm_mon+months)%12)-1;
//	this->seconds = mktime(&this->date);
//}
//
//void DateTime::AddDays(const time_t days)
//{    
//	this->AddHours(days*24);
//}
//void DateTime::AddHours(const time_t hours)
//{
//    this->AddMinutes(hours*60);
//}
//void DateTime::AddMinutes(const time_t minutes)
//{
//    this->AddSeconds(minutes *60);
//}
//
//void DateTime::AddSeconds(const time_t seconds)
//{
//    this->seconds = this->seconds+seconds;
//    this->date = *localtime(&this->seconds);
//}
//void DateTime::AddWeeks(const time_t weeks)
//{
//    this->AddDays(weeks*7);
//}
//
//
//int DateTime::Compare(const DateTime *t1,const DateTime *t2)
//{
//	if( t1->seconds == t2->seconds)
//		return 0;
//	if(t1->seconds < t2->seconds)
//		return -1;
//	return 1;
//}
//
//
//int DateTime::CompareTo(const DateTime *value)
//{
//    if(this->seconds == value->seconds)
//        return 0;
//    if(this->seconds <value->seconds)
//        return -1;
//    return 1;
//}
//
//std::string DateTime::ToShortDateString()
//{
//	return this->ToString("%Y-%m-%d");
//}
//
//std::string DateTime::ToString()
//{
//    return this->ToString("%Y-%m-%d %H:%M:%S");
//}
//std::string DateTime::ToString(const std::string formaterStr)
//{
//    //strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", localtime(&t)); 
//    char s[256];
//    strftime(s, sizeof(s), formaterStr.c_str(), &this->date);
//    std::string str(s);
//    return str;
//}
//
//
//bool DateTime::operator != (DateTime &datetime)
//{
//	if(this->seconds != datetime.seconds)
//		return true;
//	return false;
//}
//bool DateTime::operator > (DateTime &dateTime)
//{
//	if(this->seconds > dateTime.seconds)
//		return true;
//	return false;
//}
//bool DateTime::operator < (DateTime &datetime)
//{
//	if(this->seconds <datetime.seconds)
//		return true;
//	return false;
//}
//bool DateTime::operator >=(DateTime &datetime)
//{
//	if(this->seconds >= datetime.seconds)
//		return true;
//	return false;
//}
//
//bool DateTime::operator <=(DateTime &datetime)
//{
//	if(this->seconds <= datetime.seconds)
//		return true;
//	return false;
//}
//DateTime::~DateTime(void)
//{
//
//}
//
//void DateTime::setDate(DateTime date)
//{
//	this->date.tm_year = date.date.tm_year;
//	this->date.tm_mon = date.date.tm_mon;
//	this->date.tm_mday = date.date.tm_mday;
//	this->seconds = mktime(&this->date);
//}
//
//void DateTime::setTime(DateTime time)
//{
//	this->date.tm_hour = time.date.tm_hour;
//	this->date.tm_min = time.date.tm_min;
//	this->date.tm_sec = time.date.tm_sec;
//	this->seconds = mktime(&this->date);
//}
//

