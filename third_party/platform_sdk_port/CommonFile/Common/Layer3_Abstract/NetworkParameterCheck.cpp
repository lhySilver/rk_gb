#include "NetworkParameterCheck.h"
#include "_ErrorCodeDef.h"

CNetworkParameterCheck::CNetworkParameterCheck()
{
}

CNetworkParameterCheck::~CNetworkParameterCheck()
{
}

bool CNetworkParameterCheck::IsValidNetwork(tuint32 dwIP, tuint32 dwSubnet, tuint32 dwGateway, int &errCode)
{
    //IP和子网掩码的或不能全为1
    if(0xffffffff == (dwIP|dwSubnet))
    {
        errCode = USER_ERROR_IP_MASK_ALL1;
        return false;
    }
    //检查主机号，不能为0
    if(0 == (dwIP&~dwSubnet))
    {
        errCode = USER_ERROR_IP_MASK_ALL0;
        return false;
    }

    if (dwGateway!=0)
    {
        //检查网关主机号
        if(0xffffffff == (dwGateway|dwSubnet))
        {
            errCode = USER_ERROR_ROUTE_MASK_ALL1;
            return false;
        }
        if(0 == (dwGateway&~dwSubnet))
        {
            errCode = USER_ERROR_ROUTE_MASK_ALL0;
            return false;
        }
    }

    ////检验网络号
    BYTE nIPField0 = 0,nRouteField0 = 0 ;
    nIPField0 = (BYTE)(dwIP)&0xff;
    nRouteField0= (BYTE)(dwGateway)&0xff ;

    if((nIPField0 == 127) || (nRouteField0 == 127))
    {
        errCode = USER_ERROR_USE_LOOPBACK;
        return false;
    }

    bool ipnetnumerr=(nIPField0 == 0) || (nIPField0 > 223);
    bool routenetnumerr=(nRouteField0 == 0) || (nRouteField0 > 223);
    if(ipnetnumerr || (routenetnumerr && dwGateway != 0) )
    {
        errCode = USER_ERROR_IP_ROUTE_INVALID;
        return false;
    }

    //掩码中间出现了断位
    tuint32 dwTemp = ntohl(dwSubnet);
    bool maskError = true;
    do
    {
        if ((nIPField0 >= 1) && (nIPField0 <= 126))
        {
            if (dwTemp < 192)
            {
                maskError = false;
                break;
            }
        }
        else if ((nIPField0 >= 128) && (nIPField0 <= 191))
        {
            if (dwTemp < 192)
            {
                maskError = false;
                break;
            }
        }
        else if((nIPField0 >= 192) && (nIPField0 <= 223))
        {
            if (dwTemp < 224)
            {
                maskError = false;
                break;
            }
        }

        for(int i=0;i<32&&dwTemp;i++)
        {
            if(!(dwTemp&(0x01<<31)))		//中间出现了为断位
            {
                maskError = FALSE;
                break;
            }
            dwTemp<<=1;
        }
    } while (0);

    if(false  == maskError)
    {
        errCode = USER_ERROR_MASK_NOT_CONTINE;
        return false;
    }

    if((dwIP&dwSubnet) != (dwGateway&dwSubnet) && (dwGateway != 0) )
    {
        errCode = USER_ERROR_DIFFERENT_SEGMENT;
        return false;
    }

    errCode = 0;
    return true;
}
