#ifndef __PFMulticastSocket_H__
#define __PFMulticastSocket_H__

#include "base_type_define.h"
#include "PFHelper.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <list>
#include <string>

#define SOCKET_ERROR (-1)
#define closesocket close

class CPFMulticastSocket
{
public:

    CPFMulticastSocket(void);
    ~CPFMulticastSocket(void);
    //创建UDP Socket，返回0为成功，-1为失败
	static int Create_UDPSocket(tint32 &hSock, const char *sEthName, int iReuse);
	
	//added by chenqiang for onvif probe without gsoap
	static int Create_ONVIFSearchUDPSocket(tint32 &hSock, const char *sEthName, int iReuse);
	

	//检查是否有接收到数据，返回0为超时，返回-1有错误，返回>0表示有数据
	static int Select_Recv(tint32 hSock, fd_set& fds, struct timeval* timeout);
	
	//关闭socket
	static void Shutdown_Socket(tint32 &hSock);

    //设置非阻塞
    static bool SetNonBlock(tint32 &hSock);
public:
	/////////////// 多播相关

	//准备用于发送多播消息的socket，返回0为成功，-1为失败
	static int Multicast_PrepareSendSocket(tint32 hSock, tuint32 dwLocalIP);
    
	//added by chenqiang for onvif probe without gsoap
    static int Multicast_PrepareONVIFSearchSocket(tint32 hSock, tuint32 dwLocalIP,tuint32 dwLocalPort);
	
	//准备用于接收多播消息的socket，返回0为成功，-1为失败
	static int Multicast_PrepareRecvSocket(tint32 hSock, tuint32 dwLocalIP, tuint16 wMulticastPort);

	//加入多播组，返回 0 为成功， -1 为失败
	static int Multicast_AddMemberShip(tint32 hSock, tuint32 dwMulticastAddr, tuint32 dwLocalIP);

	//退出多播组，返回 0 为成功， -1 为失败
	static int Multicast_DropMemberShip(tint32 hSock, tuint32 dwMulticastAddr, tuint32 dwLocalIP);

	//接收多播数据，返回接收到的数据大小
	static int Multicast_ReceiveFrom(tint32 hSock, tuint32 dwLocalIP, tuint16 wLocalPort, char* pszBuf, int nLen);

    //设置网卡AllMulti模式 (Linux多网卡组播，必须设置，否则接收不到数据)。 返回0为成功，-1为失败
    static int Multicast_SetNIC_AllMulti(tint32 hSock, const char *sEthName, bool bAllMulti);

    //尝试获取一个网卡的信息
    typedef struct _NICInfo {
        char eth_name[48];  //网卡标识名称 (Linux为eth0, eth1，Win32为GUID)
        tuint32 IP;
        tuint32 Netmask;
		_NICInfo()
		{
			memset(eth_name,0,sizeof(eth_name));
			IP = 0;
			Netmask = 0;
		}
    } NICInfo, *pNICInfo;

    static int  GetNICNameInproc( std::list<CPFHelper::TNICInfo> &NICList);

    static int  GetSingleNICInfo(char * pEth,NICInfo &info);

private:

};

#endif //__PFMulticastSocket_H__
