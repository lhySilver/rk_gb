#ifndef MEDIAPLAY_COMMON_H_ 
#define MEDIAPLAY_COMMON_H_

#include "base_type_define.h"
#include <list>
#include <string>
#include "../../../Common/Layer1_Common/IASrvControl/ClientIASrvProtocol.h"

struct FaceAttributeConvert{
    static std::list<string> getAttributeStrList(FaceAttribute* face){
        std::list<std::string> strList;
        // 目标置信度
        float confidence = 0;
        // 人脸属性
        return strList;
    }
};

namespace MediaPlayHelper
{
    static std::list<std::string> stringSplit(const  std::string& s, const std::string& delim=","){
        std::list<std::string> elems;
        size_t pos = 0;
        size_t len = s.length();
        size_t delim_len = delim.length();
        if (delim_len == 0) return elems;
        while (pos < len)
        {
            int find_pos = s.find(delim, pos);
            if (find_pos < 0)
            {
                elems.push_back(s.substr(pos, len - pos));
                break;
            }
            elems.push_back(s.substr(pos, find_pos - pos));
            pos = find_pos + delim_len;
        }
        return elems;
    }
}

struct BlockAreaInfo
{
    RECT	szRect[4];	
    tint32	nCount;		//记录显示的个数
    tint32	nColorType;	//0=红，1=绿，2=蓝
    bool	bFillColor;	//是否填充颜色
    BlockAreaInfo()
    {
        memset(szRect, 0, sizeof(szRect));
        nCount = 0;
        nColorType = 1;
        bFillColor = true;
    };
};

struct PolygonAreaInfo
{
    POINT szPoint[10];
    tint32 nCount;	//记录有效点的个数
    PolygonAreaInfo()
    {
        memset(szPoint, 0, sizeof(szPoint));
        nCount = 0;
    }
};


#endif //MEDIAPLAY_COMMON_H_
