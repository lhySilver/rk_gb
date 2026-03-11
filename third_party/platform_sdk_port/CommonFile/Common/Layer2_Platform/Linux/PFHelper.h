
#ifndef __PF_HELPER__
#define __PF_HELPER__

#include <list>
#include <string>
#include "base_type_define.h"


#ifndef SIOCETHTOOL
#define SIOCETHTOOL     0x8946
#endif
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif
/* CMDs currently supported */
#define ETHTOOL_GSET        0x00000001 /* Get settings. */
#define ETHTOOL_SSET        0x00000002 /* Set settings. */
/* This should work for both 32 and 64 bit userland. */
struct _ethtool_cmd_
{
        tuint32   cmd;
        tuint32   supported;      /* Features this interface supports */
        tuint32   advertising;    /* Features this interface advertises */
        tuint16   speed;          /* The forced speed, 10Mb, 100Mb, gigabit */
        tuint8    duplex;         /* Duplex, half or full */
        tuint8    port;           /* Which connector port */
        tuint8    phy_address;
        tuint8    transceiver;    /* Which transceiver to use */
        tuint8    autoneg;        /* Enable or disable autonegotiation */
        tuint32   maxtxpkt;       /* Tx pkts before generating tx int */
        tuint32   maxrxpkt;       /* Rx pkts before generating rx int */
        tuint32   reserved[4];
};

typedef struct CPU_PACKED         //定义一个cpu occupy的结构体
{
    char name[20];             //定义一个char类型的数组名name有20个元素
    unsigned int user;        //定义一个无符号的int类型的user
    unsigned int nice;        //定义一个无符号的int类型的nice
    unsigned int system;    //定义一个无符号的int类型的system
    unsigned int idle;         //定义一个无符号的int类型的idle
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
}CPU_OCCUPY;

//用于父进程通知子进程执行外部命令的命名管道
#define SystemCall_Pipe_Server "/tmp/SystemCall_pipe_Server_89Io2"
#define SystemCall_Pipe_Client "/tmp/SystemCall_pipe_Client_89Io2"
//命名信号量会自动存在/run/shm/下，并且加sem. 前缀 ，这里不要再带路径了
#define SystemCall_LockFile    "SystemCall_LockFile_89Io2"

#define GetMBFromBytes(x)	(x >>20)
#define GetGBFromBytes(x)	(x >>30)

#define GetBytesFromMB(x)	(x << 20)
#define GetBytesFromGB(x)	(x << 30)

class CPFHelper
{
public:
    //
    static bool GetModuleFreeSpace(char* modulePath,  tuint64& freeSizeForCaller, tuint64& freeSizeForAll, tuint64& totalSize);

    static tint32 GetCPUCount();///获取CPU的个数
    static tint32 GetPageSize();
    static HANDLE CreateThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, tuint_ptr* lpThreadId);
    static void DestroyThread(HANDLE &hThread);
	static void DetachThread(HANDLE &hThread);//销毁线程句柄但不等待线程退出
    static bool SetCurrentThreadName(const char *pName,const char *pFileName,int fileline);
	static bool AttachThreadToCPU(HANDLE hThread,int cpuindex);//暂时只支持单线程绑定到特定CPU
    static bool CreateGUID(GUID &guid);
	//根据当前时间创建GUID,主要用于解决排序问题
	static bool CreateGUIDByCurTime(GUID &guid);
	//根据父节点的GUID创建子节点,主要用于解决排序问题
	static bool CreateGUIDByParentGUID(GUID parentGUID,unsigned short nIndex,GUID &guid);//设备下面创建通道，输入，输出  报警主机下面创建子系统、防区 门禁考勤系统下面创建设备门读头 用到
    static void Sleep(tuint32 ms);//ms单位为毫秒
    static void SetLastError(tuint32 errorcode);
    static tuint32 GetLastError();
    static tuint32 GetTickCount();
    static tuint_ptr GetThreadID();
    static tuint32 GetThreadID(HANDLE hThread);
    //取得执行程序所在文件夹，最末带分隔符
    static tuint32 GetModuleFileDir(HANDLE hModule, char *lpFilename, tuint32 dwSize);
	//取得执行程序所在文件夹，最末带分隔符
    static tuint32 GetModuleFilePath(HANDLE hModule, char *lpFilename, tuint32 dwSize);
    //取得执行程序所在文件夹，最末带分隔符
    static tuint32 GetModuleFileName(HANDLE hModule, char *lpFilename, tuint32 dwSize);
	static tuint32 GetConfigFileDir(char *lpFilename, tuint32 dwSize);
    //返回文件夹分隔符
    static const char DirectorySeparator;
    //获取文件夹下的文件列表，路径不能带通配符
    static bool GetFileList(const char *szPath, std::list<std::string> &FileList);

	static bool GetFileListRecursive(std::string rootPath, std::list<std::string>& fileList);
    //强制将操作系统内存中的文件缓冲内容写到硬盘
    static void Sync(void);
    //强制要求操作系统回收缓存
    static void ReleaseOSCachedMem(void);
    
	static tuint32 GetAppCurrentMemoryRSS(tuint32 &dwPID);

    static bool IPv6AddressToStr(const struct in6_addr &addr, char *pStrIP, int strLen);
    static bool StrToIPv6Address(const char *strIP, struct in6_addr &addr);
    static bool Rename(const char *szOld,const char *szNew);
    ///////////////////// 内存分配相关
#define MEM_COMMIT          0x1000
#define MEM_RESERVE         0x2000
#define MEM_TOP_DOWN       	0x100000
#define MEM_DECOMMIT		0x4000
#define MEM_RELEASE         0x8000
#define PAGE_READWRITE      0x04

	//cnt[5]:数据个数>=5, 字符串中，cnt[0]:各种长度字符的总和。cnt[1] 长度为1的字符总和, 也就是源字符串中ASCII字符的个数。
	//cnt[2] 长度为2的字符总和。cnt[3] 长度为3的字符总和。cnt[4] 长度为4的字符总和。
	static int GetUtf8Length( char const *str, int *cnt = 0);
	//返回max个utf-8字符范围内的字符串的数量如StringCutBytesUtf8("12例如",3)返回5即"12例"
	static int StringCutBytesUtf8(char const *str, int maxChar);
public:
    static LPVOID VirtualAlloc(LPVOID lpAddress, tuint32 dwSize, tuint32 flAllocationType, tuint32 flProtect);
    static bool VirtualFree(LPVOID lpAddress, tuint32 dwSize, tuint32 dwFreeType);

    /////////////////////  获取网卡信息相关
public:
    enum NIC_STATE {
		NIC_STATE_RANGE_BEGIN,
		NIC_STATE_INITIAL_VALUE,
        NIC_STATE_Offline = 0, //网络电缆已拔出
        NIC_STATE_Online = 1,  //已链接
        NIC_STATE_Disabled = 2, //禁用
		NIC_STATE_RANGE_END,
    };
#pragma pack(push,1)
    //尝试获取一个网卡的信息
    typedef struct __TNICInfo {
        NIC_STATE State;		//状态，见 NIC_STATE
        char eth_name[48];  //网卡标识名称 (Linux为eth0, eth1，Win32为GUID)
        tuint32 IP;
        tuint32 Netmask;
        tuint32 Gateway;
        BYTE MAC[6];
		bool bIsMaster; //是bonding 的 master
		bool bIsSlave;	//是bonding 的 slave（不能用于socket绑定，没有路由）

		bool operator == (const __TNICInfo &d) const
		{
			return (State == d.State)&&(strcmp(eth_name,d.eth_name)==0)&&(Netmask==d.Netmask)&&(IP==d.IP)&&(Gateway==d.Gateway)\
				&&(strcmp((const char *)MAC,(const char *)d.MAC)==0)&&(bIsMaster == d.bIsMaster)&&(bIsSlave==d.bIsSlave);
		}
    } TNICInfo, *TPNICInfo;
#pragma pack(pop)

    typedef struct ETHInfo {
        char eth_name[48];  //网卡标识名称 (Linux为eth0, eth1，Win32为GUID)
        tuint32 nAllBandWidth;//kb/s 总带宽
        tuint32 nResBandWidth;//kb/s 当前剩余带宽
    } ;

public:
    //获取网卡详细信息列表(常用于配置网卡时)
    static int GetNICList(std::list<TNICInfo> &NICList);
    //获取可用于socket的网络设备信息列表(绑定了组IP的物理网卡，不可再用于socket)
    static int GetSocketEnabled_NICList(std::list<TNICInfo> &NICList);

    /////////////////////// 调用外部命令相关
public:
    //定义父进程和子进程消息队列的格式
    #pragma pack(push,1)
    //要调用的命令
    typedef struct __SystemCall_Request
    {
       bool bWaitReturn;
       char szCmd[256]; //要执行的命令
    } SystemCall_Request, *PSystemCall_Request;

    //命令返回结果
    typedef struct __SystemCall_Reply
    {
        int nCmdResult; //命令执行的返回结果 WEXITSTATUS(system函数返回值)
        int nSystemErrno;  //返回0 表示system 调用成功，否则是 errno
    } SystemCall_Reply, *PSystemCall_Reply;

    #pragma pack(pop)

    //主进程通知子进程调用一个外部程序
    //【注意】系统的/bin/sh 需要链接至bash，如果是dash，则重定向或不等待时，调用失败“sh: 1: Syntax error: Bad fd number”
    static bool SystemCall(const char *szCmd, bool bWaitReturn, int &nCmdResult);
	
	static long SystemOpenProcess(const char *szCmd, int &nCmdResult);
	static bool SystemCloseProcess(const char *strProcessName, long nProcess);//二选一

    static double GetRevAndSendValue(const char *ethName,double &rev,double &send);
    static void GetEthSpeedInfo(std::list<ETHInfo> &ethList);
    static int GetMemoryUsageRate(void);
    static unsigned long GetUsedVirMemory(void);
    static int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n);
    static void get_cpuoccupy (CPU_OCCUPY *cpust);
    static int GetCPUUsageRate(void);
private:
    /////构造
    CPFHelper(void);
    ////析构
    ~CPFHelper(void);
    static tint32 m_dwPageSize;

};

#endif
