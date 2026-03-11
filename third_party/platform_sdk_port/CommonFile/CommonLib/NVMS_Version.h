#ifndef _NVMS_VERSION_H_
#define _NVMS_VERSION_H_
#include <string>
#include "NVMS_VersionType.h"
using namespace std;
static std::string GetNvmsVersion()
{
    std::string monthes[] =
    {
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec",
    };

    std::string dateStr = __DATE__;

    int year = atoi(dateStr.substr(dateStr.length() - 1).c_str());

    int month = 0;
    for (int i = 0; i < 12; i++)
    {
        if (dateStr.find(monthes[i]) != string::npos)
        {
            month = i + 1;
            break;
        }
    }

    std::string dayStr = dateStr.substr(4, 2);
    int day = atoi(dayStr.c_str());

    char version[20];
    sprintf(version, "2.1.2.%01d%02d%02d", year, month, day);

    return version;
}

#endif