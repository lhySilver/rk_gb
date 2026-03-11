#include "PFMulticastSocket.h"
#include "ShareSDK.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/route.h>
#include <errno.h>

#ifndef RTF_MULTICAST
//http://www.scs.stanford.edu/histar/src/pkg/uclibc/include/net/route.h 老版的定义
#define RTF_MULTICAST	0x20000000
//新版本的定义在 linux/in_route.h
//#define RTCF_MULTICAST	0x20000000
#endif

CPFMulticastSocket::CPFMulticastSocket(void)
{
	
}

CPFMulticastSocket::~CPFMulticastSocket(void)
{
}

int CPFMulticastSocket::Create_UDPSocket(tint32 &hSock, const char *sEthName, int iReuse)
{
	hSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (INVALID_SOCKET == hSock)
	{
		return SOCKET_ERROR;
	}

	if (SOCKET_ERROR == setsockopt(hSock, SOL_SOCKET, SO_REUSEADDR, 
		(char *)&iReuse, sizeof(iReuse)))
	{
		closesocket(hSock);
		return SOCKET_ERROR;
	}

    //必须绑定网卡
    struct ifreq intf;
    memset(&intf, 0, sizeof(intf));
    strncpy(intf.ifr_ifrn.ifrn_name, sEthName, IFNAMSIZ);
    if (0 != (setsockopt(hSock, SOL_SOCKET, SO_BINDTODEVICE, (char*)&intf, sizeof(intf))))
    {
        closesocket(hSock);
        return SOCKET_ERROR;
    }

	return 0;
}

int CPFMulticastSocket::Create_ONVIFSearchUDPSocket(tint32 &hSock, const char *sEthName, int iReuse)
{
	hSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (INVALID_SOCKET == hSock)
	{
		return SOCKET_ERROR;
	}

	if (SOCKET_ERROR == setsockopt(hSock, SOL_SOCKET, SO_REUSEADDR, 
		(char *)&iReuse, sizeof(iReuse)))
	{
		closesocket(hSock);
		return SOCKET_ERROR;
	}

    //不能绑定网卡
	#if 0
    struct ifreq intf;
    memset(&intf, 0, sizeof(intf));
    strncpy(intf.ifr_ifrn.ifrn_name, sEthName, IFNAMSIZ);
    if (0 != (setsockopt(hSock, SOL_SOCKET, SO_BINDTODEVICE, (char*)&intf, sizeof(intf))))
    {
        closesocket(hSock);
        return SOCKET_ERROR;
    }
    #endif
	
	return 0;
}


int CPFMulticastSocket::Select_Recv(tint32 hSock, fd_set& fds, struct timeval* timeout)
{
	FD_ZERO(&fds);
	FD_SET(hSock, &fds);
	int ret = select(hSock + 1, &fds, NULL, NULL, timeout);
	return ret;
}

void CPFMulticastSocket::Shutdown_Socket(tint32 &hSock)
{
	if (hSock != INVALID_SOCKET)
	{
        shutdown(hSock, SHUT_RDWR);
		close(hSock);
		hSock = INVALID_SOCKET;
	}
}

bool CPFMulticastSocket::SetNonBlock(tint32 &hSock)
{
	int iSave = fcntl(hSock, F_GETFL);
	int ret = fcntl(hSock, F_SETFL, iSave | O_NONBLOCK);
	return (ret==0);
}

int CPFMulticastSocket::Multicast_PrepareSendSocket(tint32 hSock, tuint32 dwLocalIP)
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = 0;
	addr_in.sin_addr.s_addr = INADDR_ANY; //注意和win32不一样

	if (SOCKET_ERROR == bind(hSock, (struct sockaddr* )&addr_in, sizeof(addr_in)))
	{
		closesocket(hSock);
		return SOCKET_ERROR;
	}
	//特别指定发出数据的网卡
	struct in_addr addr;
	addr.s_addr = dwLocalIP;
	if (SOCKET_ERROR == setsockopt(hSock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr)))
	{
        closesocket(hSock);
		return SOCKET_ERROR;
	}
	return 0;
}



int CPFMulticastSocket::Multicast_PrepareONVIFSearchSocket(tint32 hSock, tuint32 dwLocalIP,tuint32 dwLocalPort)
{
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(dwLocalPort);
    addr_in.sin_addr.s_addr = dwLocalIP;
    if (SOCKET_ERROR == bind(hSock, (struct sockaddr* )&addr_in, sizeof(addr_in)))
    {
        closesocket(hSock);
        return SOCKET_ERROR;
    }
	
	//不能指定发出数据的网卡
#if 0
    struct in_addr addr;
    addr.s_addr = dwLocalIP;
    if (SOCKET_ERROR == setsockopt(hSock, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr)))
    {
        closesocket(hSock);
        return SOCKET_ERROR;
    }
#endif

    return 0;
}



int CPFMulticastSocket::Multicast_PrepareRecvSocket(tint32 hSock, tuint32 dwLocalIP, tuint16 wMulticastPort)
{
	struct sockaddr_in addr_in;
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(wMulticastPort);
	//多网卡时不能指定IP地址
	//addr_in.sin_addr.s_addr = dwLocalIP;
	addr_in.sin_addr.s_addr = INADDR_ANY;
	
	if (SOCKET_ERROR == bind(hSock, (struct sockaddr* )&addr_in, sizeof(addr_in)))
	{
		closesocket(hSock);
		return SOCKET_ERROR;
	}

    UNREFERENCED_PARAMETER(dwLocalIP);
	return 0;
}

int CPFMulticastSocket::Multicast_AddMemberShip(tint32 hSock, tuint32 dwMulticastAddr, tuint32 dwLocalIP)
{

	struct ip_mreq ipMReq;
	memset(&ipMReq, 0, sizeof(ipMReq));
    ipMReq.imr_multiaddr.s_addr = dwMulticastAddr; //多播组IP
	//多网卡时必须指定IP地址，否则可能收不到
    ipMReq.imr_interface.s_addr = dwLocalIP; //本地IP

	int ret = setsockopt(hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char* )&ipMReq, sizeof(ipMReq));
	return ret;
}

int CPFMulticastSocket::Multicast_DropMemberShip(tint32 hSock, tuint32 dwMulticastAddr, tuint32 dwLocalIP)
{
	struct ip_mreq ipMReq;
	memset(&ipMReq, 0, sizeof(ipMReq));
	ipMReq.imr_multiaddr.s_addr = dwMulticastAddr;  //多播组IP
    ipMReq.imr_interface.s_addr = dwLocalIP; //本地IP
	int ret = setsockopt(hSock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&ipMReq, sizeof(ipMReq));
	return ret;
}

int CPFMulticastSocket::Multicast_ReceiveFrom(tint32 hSock, tuint32 dwLocalIP, tuint16 wLocalPort, char* pszBuf, int nLen)
{
	struct sockaddr_in recvAddr;
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(wLocalPort);
	recvAddr.sin_addr.s_addr = INADDR_ANY;  //dwLocalIP;

	unsigned int iRecvLen = sizeof(recvAddr);
	int iRet = recvfrom(hSock, pszBuf, nLen, 0, (struct   sockaddr *)   &recvAddr, &iRecvLen);

    UNREFERENCED_PARAMETER(dwLocalIP);
    return iRet;
}

int CPFMulticastSocket::Multicast_SetNIC_AllMulti(tint32 hSock, const char *sEthName, bool bAllMulti)
{
    struct ifreq ifr;
    int ret;
    strcpy(ifr.ifr_name, sEthName);
    ret = ioctl(hSock, SIOCGIFFLAGS, &ifr);
    if (ret == -1)
    {
        //AP_WARN_PRINTF("get nic flag fail, errno=%d", errno);
        return -1;
    }
    if (bAllMulti) ifr.ifr_flags |= IFF_ALLMULTI;
    else ifr.ifr_flags &= ~IFF_ALLMULTI;
    ret = ioctl(hSock, SIOCSIFFLAGS, &ifr);
    if (ret!=0)
    {
        //AP_WARN_PRINTF("SetNIC_AllMulti fail, errno=%d", errno);
    }
    return ret;
}


char *get_name(char *name, char *p)
{
    while (isspace(*p))
        p++;
    while (*p)
    {
        if (isspace(*p))
            break;
        if (*p == ':')
        {	/* could be an alias */
            char *dot = p, *dotname = name;
            *name++ = *p++;
            while (isdigit(*p))
                *name++ = *p++;
            if (*p != ':')
            {	/* it wasn't, backup */
                p = dot;
                name = dotname;
            }
            if (*p == '\0')
                return NULL;
            p++;
            break;
        }
        *name++ = *p++;
    }
    *name++ = '\0';
    return p;
}

//所有当前系统所有网卡名
int  CPFMulticastSocket::GetNICNameInproc( std::list<CPFHelper::TNICInfo> &NICList)
{
    return CPFHelper::GetNICList(NICList);
}




int GetNICInfo(int fd, struct ifreq *pIfr,CPFMulticastSocket::NICInfo &info)
{

    memset(&info, 0, sizeof(CPFMulticastSocket::NICInfo));
    strncpy(info.eth_name, pIfr->ifr_name,sizeof(info.eth_name));

    if (ioctl(fd, SIOCGIFADDR, pIfr) != -1)
    {
        info.IP = ((struct sockaddr_in*) (&pIfr->ifr_addr))->sin_addr.s_addr;
    }
    else
    {
        return -1;
    }

    if (ioctl(fd, SIOCGIFNETMASK, pIfr) != -1)
    {
        info.Netmask = ((struct sockaddr_in*) (&pIfr->ifr_addr))->sin_addr.s_addr;
    }    
    return 0;
}


int  CPFMulticastSocket::GetSingleNICInfo(char * pEth,NICInfo &info)
{
    int fd;
    struct ifreq tmpIfr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        return -1;
    }
    else
    {
        memset(&tmpIfr, 0, sizeof(struct ifreq));
        strcpy(tmpIfr.ifr_name, pEth);
        GetNICInfo(fd, &tmpIfr, info);
        close(fd);
    }
    return 0;
}

