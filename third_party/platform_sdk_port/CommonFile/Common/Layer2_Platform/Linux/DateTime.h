#pragma once
#include "time.h"
#include <string>
#include <iostream>

using namespace std;
typedef unsigned int            uint;
typedef unsigned long long      uint64;
typedef long long int           int64;

enum {
    FIRST_YEAR = -4713,
    FIRST_MONTH = 1,
    FIRST_DAY = 2,  // ### Qt 5: make FIRST_DAY = 1, by support jd == 0 as valid
    SECS_PER_DAY = 86400,
    MSECS_PER_DAY = 86400000,
    SECS_PER_HOUR = 3600,
    MSECS_PER_HOUR = 3600000,
    SECS_PER_MIN = 60,
    MSECS_PER_MIN = 60000,
    JULIAN_DAY_FOR_EPOCH = 2440588 // result of julianDayFromGregorianDate(1970, 1, 1)
};
class PTime;
class PDateTime;

class PDate
{
    friend class PDateTime;
public:
    PDate() : jd(0) {}
    PDate(int y, int m, int d);
    bool isNull() const { return jd == 0; }
    bool isValid() const{return !isNull();};

    int year() const;
    int month() const;
    int day() const;
    int dayOfWeek() const;
    int dayOfYear() const;
    int daysInMonth() const;
    int daysInYear() const;
    int weekNumber(int *yearNum = 0) const;

    bool setDate(int year, int month, int day);
    void getDate(int *year, int *month, int *day);

    PDate addDays(int days) const;
    PDate addMonths(int months) const;
    PDate addYears(int years) const;
    int daysTo(const PDate &) const;

    bool operator==(const PDate &other) const {
        return jd == other.jd;
    }
    bool operator!=(const PDate &other) const {
        return jd != other.jd;
    }
    bool operator<(const PDate &other) const {
        return jd < other.jd;
    }
    bool operator<=(const PDate &other) const {
        return jd <= other.jd;
    }
    bool operator>(const PDate &other) const {
        return jd > other.jd;
    }
    bool operator>=(const PDate &other) const {
        return jd >= other.jd;
    }
    PDate& operator=(const PDate& other) {
        this->jd = other.jd; return *this;
    }
    static PDate currentDate();

    // yyyy-mm-dd
    static PDate fromString(const string &s);
    string toString();
    static bool isLeapYear(int year);
    static bool isValid(int y, int m, int d);

private:
    // ### Qt 5: remove these two functions
    static uint gregorianToJulian(int y, int m, int d);
    static void julianToGregorian(uint jd, int &y, int &m, int &d);

    static inline PDate fromJulianDay(int jd) {
        PDate d; d.jd = jd; return d;
    }
    inline int toJulianDay() const {
        return jd;
    }

private:
    uint jd;
};


class PTime
{
    friend class PDateTime;
public:
    PTime() : mds(NullTime) {}
    PTime(int h, int m, int s = 0, int ms = 0);

    bool isNull() const {
        return mds == NullTime;
    }
    bool isValid() const;

    bool operator==(const PTime &other) const {
        return mds == other.mds;
    }
    bool operator!=(const PTime &other) const {
        return mds != other.mds;
    }
    bool operator<(const PTime &other) const {
        return mds < other.mds;
    }
    bool operator<=(const PTime &other) const {
        return mds <= other.mds;
    }
    bool operator>(const PTime &other) const {
        return mds > other.mds;
    }
    bool operator>=(const PTime &other) const {
        return mds >= other.mds;
    }

    bool setHMS(int h, int m, int s, int ms = 0);
    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;

    PTime addSecs(int secs) const;
    int secsTo(const PTime &) const;

    PTime addMSecs(int ms) const;
    int msecsTo(const PTime &) const;

    static bool isValid(int h, int m, int s, int ms = 0);
    static PTime currentTime();
    // hh:mm:ss
    static PTime fromString(const string &s);
    string toString();

private:
    void start();
    int restart();
    int elapsed() const;

private:
    enum TimeFlag {
        NullTime = -1
    };
    inline int ds() const {
        return mds == -1 ? 0 : mds;
    }
    int mds;
};

class PDateTime
{
public:
    PDateTime();
    explicit PDateTime(const PDate &);
    PDateTime(const PDate &, const PTime &);
    PDateTime(const PDateTime &other);
    ~PDateTime();

    PDateTime &operator=(const PDateTime &other);
    bool operator==(const PDateTime &other) const;
    bool operator!=(const PDateTime &other) const;
    bool operator<(const PDateTime &other) const;
    bool operator<=(const PDateTime &other) const;
    bool operator>(const PDateTime &other) const;
    bool operator>=(const PDateTime &other) const;

    bool isNull() const;
    bool isValid() const;

    PDate date() const;
    PTime time() const;

    void setDate(const PDate &date);
    void setTime(const PTime &time);

    void setTime_t(time_t t);
    int64 toTime_t() const;

    // yyyy-mm-dd hh:mm:ss
    static PDateTime fromString(const string &s);
    static PDateTime currentDateTime();
    static PDateTime fromTime_t(time_t t);
    string toString();

    PDateTime addYears(int years) const;
    PDateTime addMonths(int months) const;
    PDateTime addDays(int days) const;
    PDateTime addSecs(int secs) const;

private:
    int64 toMSecsSinceEpoch() const;

private:
    PDate _date;
    PTime _time;
    tm _dateTime;

    uint64	d;
    static time_t  deltaTime;   // diff between utc and local seconds
};




































//
//class DateTime
//{
//public:   
//	DateTime(){};
//	DateTime(time_t seconds);
//	DateTime(int year,int month,int day);
//	DateTime(int year,int month,int day,int hour,int minute,int second);
//	//DateTime(std::string datetimeStr);//日期字符串格式 年/月/日 时:分:秒 例:2008/02/03 9:30:20 出错返回 01/01/1970 00:00:00
//	// 修改为 yyyy-mm-dd
//	DateTime(std::string dateTimeStr);
//	DateTime(std::string datetimeStr,std::string formaterStr);
//private:
//	time_t seconds;//自1970起的秒数
//	tm date;
//public:
//	void setDate(DateTime date);
//	void setTime(DateTime time);
//
//
//	void AddYears(const time_t years); //将指定的年份数加到此实例的值上。  
//	void AddMonths(const time_t Months);//将指定的月份数加到此实例的值上。  
//	void AddDays(const time_t days); //将指定的天数加到此实例的值上。 
//	void AddHours(const time_t hours);//将指定的小时数加到此实例的值上。 
//	void AddMinutes(const time_t minutes);//将指定的分钟数加到此实例的值上。  
//	void AddSeconds(const time_t seconds); //将指定的秒数加到此实例的值上。
//	void AddWeeks(const time_t weeks);//将指定的周数加到些实上的值上。
//	static int Compare(const DateTime *value1,const DateTime *value2);//对两个 DateTime 的实例进行比较，并返回一个指示第一个实例是早于、等于还是晚于第二个实例的整数。  返回值：小于零 value1 小于 value2。 零 value1 等于 value2。 大于零 value1 大于 value2。 
//	int CompareTo(const DateTime *value);//小于零 此实例小于 value。 零 此实例等于 value。 大于零 此实例大于 value。 
//
//
//	int DaysInMonth(const int year,const int months);//返回指定年和月中的天数。    
//
//	static bool Equals(const DateTime *value1,const DateTime *value2);
//	static DateTime Parse(std::string datetimeStr);//日期字符串格式 月/日/年 时:分:秒 例:02/03/2008 9:30:20 出错返回 01/01/1970 00:00:00
//	static DateTime Parse(std::string dateTimeStr,std::string formaterStr);
//	std::string ToShortDateString();//将当前 DateTime 对象的值转换为其等效的短日期字符串表示形式。  
//	std::string ToString();
//	std::string ToString(const std::string formaterStr);//formaterStr = "%Y-%m-%d %H:%M:%S" %Y=年 %m=月 %d=日 %H=时 %M=分 %S=秒
//public:    
//	uint val(){return seconds;};
//	int GetYear();//获取此实例所表示日期的年份部分。
//	int GetMonth();//获取此实例所表示日期的年份部分。
//	int GetDay();// 获取此实例所表示的日期为该月中的第几天。
//	int GetHour();//获取此实例所表示日期的小时部分。 
//	int GetMinute();//获取此实例所表示日期的分钟部分
//	int GetSecond();//获取此实例所表示日期的秒部分。 
//	int DayOfWeek(); //获取此实例所表示的日期是星期几。
//	int DayOfYear();//记录今天是一年里面的第几天,从1月1日起,0-365
//	static DateTime GetNow(); //返回当前日期是间
//public:
//	bool operator == (DateTime &dateTime);
//	bool operator > (DateTime &dateTime);
//	bool operator < (DateTime &DateTime);
//	bool operator >= (DateTime &DateTime);
//	bool operator <= (DateTime &DateTime);
//	bool operator != (DateTime &DateTime);
//
//private:
//	void InitByStr(std::string dateTimeStr,std::string formaterStr);
//public:
//	~DateTime(void);
//};
