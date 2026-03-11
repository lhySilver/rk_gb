
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
//#include <uuid/uuid.h>
#include "uuid.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
//#include <net/if.h>	//同时引用，和 linux/route.h 冲突
#include <net/if_arp.h>
#include <linux/route.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/statfs.h>

#include "base_type_define.h"
#include "PFHelper.h"
#include "Lock.h"
#include <sys/sysinfo.h>
//#include "ShareSDK.h"

const char CPFHelper::DirectorySeparator = '/';

tint32 CPFHelper::m_dwPageSize = 0;

CPFHelper::CPFHelper(void)
{
}

CPFHelper::~CPFHelper(void)
{
}

bool CPFHelper::GetModuleFreeSpace(char* modulePath,  tuint64& freeSizeForCaller, tuint64& freeSizeForAll, tuint64& totalSize)
{
    struct statfs diskStatfs;
    int ret = statfs(modulePath, &diskStatfs);
    if( ret != 0)
        return false;

    unsigned long long blocksize = diskStatfs.f_bsize;          // size for per block
    unsigned long long _totalSize = blocksize * diskStatfs.f_blocks; // total size for block

    unsigned long long freeDisk = diskStatfs.f_bfree * blocksize;
    unsigned long long avaliableDisk = diskStatfs.f_bavail * blocksize;

    totalSize	= _totalSize;
    freeSizeForCaller	= avaliableDisk;
    freeSizeForAll = freeDisk;
    return true;
}

///////////获取CPU的个数
tint32 CPFHelper::GetCPUCount()
{
    int cpu_num = sysconf(_SC_NPROCESSORS_ONLN);
    return cpu_num;
}

tint32 CPFHelper::GetPageSize()
{
    if(m_dwPageSize == 0)
        m_dwPageSize = sysconf(_SC_PAGESIZE);
    return m_dwPageSize;
}


HANDLE CPFHelper::CreateThread(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, tuint_ptr* lpThreadId)
{
    pthread_t threadhandle;
    int iRet = 0;
    if((iRet = pthread_create(&threadhandle, 0, lpStartAddress, lpParameter)) != 0)
    {
        errno = iRet;	//线程创建不会设置错误码，而是直接返回错误码
        return 0;
    }
    else
    {
        *lpThreadId=threadhandle;
        return threadhandle;
    }
}

void CPFHelper::DestroyThread(HANDLE &hThread)
{
    pthread_join(hThread, NULL);
}

#include <sys/syscall.h>
#include <sys/prctl.h>
bool CPFHelper::SetCurrentThreadName(const char *pName,const char *pFileName,int fileline)
{
    printf("THREADDDD=%s,%d,%s,%d\n",pName,syscall(__NR_gettid),pFileName,fileline);
    prctl(PR_SET_NAME, pName);
    return true;
}

bool CPFHelper::AttachThreadToCPU(HANDLE hThread,int cpuindex)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpuindex,&mask);
    return (pthread_setaffinity_np(hThread,sizeof(mask),&mask) >=0 );
}

void CPFHelper::DetachThread(HANDLE &hThread)//销毁线程句柄但不等待线程退出
{	
	pthread_detach(hThread);
}

bool CPFHelper::CreateGUID(GUID &guid)
{
    uuid_generate(reinterpret_cast<unsigned char *>(&guid));
    return true;
}

//根据当前时间创建GUID,主要用于解决排序问题
bool CPFHelper::CreateGUIDByCurTime(GUID &guid)
{
	uuid_generate(reinterpret_cast<unsigned char *>(&guid));

	timeval tp;
	gettimeofday(&tp, NULL);
	guid.Data1=tp.tv_sec;//总秒数
	guid.Data2=tp.tv_usec/1000;//毫秒
	CPFHelper::Sleep(1);//为了绝对确保GUID不一样，休眠1毫秒

	return true;
}

//根据父节点的GUID创建子节点,主要用于解决排序问题
//设备下面创建通道，输入，输出  报警主机下面创建子系统、防区 门禁考勤系统下面创建设备门读头 用到
bool CPFHelper::CreateGUIDByParentGUID(GUID parentGUID,unsigned short nIndex,GUID &guid)
{
	if (parentGUID==GUID_NULL)
	{
		return false;
	}

    uuid_generate(reinterpret_cast<unsigned char *>(&guid));

    guid.Data1 = parentGUID.Data1;
    guid.Data2 = parentGUID.Data2;
    guid.Data3 = nIndex;
    CPFHelper::Sleep(1);//为了绝对确保GUID不一样，休眠1毫秒
    return true;

}

void CPFHelper::Sleep(tuint32 ms)//ms单位为毫秒
{
    //以下代码只支持到glibc
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;

    // many calls will be interupted. so we keep looping till we're done.
    while ( nanosleep(&req, &req) == -1 && errno == EINTR && (req.tv_nsec > 0 || req.tv_sec > 0))
        ;

    //以下代码支持非glibc
    /*
    //大于一秒的时间用这个睡眠
    if (iSec > 0)
    {
        do
        {
            iSec = sleep(iSec);
        } while(iSec > 0);
    }

    if(0 != usleep(iMicSec))
    {
        if (EINTR == errno)
        {
            printf("the usleep Interrupted by a signal. pid = %d\n", getpid());
        }
        else if (EINVAL == errno)
        {
            assert(false);
            printf("the usleep param is not smaller than 1000000");
        }
    }
    */
}

void CPFHelper::SetLastError(tuint32 errorcode)
{
    errno=errorcode;
}

tuint32 CPFHelper::GetLastError()
{
    return errno;
}

tuint32 CPFHelper::GetTickCount()
{
    unsigned long dwTick = 0;
    struct timespec tp;

	do 
	{
		clock_gettime(CLOCK_MONOTONIC, &tp);
		dwTick = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
	} while (0 == dwTick);

    return dwTick;
}

tuint_ptr CPFHelper::GetThreadID()
{
    //return syscall(SYS_gettid);
    return pthread_self();
}

tuint32 CPFHelper::GetThreadID(HANDLE hThread)
{
    return hThread;
}

tuint32 CPFHelper::GetModuleFileDir(HANDLE hModule, char *lpFilename, tuint32 dwSize)
{
    char tmpBuf[512]={0};
    char szExe[512]={0};
    int ret;
    //得到程序的运行路径
    snprintf(tmpBuf, 511, "/proc/%d/exe", getpid());
    ret = readlink(tmpBuf, szExe, dwSize);
    if (ret>=0 && ret<dwSize)
    {
        szExe[ret] = 0 ;
    }
	char *pPos = strrchr(szExe,'/');
	szExe[pPos-szExe] = '\0';
	strcat(szExe,"/");
	memset(lpFilename,0,dwSize);
    memcpy(lpFilename,szExe,std::min(strlen(szExe),(size_t)dwSize));
	dwSize = strlen(lpFilename);
	return dwSize;
}

tuint32 CPFHelper::GetModuleFilePath(HANDLE hModule, char *lpFilename, tuint32 dwSize)
{
 int nRet = GetModuleFileName(hModule, lpFilename, dwSize);
 if(nRet > 0)
 {
  for (int i=strlen(lpFilename)-1; i>=0; --i)
  {
   if (lpFilename[i] == CPFHelper::DirectorySeparator)
   {
    break;
   }
   else
   {
    lpFilename[i] = 0;
   }
  }
 }
 return nRet;
}

tuint32 CPFHelper::GetModuleFileName(HANDLE hModule, char *lpFilename, tuint32 dwSize)
{
    char tmpBuf[512];
    int ret;
    //得到程序的运行路径
    snprintf(tmpBuf, 511, "/proc/%d/exe", getpid());
    ret = readlink(tmpBuf, lpFilename, dwSize);
    if (ret>=0 && ret<dwSize)
    {
        lpFilename[ret] = 0 ;
    }
    return ret;
}

tuint32 CPFHelper::GetConfigFileDir(char *lpFilename, tuint32 dwSize)
{
	char tmpBuf[512]={0};
	char szExe[512]={0};
	int ret;
	//得到程序的运行路径
	snprintf(tmpBuf, 511, "/proc/%d/exe", getpid());
	ret = readlink(tmpBuf, szExe, dwSize);
	if (ret>=0 && ret<dwSize)
	{
		szExe[ret] = 0 ;
	}
	char *pPos = strrchr(szExe,'/');
	szExe[pPos-szExe] = '\0';
	strcat(szExe,"/");
	memset(lpFilename,0,dwSize);
	memcpy(lpFilename,szExe,std::min(strlen(szExe),(size_t)dwSize));
	dwSize = strlen(lpFilename);
	return dwSize;
}

bool CPFHelper::GetFileList(const char *szPath, std::list<std::string> &FileList)
{
    DIR *dir;
    struct dirent *s_dir;

    FileList.clear();

    dir = opendir(szPath);
    if (dir)
    {
        while (NULL != (s_dir=readdir(dir)))
        {
            if ((strcmp(s_dir->d_name,".")==0)||(strcmp(s_dir->d_name,"..")==0)) continue;
            FileList.push_back(s_dir->d_name);
        }
        closedir(dir);
        return true;
    }

    return false;
}

void CPFHelper::Sync(void)
{
    sync();
}

void CPFHelper::ReleaseOSCachedMem(void)
{
    sync();
    int fd = open("/proc/sys/vm/drop_caches", O_WRONLY|O_CREAT|O_TRUNC, 0666);
    ssize_t n;
    do {
        n = write(fd, "3", 1);
    } while (n < 0 && errno == EINTR);
    close(fd);
}

bool CPFHelper::IPv6AddressToStr(const struct in6_addr &addr, char *pStrIP, int strLen)
{
    char str[INET6_ADDRSTRLEN] = {0};
    const char *p = inet_ntop(AF_INET6, &addr, str, INET6_ADDRSTRLEN);
    if (p)
    {
        assert(strLen >= (strlen(str) + 1));
        if ((int)strlen(str) < strLen -1)
        {
            strcpy(pStrIP, str);
            return true;
        }
    }
    return false;
}	

bool CPFHelper::Rename(const char *szOld, const char *szNew)
{
    int ret = rename(szOld,szNew);
    return ret == 0;
}


bool CPFHelper::StrToIPv6Address(const char *strIP, struct in6_addr &addr)
{
	int ret = inet_pton(AF_INET6, strIP, &addr);
	return (ret == 1);	
}

bool CPFHelper::GetFileListRecursive(std::string rootPath, std::list<std::string>& fileList)
{
    DIR*    pDir;
    struct dirent* ent;
    char path[512] = {0};
    pDir = opendir(rootPath.c_str());
    if(pDir == NULL)
        return true;
    while( (ent=readdir(pDir)) != NULL){
        memset(path, 0, sizeof(path));
        sprintf(path, "%s/%s", rootPath.c_str(), ent->d_name);
        if(ent->d_type & DT_DIR){
            if(strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name, "..")==0)
                continue;
            GetFileListRecursive(path, fileList);
        }
        else{
             fileList.push_back(path);
        }
    }
    closedir(pDir);
	return true;
}

/////////////////////////////////内存分配相关
LPVOID CPFHelper::VirtualAlloc(LPVOID lpAddress, tuint32 dwSize, tuint32 flAllocationType, tuint32 flProtect)
{
    //不支持更改已经分配的内存大小
    ASSERT(lpAddress==NULL);
    ASSERT(flAllocationType==MEM_COMMIT);

    static size_t pgsize = sysconf(_SC_PAGESIZE);
    void *pMemPrt;
    int ret = posix_memalign(&pMemPrt, pgsize, dwSize);
    if (ret == 0)
    {
        return pMemPrt;
    }
    else
    {
        return NULL;
    }
}

bool CPFHelper::VirtualFree(LPVOID lpAddress, tuint32 dwSize, tuint32 dwFreeType)
{
    //只支持释放全部内存
    ASSERT(dwSize==0);
    ASSERT(dwFreeType==MEM_RELEASE);

    free(lpAddress);
    return true;
}

///////////////////////////////// 获取网卡信息相关
//======================= 单元内部的函数  ==========================
#define PATH_PROC_NET_ROUTE "/proc/net/route"

//BUG 这里有一个BUG，找网卡的时候用的是strstr eth0 eth0:1 怎么区分
bool  Get_NIC_Gateway(tuint32 *pGateway, tuint32  *pTarget,  tuint32 *pTargetMask, const char* pInterfaceName)
{
    char buffer[200] = {0};
    tuint32 defaultRoutePara[4]={0};

    FILE* p_fd = fopen(PATH_PROC_NET_ROUTE, "r");
    if (NULL==p_fd)
    {
        return false;
    }

    while(fgets(buffer,200,p_fd))
    {
        sscanf(buffer, "%*s %x %x %x %*x %*x %*x %x %*x %*x %*x\n",
               (unsigned int *)&defaultRoutePara[1], (unsigned int *)&defaultRoutePara[0],
               (unsigned int *)&defaultRoutePara[3],(unsigned int *)&defaultRoutePara[2]);
        if (NULL!=strstr(buffer,pInterfaceName))
        {
            //如果FLAG标志中有 RTF_GATEWAY
            if (defaultRoutePara[3] & RTF_GATEWAY)
            {
                assert(NULL!=pGateway);
                *pGateway = defaultRoutePara[0];
                if (NULL != pTarget)
                {
                    *pTarget = defaultRoutePara[1];
                }
                if (NULL != pTargetMask)
                {
                    *pTargetMask = defaultRoutePara[2];
                }
                fclose(p_fd);
                return true;
            }
        }
        memset(buffer,0,200);
    }

    fclose(p_fd);
    return false;
}

//从 70-persistent-net.rules 中获得网卡MAC，传入的 MACBuf 需至少6字节大小
bool get_MAC_from_rules(char *ethname, BYTE *MACBuf)
{
#define FILE_persistent_net_rules "/etc/udev/rules.d/70-persistent-net.rules"
    FILE *fh;
    char line[1024], txtbuf[32], sResultBuf[64];
    char *p;
    bool bRet = false;
    int i;

    fh = fopen(FILE_persistent_net_rules, "r");
    if (!fh)
    {
        //TRACE("open %s fail, err=%d", FILE_persistent_net_rules, errno);
    }
    else
    {
        while (fgets(line, sizeof(line), fh) != NULL)
        {
            if (line[0]=='#') continue; //跳过#开头的注释行
            sprintf(txtbuf, "NAME=\"%s\"", ethname);
            p = strstr(line, txtbuf);
            if (p!=NULL)
            {
                strcpy(txtbuf, "ATTR{address}==\"");
                p = strstr(line, txtbuf);
                if (p!=NULL)
                {
                    memset(sResultBuf, 0, sizeof(sResultBuf));
                    strncpy(sResultBuf, p+strlen(txtbuf), 17); //10:bf:48:d7:0c:76
                    for (i=0, p=sResultBuf; i<6; i++, p=p+3)
                    {
                        strncpy(txtbuf, p, 2);
                        txtbuf[2] = 0;
                        MACBuf[i] = (BYTE)strtol(txtbuf, NULL, 16);
                    }
                    bRet = true;
                }
                break;
            }
        }
        fclose(fh);
    }
    return bRet;
}

//是 _enum_NIC_by_proc 的子函数，获得网卡的详细信息
void _getNIC_Info(int fd, struct ifreq *pIfr, CPFHelper::TNICInfo &info)
{
    bool bRet;
    memset(&info, 0, sizeof(CPFHelper::TNICInfo));
    strcpy(info.eth_name, pIfr->ifr_name);

    if (ioctl (fd, SIOCGIFFLAGS, pIfr) != -1)
    {
        //改为实际的状态
        //info.IsActived = ((pIfr->ifr_flags & IFF_RUNNING)==IFF_RUNNING);  //IFF_UP => IFF_RUNNING
        if ((pIfr->ifr_flags & IFF_RUNNING)==IFF_RUNNING)
            info.State = CPFHelper::NIC_STATE_Online;
        else info.State = CPFHelper::NIC_STATE_Offline;
        //TRACE("%s =>iff flags=%d", pIfr->ifr_name, pIfr->ifr_flags);
        info.bIsMaster = ((pIfr->ifr_flags & IFF_MASTER) == IFF_MASTER);
        info.bIsSlave = ((pIfr->ifr_flags & IFF_SLAVE) == IFF_SLAVE);
    }
    else
    {
        info.State = CPFHelper::NIC_STATE_Disabled;
        //TRACE("SIOCGIFFLAGS fail, errno=%d", errno);
    }

    if (ioctl(fd, SIOCGIFADDR, pIfr) != -1)
    {
        info.IP = ((struct sockaddr_in*) (&pIfr->ifr_addr))->sin_addr.s_addr;
    }

    if (ioctl(fd, SIOCGIFNETMASK, pIfr) != -1)
    {
        info.Netmask = ((struct sockaddr_in*) (&pIfr->ifr_addr))->sin_addr.s_addr;
    }
    /* //bern: 这个取出来不是网关，是广播地址
    if (ioctl(fd, SIOCGIFBRDADDR, pIfr) != -1) {
        info.Gateway = ((struct sockaddr_in*) (&pIfr->ifr_addr))->sin_addr.s_addr;
        info.bOK_GetGateway = true;
    } else info.Gateway = 0;
    */

    bRet = Get_NIC_Gateway(&info.Gateway, NULL, NULL, info.eth_name);

    //如果是用了bonding的话，这样取出的MAC不正确，所以先尝试从 cat /etc/udev/rules.d/70-persistent-net.rules 中取得
    bRet = false;
    if (!info.bIsMaster)
    {
        //非 bonding 就先从udev规则中获取
        bRet = get_MAC_from_rules(info.eth_name, info.MAC);
    }
    if (!bRet)
    { //如果从 70-persistent-net.rules 获取失败，就只能用原来的方法了
        if (ioctl(fd, SIOCGIFHWADDR, pIfr) != -1)
        {
            memcpy(info.MAC, pIfr->ifr_hwaddr.sa_data, 6);
        }
    }
    //TRACE("get %s IP=%08lX, Netmask=%08lX, Gateway=%08lX", info.eth_name, info.IP, info.Netmask, info.Gateway);
}

char *_get_name(char *name, char *p)
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

//是 CPFNetwork::GetNICList 的子函数，获得网卡的信息，根据 /proc/net/dev来做遍历
int _enum_NIC_by_proc(int fd, std::list<CPFHelper::TNICInfo> &Infos)
{
#define PATH_PROC_NET_DEV "/proc/net/dev"
    FILE *fh;
    char buf[512];
    int nCount = 0;
    bool bFound;
    CPFHelper::TNICInfo tmpInfo;
    struct ifreq tmpIfr;
    char *p, *s;

    fh = fopen(PATH_PROC_NET_DEV, "r");
    if (!fh) {
        //TRACE("open %s fail, err=%d", PATH_PROC_NET_DEV, errno);
    }
    else
    {
        //警告： 忽略声明有 warn_unused_result 属性的‘char* fgets(char*, int, FILE*)’的返回值 [-Wunused-result]
        p = fgets(buf, sizeof(buf), fh);	/* eat line */
        p = fgets(buf, sizeof(buf), fh);
        while (fgets(buf, sizeof(buf), fh))
        {
            char name[16];
            s = _get_name(name, buf);
            //以 eth或bond 开头
            if ((strncasecmp("eth", name, 3)==0) || (strncasecmp("bond", name, 4)==0))
            {
                bFound = false;
                std::list<CPFHelper::TNICInfo>::iterator iter = Infos.begin();
                for (; iter!=Infos.end(); ++iter)
                {
                    if (strcmp(iter->eth_name, name)==0)
                    {
                        bFound = true;
                        break;
                    }
                }
                if (!bFound)
                {
                    memset(&tmpIfr, 0, sizeof(struct ifreq));
                    strcpy(tmpIfr.ifr_name, name);
                    _getNIC_Info(fd, &tmpIfr, tmpInfo);
                    Infos.push_back(tmpInfo);
                    nCount++;
                }
            }
        }
        fclose(fh);
    }
    UNREFERENCED_PARAMETER(p);
    UNREFERENCED_PARAMETER(s);
    return nCount;
}

int CPFHelper::GetNICList(std::list<TNICInfo> &NICList)
{
    int fd;
    int nCount = 0;

    NICList.clear();
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        //TRACE("socket fail, err=%d", errno);
    }
    else
    {
        nCount = _enum_NIC_by_proc(fd, NICList);
        close(fd);
    }

    return nCount;
}

int CPFHelper::GetSocketEnabled_NICList(std::list<TNICInfo> &NICList)
{
    int nCount = 0;
    std::list<TNICInfo> TmpNICList;

    NICList.clear();
    GetNICList(TmpNICList); //临时记录列表

    std::list<TNICInfo>::iterator iter = TmpNICList.begin();
    for (; iter!=TmpNICList.end(); ++iter)
    {
        //只报告在线的、以及可用（不是Slave的）网卡
        if (iter->State==NIC_STATE_Online && !iter->bIsSlave)
        {
            NICList.push_back(*iter);
            nCount++;
        }
    }
    return nCount;
}

int CPFHelper::GetUtf8Length( char const *str, int *cntArray)
{
	int cntTemp[5] = {0};
	int *cnt = cntTemp;
	if (cntArray != 0)
	{
		cnt = cntArray;
	}
	while( *str != '\0' )
	{  
		if( *str & 1<<7 )
		{  
			if( *str & (1<<6) )
			{  
				if( *str & (1<<5) )
				{  
					if( *str & (1<<4))
					{  
						cnt[4]++,cnt[0]++,str += 4;  
						continue;  
					}  
					cnt[3]++,cnt[0]++,str += 3;  
					continue;  
				}  
				cnt[2]++,cnt[0]++,str += 2;  
				continue;  
			}  
		}  
		cnt[1]++,cnt[0]++,str += 1;  
		continue;  
	}
	return cnt[0];
} 

int CPFHelper::StringCutBytesUtf8( char const *str, int max )
{
	int strLen = strlen(str);
	if (strLen <= max)
	{
		return strLen;
	}
	int chCount = 0,count = 0;
	while( *str != '\0' && chCount < max )
	{  
		if( *str & 1<<7 )
		{  
			if( *str & (1<<6) )
			{  
				if( *str & (1<<5) )
				{  
					if( *str & (1<<4))
					{  
						++chCount,count+=4,str += 4;  
						continue;  
					}  
					++chCount,count+=3,str += 3;  
					continue;  
				}  
				++chCount,count+=2,str += 2;
				continue;  
			}  
		}  
		++chCount,count+=1,str += 1;  
		continue;  
	}
	if (count > strLen)
	{
		count = strLen;
	}
	return count;
}

//////////////////////////////////// 调用外部程序相关

bool CPFHelper::SystemCall(const char *szCmd, bool bWaitReturn, int &nCmdResult)
{
    //需要在父进程fork子进程之前，用 mkfifo 创建了两个管道 SystemCall_Pipe_Server 和 SystemCall_Pipe_Client

    //上锁,防止多线程同时调用，等不到正确的返回结果 //对应子进程初始信号1
    sem_t *sem_Lock = sem_open(SystemCall_LockFile, 0);
    if(NULL == sem_Lock)
    {
        return false;
    }
    sem_wait(sem_Lock);

    SystemCall_Request request;
    memset(&request, 0, sizeof(SystemCall_Request));
    ASSERT(strlen(szCmd) < sizeof(request.szCmd));
    strcpy(request.szCmd, szCmd);
    request.bWaitReturn = bWaitReturn;
    int fd_Server = -1, fd_Client = 1;
    bool bRet = false;

    do {
        fd_Server = open(SystemCall_Pipe_Server, O_WRONLY);
        if (fd_Server == -1)
        {
            //DEBUG_OUTPUT("open %s, errno=%d", SystemCall_Pipe_Server, errno);
            break;
        }

        int ret = write(fd_Server, &request, sizeof(SystemCall_Request));
        if (ret == sizeof(SystemCall_Request))
        {
            //将命令写入管道成功，那么一直等待返回的结果
            fd_Client = open(SystemCall_Pipe_Client, O_RDONLY);
            if (fd_Client == -1)
            {
                //DEBUG_OUTPUT("open %s, errno=%d", SystemCall_Pipe_Client, errno);
                break;
            }

            SystemCall_Reply reply;
            ret = read(fd_Client, &reply, sizeof(SystemCall_Reply));
            if (ret==sizeof(SystemCall_Reply))
            {
                nCmdResult = reply.nCmdResult;
                //DEBUG_OUTPUT("get cmd result:%s , nCmdResult=%d, nSystemErrno=%d", szCmd, nCmdResult, reply.nSystemErrno);
                bRet = (reply.nSystemErrno == 0);
            }
            else
            {
                //DEBUG_OUTPUT("read cmd result:%s fail, errno=%d, ret=%d", szCmd, errno, ret);
            }
        }
        else
        {
            //DEBUG_OUTPUT("write cmd:%s fail, errno=%d, ret=%d", szCmd, errno, ret);
        }
    } while (false);

    if (fd_Server != -1) close(fd_Server);
    if (fd_Client != -1) close(fd_Client);

    sem_post(sem_Lock);
    sem_close(sem_Lock);

    return bRet;
}

tuint32 CPFHelper::GetAppCurrentMemoryRSS(tuint32 &dwPID)
{
	pid_t pid = getpid();
	dwPID = pid;

	char szBuf[32] = {0};
	sprintf(szBuf, "/proc/%u/status", pid);

	FILE *fp = fopen(szBuf, "r");
	if (NULL == fp)
	{
		printf("CPFHelper::GetAppCurrentMemoryRSS open %s failed.\n", szBuf);
		return 0;
	}

	tuint32 dwMemmryRSS = 0;
	char szLine[256] = {0};
	while (szLine == fgets(szLine, sizeof(szLine) - 1, fp))
	{
		if (NULL != strstr(szLine, "VmRSS:"))
		{
			sscanf(szLine,"VmRSS:\t%8u kB\n",&dwMemmryRSS);
			break;
		}

		memset(szLine, 0, sizeof(szLine));
	}

	fclose(fp);

	return dwMemmryRSS;
}

long CPFHelper::SystemOpenProcess(const char *szCmd, int &nCmdResult)
{
	return 0;
}

bool CPFHelper::SystemCloseProcess(const char *strProcessName, long nProcess)
{
	return false;
}

double CPFHelper::GetRevAndSendValue(const char *ethName,double &rev,double &send)
{
    int nDevLen = strlen(ethName);
    if (nDevLen < 1 || nDevLen > 100)
    {
        //printf("dev length too long\n");
        return 0;
    }
    int fd = open("/proc/net/dev", O_RDONLY | O_EXCL);
    if (-1 == fd)
    {
        //printf("/proc/net/dev not exists!\n");
        return 0;
    }

    char buf[1024*2];
    lseek(fd, 0, SEEK_SET);
    int nBytes = read(fd, buf, sizeof(buf)-1);
    if (-1 == nBytes)
    {
        //perror("read error");
        close(fd);
        return 0;
    }
    buf[nBytes] = '\0';
    //返回第一次指向ath0位置的指针
    char* pDev = strstr(buf, ethName);
    if (NULL == pDev)
    {
        //printf("don't find dev %s\n", ath0);
        return 0;
    }
    char *p;
    char *ifconfig_value;
    int i = 0;
    //static long rx2_tx10[2];
    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
    {
        i++;
        ifconfig_value = (char*)malloc(20);
        strcpy(ifconfig_value, p);
        /*得到的字符串中的第二个字段是接收流量*/
        if(i == 2)
        {
            rev = strtod(ifconfig_value,NULL);
        }
        /*得到的字符串中的第十个字段是发送流量*/
        if(i == 10)
        {
            send = strtod(ifconfig_value,NULL);
            break;
        }
        free(ifconfig_value);
    }
    close(fd);
    return 0;//rx2_tx10;
}

//获取网卡带宽等信息
void CPFHelper::GetEthSpeedInfo(std::list<ETHInfo> &ethList)
{
    std::list<ETHInfo> EthInfoList;

    std::list<TNICInfo> TmpNICList;
    CPFHelper::GetNICList(TmpNICList);

    std::list<TNICInfo>::iterator iter = TmpNICList.begin();
    for (; iter!=TmpNICList.end() ; ++iter)
    {
        struct ifreq ifr, *ifrp;  // 接口请求结构
        int fd;   // to  access socket  通过socket访问网卡的 文件描述符号fd
        /* Setup our control structures. */
        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, iter->eth_name);
        /* Open control socket. */
        fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (fd < 0)
        {
            printf("Cannot get control socket");
            continue;
        }

        int err;
        struct _ethtool_cmd_ ep;
        //fprintf(stdout, "Settings for %s:\n", devname);
        ep.cmd = ETHTOOL_GSET; // ethtool-copy.h:380:#define ETHTOOL_GSET         0x00000001 /* Get settings. */
        ifr.ifr_data = (caddr_t)&ep;  //   caddr_t 是void类型，而这句话是什么意思
        err = ioctl(fd, SIOCETHTOOL, &ifr);  //  int ioctl(int handle, int cmd,[int *argdx, int argcx]);
        if (err != 0)
        { // 如果出错退出;
            printf(" ioctl is erro .\n");
            continue;
        }
        //Mb/s
        if( (ep.speed == 10)||(ep.speed == 100)||(ep.speed == 1000)||(ep.speed == 2500)||(ep.speed == 10000) )
        {
            ETHInfo ethInfo;
            memset(&ethInfo,0,sizeof(ETHInfo));

            strcpy(ethInfo.eth_name,iter->eth_name);
            ethInfo.nAllBandWidth = ep.speed*1024;

            double rev1=0,send1=0;
            GetRevAndSendValue(ethInfo.eth_name,rev1,send1);
            tuint32 nSleepTime = 3;//3秒为间隔
            CPFHelper::Sleep(1000*nSleepTime);//
            double rev2=0,send2=0;
            GetRevAndSendValue(ethInfo.eth_name,rev2,send2);
            long value = (rev2+send2-rev1-send1);//单位为B
            if(value>0)
            {
                long userBandWidth = value*8/(1024*nSleepTime);//kb/s
                ethInfo.nResBandWidth = ethInfo.nAllBandWidth-userBandWidth;
            }
            EthInfoList.push_back(ethInfo);
        }
        close(fd);
    }
    ethList = EthInfoList;
}

int CPFHelper::GetMemoryUsageRate(void)
{
    int iMemUsage = 0;
    struct sysinfo tmp;
    int ret = 0;
    ret = sysinfo(&tmp);
    if (0 == ret)
    {
        iMemUsage = (1.0 - (float)tmp.freeram/(float)tmp.totalram) * 100;
    }

    return iMemUsage;
}

unsigned long CPFHelper::GetUsedVirMemory(void)
{
    struct sysinfo tmp;
    int ret = 0;
    ret = sysinfo(&tmp);

    if (0 == ret)
    {
        return tmp.totalswap - tmp.freeswap;
    }

    return -1;
}

#define CK_TIME 1000

int CPFHelper::cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{
    unsigned long od, nd;
    unsigned long id, sd;
    int cpu_use = 0;

    od = (unsigned long ) (o->user + o->nice + o->system +o->idle/*+o->softirq+o->iowait+o->irq*/);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long ) (n->user + n->nice + n->system +n->idle/*+n->softirq+n->iowait+n->irq*/);//第二次(用户+优先级+系统+空闲)的时间再赋给od

    id = (unsigned long ) (n->user - o->user);    //用户第一次和第二次的时间之差再赋给id
    sd = (unsigned long ) (n->system - o->system) ;    //系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
    cpu_use = (int)((sd + id)*100/(nd - od)); //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else cpu_use = 0;
    return cpu_use;
}
void CPFHelper::get_cpuoccupy (CPU_OCCUPY *cpust)
{
    FILE *fd;
    int n;
    char buff[256];
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;

    fd = fopen ("/proc/stat", "r");
	if(fd==NULL)
		return;
    fgets (buff, sizeof(buff), fd);

    sscanf (buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle ,&cpu_occupy->iowait,&cpu_occupy->irq,&cpu_occupy->softirq);

    fclose(fd);
}

int CPFHelper::GetCPUUsageRate(void)
{
//    FILE *fp;
//    char buf[128];
//    char cpu[5];
//    long int user,nice,sys,idle,iowait,irq,softirq;

//    long int all1,all2,idle1,idle2;
//    float usage;

//    fp = fopen("/proc/stat","r");
//    if(fp == NULL)
//    {
//        perror("fopen:");
//        return 0;
//    }

//    fgets(buf,sizeof(buf),fp);
//#ifndef NDEBUG
//    printf("buf=%s",buf);
//#endif
//    sscanf(buf,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

//#ifndef NDEBUG
//    printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
//#endif

//    all1 = user+nice+sys+idle+iowait+irq+softirq;
//    idle1 = idle;
//    rewind(fp);
//    /*第二次取数据*/
//    usleep(CK_TIME);
//    memset(buf,0,sizeof(buf));
//    cpu[0] = '\0';
//    user=nice=sys=idle=iowait=irq=softirq=0;
//    fgets(buf,sizeof(buf),fp);
//#ifndef NDEBUG
//    printf("buf=%s",buf);
//#endif
//    sscanf(buf,"%s%d%d%d%d%d%d%d",cpu,&user,&nice,&sys,&idle,&iowait,&irq,&softirq);

//#ifndef NDEBUG
//    printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
//#endif

//    all2 = user+nice+sys+idle+iowait+irq+softirq;
//    idle2 = idle;

////    usage = (float)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ;

//    usage=all2-all1;
//    usage=(usage-(idle2-idle1))/usage;

//    printf("all=%d\n",all2-all1);
//    printf("ilde=%d\n",all2-all1-(idle2-idle1));
//    printf("cpu use = %.2f\%\n",usage);
//    printf("=======================\n");

//    fclose(fp);

//    return usage;
    CPU_OCCUPY cpu_stat1;
    CPU_OCCUPY cpu_stat2;
    double cpu;
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
    //sleep(1);
    usleep(200000);
    //第二次获取cpu使用情况
    get_cpuoccupy((CPU_OCCUPY *)&cpu_stat2);

    //计算cpu使用率
    cpu = cal_cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
  
    return cpu;
}


