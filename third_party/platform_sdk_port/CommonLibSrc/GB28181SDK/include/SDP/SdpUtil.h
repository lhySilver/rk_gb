#ifndef CSDPUTIL_H
#define CSDPUTIL_H
#include "GB28181Defs.h"
#include <string>
class CSdpUtil
{
public:
    CSdpUtil();
    static void ToString(const MediaInfo* media,  std::string&  );
    static bool String2MediaInfo(const char* str, MediaInfo* output );
};

#endif // CSDPUTIL_H
