#ifndef __NetworkParameterCheck_H__
#define __NetworkParameterCheck_H__

#include "base_type_define.h"

class CNetworkParameterCheck
{
public:
    //是否有效的网络，如果无效，errCode为_ErrorCodeDef.h 指定的错误码
    //注意：dwIP/dwSubnet/dwGateway 都是通过 CSysHelper::StrToIPAddress 或 inet_aton 转换得到的，是网络序
    static bool IsValidNetwork(tuint32 dwIP, tuint32 dwSubnet, tuint32 dwGateway, int &errCode);
private:
    CNetworkParameterCheck();
    ~CNetworkParameterCheck();
};

#endif // __NetworkParameterCheck_H__
