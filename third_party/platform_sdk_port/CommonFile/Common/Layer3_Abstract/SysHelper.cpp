#include "SysHelper.h"
#include "PFHelper.h"
#include <assert.h>
using namespace std;
#define new DEBUG_NEW
#ifdef WIN32
#if _MSC_VER >=1400    // VC2005才支持intrin.h
#include <intrin.h>    // 所有Intrinsics函数
#include <comutil.h>
#include <Wbemidl.h>
#include <tchar.h>
#include <strsafe.h>
#include <algorithm>
#include <atlconv.h>
#include <ntddndis.h>
#include <winioctl.h>
#pragma comment (lib, "comsuppw.lib")
#pragma comment (lib, "comsupp.lib")
#pragma comment (lib, "wbemuuid.lib")
#endif


#else

#ifndef __ENVIRONMENT_LINUX_ANDROID__
#include <iconv.h>
#endif

#include "errno.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#include <syslog.h>
#include <sys/stat.h>
#include <sys/time.h>

#if (!defined __ENVIRONMENT_LINUX_ANDROID__) && (!defined __ENVIRONMENT_MAC__)
#include <scsi/sg.h>
#endif

#include <sys/ioctl.h>
#ifndef __ENVIRONMENT_MAC__
#include <linux/hdreg.h>
#endif
#include <string>
#include <fstream>
#endif
#include "WarningDisable.h"
#define DESENSITIZATION 1

tuint32 CSysHelper::m_nUbuntuOS = 0;
std::string CSysHelper::m_strLinuxDevProduct="";//出厂产品型号
std::string CSysHelper::m_strLinuxDevCustomerModel="";//出厂产品型号

CSysHelper::CSysHelper(void)
{

}

CSysHelper::~CSysHelper(void)
{

}

int CSysHelper::GetCPUCount()
{
	return CPFHelper::GetCPUCount();
}

bool CSysHelper::CreateGUID(GUID & guid)
{
	return CPFHelper::CreateGUID(guid);
}

GUID CSysHelper::GetGuidFromString(const char *pGUIDStr)
{
	//判断pGUIDStr是否含大括号,不含则补上
	std::string strGUIDStr(pGUIDStr);
	if (strGUIDStr.size()<2)
	{
		return GUID_NULL;
	}
	std::string strBegin = strGUIDStr.substr(0,1);
	std::string strEnd = strGUIDStr.substr(strGUIDStr.size()-1,1);

	std::string strNewGUIDStr;
	if ( strcmp(strBegin.c_str(),"{") != 0 )
	{
		strNewGUIDStr.append("{");
		strNewGUIDStr.append(strGUIDStr.c_str());
	}
	else
	{
		strNewGUIDStr = strGUIDStr;
	}
	if ( strcmp(strEnd.c_str(),"}") != 0 )
	{
		strNewGUIDStr.append("}");
	}

	GUID guid = GUID_NULL;

	int Data2, Data3;
	int d0, d1, d2, d3, d4, d5, d6, d7;
	int n = sscanf(strNewGUIDStr.c_str(),"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		&guid.Data1, &Data2, &Data3, &d0, &d1,
		&d2, &d3, &d4, &d5, &d6, &d7
		);

	if (n!=11)
	{
		ASSERT(false);
		return GUID_NULL;
	}

	guid.Data2 = Data2; guid.Data3 = Data3;
	guid.Data4[0] = d0; guid.Data4[1] = d1;
	guid.Data4[2] = d2; guid.Data4[3] = d3;
	guid.Data4[4] = d4; guid.Data4[5] = d5;
	guid.Data4[6] = d6; guid.Data4[7] = d7;

	return guid;
}

bool CSysHelper::GuidToString(const GUID &guid,char *pGUIDStr)
{
    ASSERT(pGUIDStr != NULL);
    //需要保证外部传入的 pGUIDStr 缓冲区足够大, GUID字符串长38 B
    sprintf(pGUIDStr, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);
    return true;
}

std::string CSysHelper::GuidToString(const GUID &guid)
{
	char strGuid[40] = {0};
	GuidToString(guid,strGuid);
	std::string strRet(strGuid);

	return strRet;
}

bool CSysHelper::GuidToEncrypString(const GUID &guid,char *pGUIDStr)
{
#ifdef DESENSITIZATION
	ASSERT(pGUIDStr != NULL);
	//需要保证外部传入的 pGUIDStr 缓冲区足够大, GUID字符串长38 B
	sprintf(pGUIDStr, "{%08X-%04X-****-****-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2,
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
#else
	GuidToString(guid,pGUIDStr);
#endif

	return true;
}

std::string CSysHelper::GuidToEncrypString(const GUID &guid)
{
	char strGuid[40] = {0};
	GuidToEncrypString(guid,strGuid);
	std::string strRet(strGuid);

	return strRet;
}

bool CSysHelper::MACAddressToStr(const BYTE bMAC[6], char *pStrMac, int strLen)
{
	char sBuf[64] = {0};
    sprintf(sBuf, "%02X:%02X:%02X:%02X:%02X:%02X", bMAC[0], bMAC[1], bMAC[2], bMAC[3], bMAC[4], bMAC[5]);
    if ((int)strlen(sBuf) < strLen -1)
    {
		assert(strLen >= (int)(strlen(sBuf) + 1));
        strcpy(pStrMac, sBuf);
        return true;
    }

    return false;
}

bool CSysHelper::StrToMACAddress(const char* strMAC, BYTE bMAC[6])
{
    //这样是错误的，会造成溢出
    //int n = sscanf(strMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
    //               &bMAC[0], &bMAC[1], &bMAC[2], &bMAC[3], &bMAC[4], &bMAC[5]);
    int n, m0, m1, m2, m3, m4, m5;
    n = sscanf(strMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
                       &m0, &m1, &m2, &m3, &m4, &m5);
    if (n==6)
    {
        bMAC[0] = (BYTE)m0;
        bMAC[1] = (BYTE)m1;
        bMAC[2] = (BYTE)m2;
        bMAC[3] = (BYTE)m3;
        bMAC[4] = (BYTE)m4;
        bMAC[5] = (BYTE)m5;
        return true;
    }
    return false;
}

bool CSysHelper::IPAddressToStr(const tuint32 dwIP, char *pStrIP,int strLen)
{
	char sBuf[64] = {0};
    sprintf(sBuf,"%d.%d.%d.%d", dwIP&0xff,(dwIP>>8)&0xff,(dwIP>>16)&0xff,(dwIP>>24)&0xff);
    if ((int)strlen(sBuf) < strLen -1)
    {
		assert(strLen >= (int)(strlen(sBuf) + 1));
        strcpy(pStrIP, sBuf);
        return true;
    }

    return false;
}

bool CSysHelper::StrToIPAddress(const char *strIP, tuint32 &dwIP)
{
    tuint32 n1, n2, n3, n4;
    int n = sscanf(strIP, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
    if (n == 4)
    {
        dwIP = n1 + (n2<<8) + (n3<<16) + (n4<<24);
        return true;
    }
    else
    {
        dwIP = 0;
        return false;
    }
}

bool CSysHelper::ullToString(const tuint64 ullValue, char* strValue)////转换的字符串的长度为64
{
	tuint64   offsetValue = 1;
	tuint32 __i=0;
	tuint32 dwNum = sizeof(ullValue)*8;
	for(; __i<dwNum; ++__i)
	{
		if( (offsetValue << __i) & ullValue )
		{
			strValue[dwNum-__i-1] = '1';
		}
		else
		{
 			strValue[dwNum-__i-1] = '0';
		}
	}
	strValue[__i] = 0;
	return true;	
}

bool CSysHelper::stringToUll(const char* strValue, tuint64 &ullValue)////如果字符串包含的字符除‘1’，‘0’以外还有其他字符就会转换失败
{
	ullValue = 0;
	tuint64   tmpValue = 1;
	tuint32 dwNum = strlen(strValue);
	for(tuint32 __i=0; __i<dwNum; ++__i)
	{
		if( strValue[__i] == '1' )
		{
			ullValue |= (tmpValue << (dwNum-__i-1));//高位在低下标
		}
		else if(strValue[__i] == '0')
		{
			;
		}
		else
		{
			ullValue = 0;
			assert(false);
			return false;
		}
	}
	return true;
}

static bool checkIPPart(const char *szPart, int &nPartVal)
{
    int nLen = strlen(szPart);
    if (nLen>3) return false;
    for (int i=0; i<nLen; i++)
    {
        if (szPart[i]<'0' || szPart[i]>'9') return false;
    }
    nPartVal = atoi(szPart);
    return true;
}

bool CSysHelper::IsValidIP(const char *szIP)
{
    int n, d1, d2, d3, d4;
    char szTmp1[64], szTmp2[64], szTmp3[64], szTmp4[64];
    //n =sscanf(szIP, "%03d.%03d.%03d.%03d%s", &d1, &d2, &d3, &d4, szTmp);  //这样不能判断 192.168.6.1212 的情形
    n =sscanf(szIP, "%[^.].%[^.].%[^.].%[^.]", szTmp1, szTmp2, szTmp3, szTmp4);
    if (n!=4) return false;

    //检查各分割是否全为数字
    bool bRet = checkIPPart(szTmp1, d1);
    if (!bRet) return false;
    bRet = checkIPPart(szTmp2, d2);
    if (!bRet) return false;
    bRet = checkIPPart(szTmp3, d3);
    if (!bRet) return false;
    bRet = checkIPPart(szTmp4, d4);
    if (!bRet) return false;

    if ( (d1>=1 && d1<=223) && (d2>=0 && d2<=255) && (d3>=0 && d3<=255) && (d4>=0 && d4<=255) )
    {
        //printf("get %d.%d.%d.%d\n", d1, d2, d3, d4);
        return true;
    }

    return false;
}

bool CSysHelper::IsValidSubnet(const char *szSubnet)
{
    int n, d1, d2, d3, d4;
    char szTmp1[64], szTmp2[64], szTmp3[64], szTmp4[64];
    n =sscanf(szSubnet, "%[^.].%[^.].%[^.].%[^.]", szTmp1, szTmp2, szTmp3, szTmp4);
    if (n!=4) return false;

    //检查各分割是否全为数字
    bool bRet = checkIPPart(szTmp1, d1);
    if (!bRet) return false;
    bRet = checkIPPart(szTmp2, d2);
    if (!bRet) return false;
    bRet = checkIPPart(szTmp3, d3);
    if (!bRet) return false;
    bRet = checkIPPart(szTmp4, d4);
    if (!bRet) return false;

    if ( (d1>=0 && d1<=255) && (d2>=0 && d2<=255) && (d3>=0 && d3<=255) && (d4>=0 && d4<=255) )
    {
        //printf("get %d.%d.%d.%d\n", d1, d2, d3, d4);
        return true;
    }

    return false;
}

tuint32 CSysHelper::GetBuildDate(void)
{
    const char *pDate=__DATE__;
    const char *pTime=__TIME__;
    const tuint32 hour=atoi(pTime);
    const tuint32 year=atoi(pDate+7);
    const tuint32 day=atoi(pDate+4);
    const tuint32 res=(year<<16)|(day<<5)|(hour);

    //printf("__DATE__=%s, __TIME__=%s\n", pDate, pTime);
    if(strncmp(pDate,"Jan",3)==0)
    {
        return res|(1<<10);
    }
    if(strncmp(pDate,"Feb",3)==0)
    {
        return  res|(2<<10);
    }
    if(strncmp(pDate,"Mar",3)==0)
    {
        return  res|(3<<10);
    }
    if(strncmp(pDate,"Apr",3)==0)
    {
        return res|(4<<10);
    }
    if(strncmp(pDate,"May",3)==0)
    {
        return  res|(5<<10);
    }
    if(strncmp(pDate,"Jun",3)==0)
    {
        return  res|(6<<10);
    }
    if(strncmp(pDate,"Jul",3)==0)
    {
        return  res|(7<<10);
    }
    if(strncmp(pDate,"Aug",3)==0)
    {
        return  res|(8<<10);
    }
    if(strncmp(pDate,"Sep",3)==0)
    {
        return  res|(9<<10);
    }
    if(strncmp(pDate,"Oct",3)==0)
    {
        return  res|(10<<10);
    }
    if(strncmp(pDate,"Nov",3)==0)
    {
        return  res|(11<<10);
    }
    if(strncmp(pDate,"Dec",3)==0)
    {
        return  res|(12<<10);
    }
    return 0;
}

void CSysHelper::ParseBuildDate(tuint32 dwBuildDate, int &nYear, int &nMonth, int &nDay, int &nHour)
{
    nYear = dwBuildDate>>16;
    nMonth = (dwBuildDate>>10) & 0x1f;
    nDay = (dwBuildDate>>5) & 0x1f;
    nHour = dwBuildDate & 0x1f;
    //printf("get %d-%d-%d %d\n", nYear, nMonth, nDay, nHour);
}

/*
"00:05:69"; //vmware1
"00:0C:29"; //vmware2
"00:50:56"; //vmware3
"00:1C:14"; //vmware4
"00:1C:42"; //parallels1
"00:03:FF"; //microsoft virtual pc
"00:0F:4B"; //virtual iron 4
"00:16:3E"; //red hat xen , oracle vm , xen source, novell xen
"08:00:27"; //virtualbox
*/
//是否是虚拟网卡 是返回true 00:50:56:C0:00:08
bool CSysHelper::IsVirtualMac(const char * mac)
{
	string strMac(mac);
	strMac = CSysHelper::ToUpper((char*)strMac.c_str());//转成大写
	if (strMac.size()<=8)
	{
		return false;
	}
	string strMacHead=strMac.substr(0,8); 
	map<string,string> virtualMap;
	virtualMap["00:05:69"]="00:05:69";
	virtualMap["00:0C:29"]="00:0C:29";
	virtualMap["00:50:56"]="00:50:56";
	virtualMap["00:1C:14"]="00:1C:14";
	virtualMap["00:1C:42"]="00:1C:42";
	virtualMap["00:03:FF"]="00:03:FF";
	virtualMap["00:0F:4B"]="00:0F:4B";
	virtualMap["00:16:3E"]="00:16:3E";
	virtualMap["08:00:27"]="08:00:27";
	map<string,string>::iterator it = virtualMap.find(strMacHead);
	if (it!=virtualMap.end())
	{
		return true;
	}
	return false;
}

#ifdef WIN32
std::string TCHAR2STRING(TCHAR *STR)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	memset(chRtn,0,iLen*sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)STR, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	delete [] chRtn;
	chRtn = NULL;
	return str.c_str();
}
#define PROPERTY_MAX_LEN	128	// 属性字段最大长度
struct T_DEVICE_PROPERTY
{
	TCHAR szProperty[PROPERTY_MAX_LEN];
	DWORD dwIndex;//用于判断系统分区在哪个硬盘
} ;

typedef struct _T_WQL_QUERY
{
	CHAR*	szSelect;		// SELECT语句
	WCHAR*	szProperty;		// 属性字段
} T_WQL_QUERY;

// WQL查询语句
const T_WQL_QUERY szWQLQuery[] = {
	//0 网卡原生MAC地址
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",
	L"PNPDeviceID",

	//1 硬盘序列号
	"SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",

	//2 主板序列号
	"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",	

	//3 处理器ID
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)",
	L"ProcessorId",

	//4 BIOS序列号
	"SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)",
	L"SerialNumber",

	//5 主板型号
	"SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)",
	L"Product",

	//6 网卡当前MAC地址
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))",
	L"MACAddress",
};

// 通过“PNPDeviceID”获取网卡原生MAC地址
static BOOL WMI_DoWithPNPDeviceID( const TCHAR *PNPDeviceID, TCHAR *MacAddress, UINT uSize )
{
	TCHAR	DevicePath[MAX_PATH];
	HANDLE	hDeviceFile;	
	BOOL	isOK = FALSE;

	// 生成设备路径名
	StringCchCopy( DevicePath, MAX_PATH, TEXT("\\\\.\\") );
	StringCchCat( DevicePath, MAX_PATH, PNPDeviceID );
	StringCchCat( DevicePath, MAX_PATH, TEXT("#{ad498944-762f-11d0-8dcb-00c04fc3358c}") );

	// 将“PNPDeviceID”中的“\”替换成“#”，以获得真正的设备路径名
	std::replace( DevicePath + 4, DevicePath + 4 + _tcslen(PNPDeviceID), TEXT('\\'), TEXT('#') ); 

	// 获取设备句柄
	hDeviceFile = CreateFile( DevicePath,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if( hDeviceFile != INVALID_HANDLE_VALUE )
	{	
		ULONG	dwID;
		BYTE	ucData[8];
		DWORD	dwByteRet;		

		// 获取网卡原生MAC地址
		dwID = OID_802_3_PERMANENT_ADDRESS;
		isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );
		if( isOK )
		{	// 将字节数组转换成16进制字符串
			for( DWORD i = 0; i < dwByteRet; i++ )
			{
				StringCchPrintf( MacAddress + (i << 1), uSize - (i << 1), TEXT("%02X"), ucData[i] );
			}
		}

		CloseHandle( hDeviceFile );
	}

	return isOK;
}

static BOOL WMI_DoWithHarddiskSerialNumber( TCHAR *SerialNumber, UINT uSize )
{
	UINT	iLen;
	UINT	i;

	iLen = _tcslen( SerialNumber );
	if( iLen == 40 )	// InterfaceType = "IDE"
	{	// 需要将16进制编码串转换为字符串
		TCHAR ch, szBuf[32]={0};
		BYTE b;		

		for( i = 0; i < 20; i++ )
		{	// 将16进制字符转换为高4位
			ch = SerialNumber[i * 2];
			if( (ch >= '0') && (ch <= '9') )
			{
				b = ch - '0';
			}
			else if( (ch >= 'A') && (ch <= 'F') )
			{
				b = ch - 'A' + 10;
			}
			else if( (ch >= 'a') && (ch <= 'f') )
			{
				b = ch - 'a' + 10;
			}
			else
			{	// 非法字符
				break;
			}

			b <<= 4;

			// 将16进制字符转换为低4位
			ch = SerialNumber[i * 2 + 1];
			if( (ch >= '0') && (ch <= '9') )
			{
				b += ch - '0';
			}
			else if( (ch >= 'A') && (ch <= 'F') )
			{
				b += ch - 'A' + 10;
			}
			else if( (ch >= 'a') && (ch <= 'f') )
			{
				b += ch - 'a' + 10;
			}
			else
			{	// 非法字符
				break;
			}

			szBuf[i] = b;
		}

		if( i == 20 )
		{	// 转换成功
			szBuf[i] = L'\0';
			StringCchCopy( SerialNumber, uSize, szBuf );
			iLen = _tcslen( SerialNumber );
		}

		// 每2个字符互换位置
		for( i = 0; i < iLen; i += 2 )
		{
			std::swap( SerialNumber[i], SerialNumber[i+1] );
		}
	}

	// 去掉空格
	std::remove( SerialNumber, SerialNumber + _tcslen(SerialNumber) + 1, L' ' );
	std::remove( SerialNumber, SerialNumber + _tcslen(SerialNumber) + 1, L'-' );
	std::remove( SerialNumber, SerialNumber + _tcslen(SerialNumber) + 1, L'_' );
	std::remove( SerialNumber, SerialNumber + _tcslen(SerialNumber) + 1, L':' );
	std::remove( SerialNumber, SerialNumber + _tcslen(SerialNumber) + 1, L'.' );
	return TRUE;
}
static BOOL WMI_DoWithProperty( tuint32 iQueryType, TCHAR *szProperty, tuint32 uSize )
{
	BOOL isOK = TRUE;
	switch( iQueryType )
	{
	case 0:		// 网卡原生MAC地址		
		isOK = WMI_DoWithPNPDeviceID( szProperty, szProperty, uSize );
		break;
	case 1:		// 硬盘序列号
		isOK = WMI_DoWithHarddiskSerialNumber( szProperty, uSize );
		break;
	case 6:		// 网卡当前MAC地址
		// 去掉冒号
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L':' );
		break;
	default:
		// 去掉空格
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L' ' );
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L'-' );
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L'_' );
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L':' );
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L'.' );
	}
	return isOK;
}
// 基于Windows Management Instrumentation（Windows管理规范） iQueryType=1查询硬盘信息 =2查询主板信息 对应数组szWQLQuery
tuint32 WMI_DeviceQuery( tuint32 iQueryType,std::vector<T_DEVICE_PROPERTY>& PropertyVec)
{
	HRESULT hres;
	tuint32	iTotal = 0;

	// 判断查询类型是否支持
	if( (iQueryType < 0) || (iQueryType >= sizeof(szWQLQuery)/sizeof(T_WQL_QUERY)) )
	{
		return -1;	// 查询类型不支持
	}
	// 初始化COM
	hres = CoInitializeEx( NULL, COINIT_MULTITHREADED ); 
	if( FAILED(hres) )
	{
		return -2;
	}

	// 设置COM的安全认证级别
	hres = CoInitializeSecurity(NULL,-1,NULL,NULL,RPC_C_AUTHN_LEVEL_DEFAULT,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE,NULL);
	if( FAILED(hres) )
	{
		if (hres!= 0x80010119)
		{
			CoUninitialize();
			return -2;
		}
	}

	// 获得WMI连接COM接口
	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator,NULL,CLSCTX_INPROC_SERVER,IID_IWbemLocator,reinterpret_cast<LPVOID*>(&pLoc)); 
	if( FAILED(hres) )
	{
		CoUninitialize();
		return -2;
	}
	// 通过连接接口连接WMI的内核对象名"ROOT\\CIMV2"
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(_bstr_t( L"ROOT\\CIMV2" ),NULL,NULL,NULL,0,NULL,NULL,&pSvc);    
	if( FAILED(hres) )
	{
		pLoc->Release(); 
		CoUninitialize();
		return -2;
	}
	// 设置请求代理的安全级别
	hres = CoSetProxyBlanket(pSvc,RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE);
	if( FAILED(hres) )
	{
		pSvc->Release();
		pLoc->Release();     
		CoUninitialize();
		return -2;
	}
	// 通过请求代理来向WMI发送请求
	IEnumWbemClassObject *pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"),bstr_t( szWQLQuery[iQueryType].szSelect),WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&pEnumerator);
	if( FAILED(hres) )
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -3;
	}
	// 循环枚举所有的结果对象  
	while( pEnumerator )
	{
		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;

		T_DEVICE_PROPERTY properties;
		memset(&properties,0,sizeof(T_DEVICE_PROPERTY));

		pEnumerator->Next(WBEM_INFINITE,1,&pclsObj,&uReturn);
		if( uReturn == 0 )
		{
			break;
		}
		// 获取属性值
		VARIANT vtProperty;

		VariantInit( &vtProperty );	
		pclsObj->Get( szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL );
		USES_CONVERSION; StringCchCopy( properties.szProperty, PROPERTY_MAX_LEN, W2T(vtProperty.bstrVal) );
		VariantClear( &vtProperty );
		// 对属性值做进一步的处理
		if( WMI_DoWithProperty( iQueryType, properties.szProperty, PROPERTY_MAX_LEN ) )
		{
			iTotal++;
		}

		if (iQueryType == 1)//查询硬盘信息
		{
			VARIANT vtIndex;
			VariantInit( &vtIndex );	
			pclsObj->Get(L"Index",0,&vtIndex,0,0);
			properties.dwIndex =  vtIndex.intVal;
			VariantClear( &vtIndex );
		}
		PropertyVec.push_back(properties);

		pclsObj->Release();
	} // End While
	// 释放资源
	pEnumerator->Release();
	pSvc->Release();
	pLoc->Release();    
	CoUninitialize();
	return iTotal;
}

DWORD GetPhysicalDriveFromPartitionLetter(CHAR letter)
{
	HANDLE hDevice = NULL;               // handle to the drive to be examined
	BOOL result;                 // results flag
	DWORD readed;                   // discard results
	STORAGE_DEVICE_NUMBER number;   //use this to get disk numbers

	char path[MAX_PATH]={0};
	sprintf(path, "\\\\.\\%c:", letter);
	hDevice = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);           // do not copy file attribute
	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		//fprintf(stderr, "CreateFile() Error: %ld\n", GetLastError());
		return DWORD(-1);
	}

	result = DeviceIoControl(
		hDevice,                // handle to device
		IOCTL_STORAGE_GET_DEVICE_NUMBER, // dwIoControlCode
		NULL,                            // lpInBuffer
		0,                               // nInBufferSize
		&number,           // output buffer
		sizeof(number),         // size of output buffer
		&readed,       // number of bytes returned
		NULL      // OVERLAPPED structure
		);
	if (!result) // fail
	{
		fprintf(stderr, "IOCTL_STORAGE_GET_DEVICE_NUMBER Error: %ld\n", GetLastError());
		(void)CloseHandle(hDevice);
		return (DWORD)-1;
	}
	//printf("%d %d %d\n\n", number.DeviceType, number.DeviceNumber, number.PartitionNumber);

	(void)CloseHandle(hDevice);
	return number.DeviceNumber;
}

DWORD GetSystemDiskPhysicalIndex(void)
{
	CHAR sysPath[512]={0};
	CHAR diskLetter;
	DWORD diskIndex;

	DWORD ret = GetSystemDirectoryA(sysPath, sizeof(sysPath));
	if (ret == 0)
	{
		fprintf(stderr, "GetSystemDirectory() Error: %ld\n", GetLastError());
		return (DWORD)-1;
	}
	diskLetter = sysPath[0];
	diskIndex = GetPhysicalDriveFromPartitionLetter(diskLetter);
	return diskIndex;
}


string GetHDSerial()
{
	string strHDSerial;
	DWORD HDIndex = GetSystemDiskPhysicalIndex();//获取操作系统所在的硬盘index
	std::vector<T_DEVICE_PROPERTY> hdPropertyVec;
	WMI_DeviceQuery(1, hdPropertyVec);
	for (tuint32 i=0;i<hdPropertyVec.size();i++)
	{
		if (hdPropertyVec[i].dwIndex == HDIndex)
		{
			strHDSerial=TCHAR2STRING(hdPropertyVec[i].szProperty);
			break;
		}
	}
	
	if ( (strHDSerial.size()==0)&&(hdPropertyVec.size()>0) )
	{
		strHDSerial=TCHAR2STRING(hdPropertyVec[0].szProperty);
	}
	return strHDSerial.c_str();
}

string GetBoardSerial()//获取主板序列号
{
	string strBoardSerial;
	std::vector<T_DEVICE_PROPERTY> hdPropertyVec;
	WMI_DeviceQuery(2, hdPropertyVec);
	if (hdPropertyVec.size()>0)
	{
		strBoardSerial=TCHAR2STRING(hdPropertyVec[0].szProperty);
	}
	
	if ( (std::string::npos !=  strBoardSerial.find("None"))||(std::string::npos !=  strBoardSerial.find(".")) )
	{
		strBoardSerial.clear();
	}
	return strBoardSerial;
}

#else
static void parse_cpu_id(const char * file_name, const char * match_words, std::string & cpu_id)
{
	cpu_id.c_str();
	std::ifstream ifs(file_name, std::ios::binary);
	if (!ifs.is_open())
	{
		return;
	}
	char line[4096] = { 0 };
	while (!ifs.eof())
	{
		ifs.getline(line, sizeof(line));
		if (!ifs.good())
		{
			break;
		}
		const char * cpu = strstr(line, match_words);
		if (NULL == cpu)
		{
			continue;
		}
		cpu += strlen(match_words);
		while ('\0' != cpu[0])
		{
			if (' ' != cpu[0])
			{
				cpu_id.push_back(cpu[0]);
			}
			++cpu;
		}
		if (!cpu_id.empty())
		{
			break;
		}
	}
	ifs.close();
}

static bool get_cpu_id_by_system(std::string & cpu_id)
{
	cpu_id.c_str();
	const char * dmidecode_result = ".dmidecode_result.txt";
	char command[512] = { 0 };
	snprintf(command, sizeof(command), "/usr/sbin/dmidecode -t 4 | grep ID > %s", dmidecode_result);
	if (0 == system(command))
	{
		parse_cpu_id(dmidecode_result, "ID:", cpu_id);
	}
	unlink(dmidecode_result);
	return(!cpu_id.empty());
}

#define WORD(x) (tuint16)(*(const tuint16 *)(x))
#define DWORD(x) (tuint32)(*(const tuint32 *)(x))
#define QWORD(x) (*(const tuint64 *)(x))

struct dmi_header
{
        tuint8 type;
        tuint8 length;
        tuint16 handle;
        tuint8 *data;
};

static int myread(int fd, tuint8 *buf, size_t count, const char *prefix)
{
    ssize_t r = 1;
    size_t r2 = 0;

    while (r2 != count && r != 0)
    {
        r = read(fd, buf + r2, count - r2);
        if (r == -1)
        {
            if (errno != EINTR)
            {
                close(fd);
                perror(prefix);
                return -1;
            }
        }
        else
            r2 += r;
    }

    if (r2 != count)
    {
        close(fd);
        fprintf(stderr, "%s: Unexpected end of file\n", prefix);
        return -1;
    }

    return 0;
}

int checksum(const tuint8 *buf, size_t len)
{
    tuint8 sum = 0;
    size_t a;

    for (a = 0; a < len; a++)
        sum += buf[a];
    return (sum == 0);
}

void *mem_chunk(size_t base, size_t len, const char *devmem)
{
    void *p;
    int fd;

    if ((fd = open(devmem, O_RDONLY)) == -1)
    {
        perror(devmem);
        return NULL;
    }

    if ((p = malloc(len)) == NULL)
    {
        perror("malloc");
        return NULL;
    }

try_read:
    if (lseek(fd, base, SEEK_SET) == -1)
    {
        fprintf(stderr, "%s: ", devmem);
        perror("lseek");
        free(p);
        return NULL;
    }

    if (myread(fd, (tuint8 *)p, len, devmem) == -1)
    {
        free(p);
        return NULL;
    }

out:
    if (close(fd) == -1)
        perror(devmem);

    return p;
}

void to_dmi_header(struct dmi_header *h, tuint8 *data)
{
    h->type = data[0];
    h->length = data[1];
    h->handle = WORD(data + 2);
    h->data = data;
}

tuint32 get_cpuid(char *cpuid)
{
    char devmem[10] = "/dev/mem";
    tuint16 len;
    tuint16 num;
    size_t fp;
    tuint8 *buf=NULL, *nbuf=NULL, *data, *p;
    int i = 0;
    if ((buf = (tuint8 *)mem_chunk(0xF0000, 0x10000, devmem)) == NULL)
    {
        free(buf);
        return 0;
    }

    for (fp = 0; fp <= 0xFFF0; fp += 16)
    {
        if(memcmp(buf + fp, "_SM_", 4) == 0 && fp <= 0xFFE0)
        {
            len = WORD(buf + fp + 0x16);
            num = WORD(buf + fp + 0x1C);

            if (!checksum(buf + fp, (buf + fp)[0x05]) || memcmp(buf + fp + 0x10, "_DMI_", 5) != 0 || !checksum(buf + fp + 0x10, 0x0F))
            {
                free(buf);
                return 0;
            }
            if ((nbuf = (tuint8 *)mem_chunk(DWORD(buf + fp + 0x18), len, devmem)) == NULL)
            {
                fprintf(stderr, "Table is unreachable, sorry.\n");
                free(buf);
                free(nbuf);
                return 0;
            }
            data = nbuf;
            while (i < num && data+4 <= nbuf + len)
            {
                tuint8 *next;
                struct dmi_header h;

                to_dmi_header(&h, data);

                if (h.length < 4)
                {
                    printf("Invalid entry length (%u). DMI table is "
                           "broken! Stop.\n\n", (unsigned int)h.length);
                    free(buf);
                    free(nbuf);
                    return 0;
                }

                next = data + h.length;
                while (next - nbuf + 1 < len && (next[0] != 0 || next[1] != 0))
                    next++;
                next += 2;
                if (h.type ==4)
                {
                    p = h.data + 0x08;
                    sprintf(cpuid, "%02X%02X%02X%02X%02X%02X%02X%02X", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
                    free(buf);
                    free(nbuf);
                    return 1;
                }
                data = next;
                i++;
            }
            fp += 16;
        }
    }
    free(buf);
    free(nbuf);
    return 0;
}


/*获取硬盘序列号*/
static bool get_disk_name(std::string & disk_name)
{
    disk_name.c_str();

    std::ifstream ifs("/etc/mtab", std::ios::binary);
    if (!ifs.is_open())
    {
        return(false);
    }

    char line[4096] = { 0 };
    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));
        if (!ifs.good())
        {
            break;
        }

        const char * disk = line;
        while (isspace(disk[0]))
        {
            ++disk;
        }

        const char * space = strchr(disk, ' ');
        if (NULL == space)
        {
            continue;
        }

        const char * mount = space + 1;
        while (isspace(mount[0]))
        {
            ++mount;
        }
        if ('/' != mount[0] || ' ' != mount[1])
        {
            continue;
        }

        while (space > disk && isdigit(space[-1]))
        {
            --space;
        }

        if (space > disk)
        {
            std::string(disk, space).swap(disk_name);
            break;
        }
    }

    ifs.close();

    return(!disk_name.empty());
}

static void trim_serial(const void * serial, size_t serial_len, std::string & serial_no)
{
    const char * serial_s = static_cast<const char *>(serial);
    const char * serial_e = serial_s + serial_len;
    while (serial_s < serial_e)
    {
        if (isspace(serial_s[0]))
        {
            ++serial_s;
        }
        else if ('\0' == serial_e[-1] || isspace(serial_e[-1]))
        {
            --serial_e;
        }
        else
        {
            break;
        }
    }

    if (serial_s < serial_e)
    {
        std::string(serial_s, serial_e).swap(serial_no);
    }
}

static bool get_disk_serial_by_way_1(const std::string & disk_name, std::string & serial_no)
{
    serial_no.clear();

    int fd = open(disk_name.c_str(), O_RDONLY);
    if (-1 == fd)
    {
        return(false);
    }
#ifndef __ENVIRONMENT_MAC__
    struct hd_driveid drive ;//= { 0 };
    memset(&drive, 0,sizeof(drive) );
    if (0 == ioctl(fd, HDIO_GET_IDENTITY, &drive))
    {
        trim_serial(drive.serial_no, sizeof(drive.serial_no), serial_no);
    }
#endif
    close(fd);

    return(!serial_no.empty());
}

static bool scsi_io(
                int fd, unsigned char * cdb,
                unsigned char cdb_size, int xfer_dir,
                unsigned char * data, unsigned int data_size,
                unsigned char * sense, unsigned int sense_len
            )
{
#if (!defined __ENVIRONMENT_LINUX_ANDROID__) && (!defined __ENVIRONMENT_MAC__)
    sg_io_hdr_t io_hdr = { 0 };
    io_hdr.interface_id = 'S';
    io_hdr.cmdp = cdb;
    io_hdr.cmd_len = cdb_size;
    io_hdr.sbp = sense;
    io_hdr.mx_sb_len = sense_len;
    io_hdr.dxfer_direction = xfer_dir;
    io_hdr.dxferp = data;
    io_hdr.dxfer_len = data_size;
    io_hdr.timeout = 5000;

    if (ioctl(fd, SG_IO, &io_hdr) < 0)
    {
        return(false);
    }

    if (SG_INFO_OK != (io_hdr.info & SG_INFO_OK_MASK) && io_hdr.sb_len_wr > 0)
    {
        return(false);
    }

    if (io_hdr.masked_status || io_hdr.host_status || io_hdr.driver_status)
    {
        return(false);
    }
#endif
    return(true);
}

static bool get_disk_serial_by_way_2(const std::string & disk_name, std::string & serial_no)
{
    serial_no.clear();
#if (!defined __ENVIRONMENT_LINUX_ANDROID__) && (!defined __ENVIRONMENT_MAC__)
    int fd = open(disk_name.c_str(), O_RDONLY);
    if (-1 == fd)
    {
        return(false);
    }

    int version = 0;

	if (ioctl(fd, SG_GET_VERSION_NUM, &version) < 0 || version < 30000)
    {
        close(fd);
        return(false);
    }

    const unsigned int data_size = 0x00ff;
    unsigned char data[data_size] = { 0 };
    const unsigned int sense_len = 32;
    unsigned char sense[sense_len] = { 0 };
    unsigned char cdb[] = { 0x12, 0x01, 0x80, 0x00, 0x00, 0x00 };
    cdb[3] = (data_size >> 8) & 0xff;
    cdb[4] = (data_size & 0xff);

    if (scsi_io(fd, cdb, sizeof(cdb), SG_DXFER_FROM_DEV, data, data_size, sense, sense_len))
    {
        int page_len = data[3];
        trim_serial(data + 4, page_len, serial_no);
    }

    close(fd);

#endif

    return(!serial_no.empty());
}

static bool parse_serial(const char * line, int line_size, const char * match_words, std::string & serial_no)
{
    const char * serial_s = strstr(line, match_words);
    if (NULL == serial_s)
    {
        return(false);
    }
    serial_s += strlen(match_words);
    while (isspace(serial_s[0]))
    {
        ++serial_s;
    }

    const char * serial_e = line + line_size;
    const char * comma = strchr(serial_s, ',');
    if (NULL != comma)
    {
        serial_e = comma;
    }

    while (serial_e > serial_s && isspace(serial_e[-1]))
    {
        --serial_e;
    }

    if (serial_e <= serial_s)
    {
        return(false);
    }

    std::string(serial_s, serial_e).swap(serial_no);

    return(true);
}

static void get_serial(const char * file_name, const char * match_words, std::string & serial_no)
{
    serial_no.c_str();

    std::ifstream ifs(file_name, std::ios::binary);
    if (!ifs.is_open())
    {
        return;
    }

    char line[4096] = { 0 };
    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));
        if (!ifs.good())
        {
            break;
        }

        if (0 == ifs.gcount())
        {
            continue;
        }

        if (parse_serial(line, ifs.gcount() - 1, match_words, serial_no))
        {
            break;
        }
    }

    ifs.close();
}

static bool get_disk_serial_by_way_3(const std::string & disk_name, std::string & serial_no)
{
    serial_no.c_str();

    const char * hdparm_result = ".hdparm_result.txt";
    char command[512] = { 0 };
    snprintf(command, sizeof(command), "hdparm -i %s | grep SerialNo > %s", disk_name.c_str(), hdparm_result);

    if (0 == system(command))
    {
        get_serial(hdparm_result, "SerialNo=", serial_no);
    }

    unlink(hdparm_result);

    return(!serial_no.empty());
}

static bool get_disk_serial_by_way_4(std::string & serial_no)
{
    serial_no.c_str();

    const char * lshw_result = ".lshw_result.txt";
    char command[512] = { 0 };
    snprintf(command, sizeof(command), "lshw -class disk | grep serial > %s", lshw_result);

    if (0 == system(command))
    {
        get_serial(lshw_result, "serial:", serial_no);
    }

    unlink(lshw_result);

    return(!serial_no.empty());
}

static bool get_disk_serial_number(std::string & serial_no)
{
    if (0 != getuid())
    {
        return(false);
    }

    std::string disk_name;
    //if (get_disk_name(disk_name))
    {
       // printf("disk_name:[%s]\n", disk_name.c_str());
        if (get_disk_serial_by_way_1("/dev/sda", serial_no))
        {
            return(true);
        }
        if (get_disk_serial_by_way_2("/dev/sda", serial_no))
        {
            return(true);
        }
        if (get_disk_serial_by_way_3("/dev/sda", serial_no))
        {
            return(true);
        }
    }
    if (get_disk_serial_by_way_4(serial_no))
    {
        return(true);
    }
    return(false);
}

//获取主板序列号
static void parse_board_serial(const char * file_name, const char * match_words, std::string & board_serial)
{
    board_serial.c_str();

    std::ifstream ifs(file_name, std::ios::binary);
    if (!ifs.is_open())
    {
        return;
    }

    char line[4096] = { 0 };
    while (!ifs.eof())
    {
        ifs.getline(line, sizeof(line));
        if (!ifs.good())
        {
            break;
        }

        const char * board = strstr(line, match_words);
        if (NULL == board)
        {
            continue;
        }
        board += strlen(match_words);

        while ('\0' != board[0])
        {
            if (' ' != board[0])
            {
                board_serial.push_back(board[0]);
            }
            ++board;
        }

        if ("None" == board_serial)
        {
            board_serial.clear();
            continue;
        }

        if (!board_serial.empty())
        {
            break;
        }
    }

    ifs.close();
}

static bool get_board_serial_by_system(std::string & board_serial)
{
    board_serial.c_str();

    const char * dmidecode_result = ".dmidecode_result.txt";
    char command[512] = { 0 };
    snprintf(command, sizeof(command), "sudo dmidecode -t 2 | grep Serial > %s", dmidecode_result);

    if (0 == system(command))
    {
        parse_board_serial(dmidecode_result, "Serial Number:", board_serial);
    }

    unlink(dmidecode_result);

    return(!board_serial.empty());
}

static bool get_board_serial_number(std::string & board_serial)
{
    if (0 == getuid())
    {
        if (get_board_serial_by_system(board_serial))
        {
            return(true);
        }
    }
    return(false);
}

#endif

//获取cpu信息
string CSysHelper::GetCpuID()
{
#ifdef WIN32
    INT32 dwBuf[4];
    __cpuidex(dwBuf, 1, 1);
    char szCPUID[33]={NULL};
    sprintf(szCPUID, "%08X%08X", dwBuf[3], dwBuf[0]);
    return string(szCPUID);
#elif defined __ENVIRONMENT_MAC__
    return "";
#else
    char szCpu[128]={0};
    tuint32 nRet = get_cpuid(szCpu);
    if( (nRet == 0)||(strlen(szCpu)==0) )
    {
        string strCpuID;
        get_cpu_id_by_system(strCpuID);
        return strCpuID;
    }
    else
    {
        string strCpuID(szCpu);
        return strCpuID;
    }
#endif
}

std::string CSysHelper::DelSubString( std::string strSrc, std::string strSub)
{ 
	string::size_type nPos = 0;
	while(nPos !=string::npos )
	{
		nPos = strSrc.find(strSub);
		if (nPos!=string::npos)
		{
			string strBegin = strSrc.substr(0,nPos);
			string strEnd = strSrc.substr(nPos+strSub.size(),strSrc.size()-nPos-strSub.size());
			strSrc.clear();
			strSrc.append(strBegin.c_str());
			strSrc.append(strEnd.c_str());
		}

		nPos = -1;
		nPos = strSrc.find(strSub);
	}
	return strSrc;
}


//获取主机硬件标识
string CSysHelper::GetDecrInfo()
{
	//获取cpu信息
	string strCPUInfo = GetCpuID();
#ifdef WIN32
    //网卡信息
    string strMAC;
    std::list<CPFHelper::TNICInfo> NICList;
    CPFHelper::GetNICList(NICList); //临时记录列表
	//用map做一个排序，取出最小的mac地址
	std::list<CPFHelper::TNICInfo>::iterator itNIC ;
	std::map<std::string,std::string> mapMac;
	for (itNIC = NICList.begin();itNIC!=NICList.end();itNIC++)
	{
		char  szMACAddr[24] = {0};
		memset(szMACAddr, 0, sizeof(szMACAddr));
		CSysHelper::MACAddressToStr(itNIC->MAC,szMACAddr,sizeof(szMACAddr));
		if (IsVirtualMac(szMACAddr) == false)
		{
			mapMac[szMACAddr]=szMACAddr;
		}
	}

	//取出第一张非虚拟网卡的mac地址	
	std::map<std::string,std::string>::iterator itMap = mapMac.begin() ;
	for (itMap = mapMac.begin();itMap!=mapMac.end();itMap++)
	{
		strMAC = DelSubString(itMap->first,":");//去掉":"号
		break;
	}
    string strDeriveKey = strCPUInfo + strMAC ;
#else
	string strDeriveKey = Linux_GetDecrInfo();
#endif
	return strDeriveKey;
}

//获取主机硬件标识(windows用的cpu+硬盘,linux用的cpu+硬盘)
std::string CSysHelper::GetDecrInfoByDisk()
{
	//获取cpu信息
	string strCPUInfo = GetCpuID();
#ifdef WIN32
	//硬盘序列号
	string strSerialNo = GetHDSerial();
	if (strSerialNo.size()<=0)
	{
		strSerialNo = GetBoardSerial();
		if (strSerialNo.size()<=0)
		{
			//网卡信息
			string strMAC;
			std::list<CPFHelper::TNICInfo> NICList;
			CPFHelper::GetNICList(NICList); //临时记录列表
			//用map做一个排序，取出最小的mac地址
			std::list<CPFHelper::TNICInfo>::iterator itNIC ;
			std::map<std::string,std::string> mapMac;
			for (itNIC = NICList.begin();itNIC!=NICList.end();itNIC++)
			{
				char  szMACAddr[24] = {0};
				memset(szMACAddr, 0, sizeof(szMACAddr));
				CSysHelper::MACAddressToStr(itNIC->MAC,szMACAddr,sizeof(szMACAddr));
				if (IsVirtualMac(szMACAddr) == false)
				{
					mapMac[szMACAddr]=szMACAddr;
				}
			}

			//取出第一张非虚拟网卡的mac地址	
			std::map<std::string,std::string>::iterator itMap = mapMac.begin() ;
			for (itMap = mapMac.begin();itMap!=mapMac.end();itMap++)
			{
				strSerialNo = DelSubString(itMap->first,":");//去掉":"号
				break;
			}
		}
	}
	string strTemp = strSerialNo;
	if(strTemp.size()>=12)
	{
		int pos = strTemp.length() - 12;
		strTemp = strTemp.substr(pos,12);
		strSerialNo = strTemp;
	}
	else if(strTemp.size()<12)
	{
		char szTemp[20]={0};
		sprintf(szTemp, "%0*d%s", 12-strTemp.size(), 0, strTemp.c_str());
		strSerialNo = szTemp;
	}
	string strDeriveKey = strCPUInfo + strSerialNo ;
	strDeriveKey = CSysHelper::ToUpper((char*)strDeriveKey.c_str());//转成大写
#elif defined __ENVIRONMENT_MAC__
    // 获取系统UUID，参考链接：
    // https://stackoverflow.com/questions/933460/unique-hardware-id-in-mac-os-x
    // https://juejin.im/post/5c541560f265da2d9b5deb1c
    string strDeriveKey;
    char* pCmd = "ioreg -rd1 -c IOPlatformExpertDevice | awk '/IOPlatformUUID/ { split($0, line, \"\\\"\"); printf(\"%s\\n\", line[4]); }'";
    FILE* fp = popen(pCmd, "r");
    if (fp)
    {
        char sOutput[128] = {0};
        if(fgets(sOutput, sizeof(sOutput) -1, fp) != NULL)//only first line
        {
            // 去除横杆和换行
            for (int i = 0; i < strlen(sOutput); i++)
            {
                if (sOutput[i] != '-' && sOutput[i] != '\n')
                {
                    strDeriveKey.append(1, sOutput[i]);
                }
            }
        }
        pclose(fp);
    }
#else
	string strDeriveKey = Linux_GetDecrInfo();
	strDeriveKey = CSysHelper::ToUpper((char*)strDeriveKey.c_str());//转成大写
#endif
	return strDeriveKey;
}

//字符串转换成大写
char* CSysHelper::ToUpper(char *str)
{
	char *p = str;
	while(p && *p)
	{
		if (*p >= 'a' && *p <= 'z')
		{
			*p -= 32;
		}
		++p;
	}
	return str;	
}


void CSysHelper::MByteToUTF8( char *pMbyte, size_t mbyteLen, char *pUTF8, size_t utf8Len)
{
#ifdef WIN32
	tuint32 dwMinSize = MultiByteToWideChar (CP_ACP, 0, pMbyte, mbyteLen, NULL, 0);

	WCHAR *pWTmp=new WCHAR[dwMinSize+1];
	memset(pWTmp,0,dwMinSize*2+2);
	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, pMbyte, mbyteLen, pWTmp, dwMinSize);  

	dwMinSize = WideCharToMultiByte(CP_UTF8,NULL,pWTmp,-1,NULL,0,NULL,FALSE);
	if(utf8Len < (dwMinSize+1))
	{
		delete []pWTmp;
		pWTmp = NULL;
		return ;
	}
	memset(pUTF8, 0, utf8Len);

	WideCharToMultiByte(CP_UTF8,NULL,pWTmp,-1,pUTF8,utf8Len,NULL,FALSE);
	delete []pWTmp;
	pWTmp = NULL;
#else

#if (defined __ENVIRONMENT_LINUX_ANDROID__) || (defined __ENVIRONMENT_MAC__)
 
#else
    int retsize;
	iconv_t cd;
    cd = iconv_open( "UTF-8","GBK");
	if((iconv_t)-1 == cd)
	{
		printf("iconv_open error");
        return ;
	}
    retsize = iconv(cd, &pMbyte, (size_t *)&mbyteLen, &pUTF8, (size_t *)&utf8Len);
	if((size_t)-1 == retsize)
	{
		printf("iconv error");
        return ;
	}
	if(utf8Len > 0)
	{
		printf("pUTF8=%s\n", pUTF8);
	}
	iconv_close(cd);
#endif

#endif
}

void CSysHelper::UTF8ToMByte(const  char *pUTF8, size_t utf8Len,char *pMbyte, size_t mbyteLen)
{
	memset(pMbyte, 0, mbyteLen);
#ifdef WIN32
	tuint32 dwMinSize = MultiByteToWideChar (CP_UTF8, 0, pUTF8, utf8Len, NULL, 0);

	WCHAR *pWTmp=new WCHAR[dwMinSize+1];
	memset(pWTmp,0,dwMinSize*2+2);
	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_UTF8, 0, pUTF8, utf8Len, pWTmp, dwMinSize);  

	dwMinSize = WideCharToMultiByte(CP_ACP,NULL,pWTmp,-1,NULL,0,NULL,FALSE);
	if(mbyteLen < (dwMinSize+1))
	{
		delete []pWTmp;
		pWTmp = NULL;
		return ;
	}
	WideCharToMultiByte(CP_ACP,NULL,pWTmp,-1,pMbyte,mbyteLen,NULL,FALSE);
	delete []pWTmp;
	pWTmp = NULL;
#else

#ifdef __ENVIRONMENT_LINUX_ANDROID__
#else
    char * pTmp8 = (char*)pUTF8;
	int retsize;
	iconv_t cd;
    cd = iconv_open("GBK","UTF-8");
	if((iconv_t)-1 == cd)
	{
		printf("iconv_open error");
        return ;
	}
	retsize = iconv(cd, &pTmp8, (size_t *)&utf8Len, &pMbyte, (size_t *)&mbyteLen);
	if((size_t)-1 == retsize)
	{
		printf("iconv error");
        return ;
	}
	if(utf8Len > 0)
	{
		printf("pMbyte=%s\n", pMbyte);
	}
	iconv_close(cd);
#endif

#endif
}

std::string CSysHelper::Linux_GetDecrInfo()
{
	string strDeriveKey;
	//获取cpu信息
	string strCPUInfo = GetCpuID();
#if (!defined WIN32) && (!defined __ENVIRONMENT_MAC__)
	//硬盘序列号,获取不到硬盘序列号为主板序列号 linux用 因为有mac地址会变化
	std::string strSerialNo;
	strSerialNo.clear();
	string strTemp;
	strTemp.clear();
	get_disk_serial_number(strTemp);
	if(strTemp.size()<=0)
	{
		printf("get_disk_serial_number false");
		//硬盘取不到取主板
		strTemp.clear();
		get_board_serial_number(strTemp);
		if(strTemp.size()<=0)
		{
			printf("get_board_serial_number false");
			//主板取不到才取mac
			std::list<CPFHelper::TNICInfo> NICList;
			CPFHelper::GetNICList(NICList); //临时记录列表
			std::list<CPFHelper::TNICInfo>::iterator itNIC ;
			for (itNIC = NICList.begin();itNIC!=NICList.end();itNIC++)
			{
				char  szMACAddr[24] = {0};
				memset(szMACAddr, 0, sizeof(szMACAddr));
				CSysHelper::MACAddressToStr(itNIC->MAC,szMACAddr,sizeof(szMACAddr));
				if (IsVirtualMac(szMACAddr) == false)
				{
					if(strcmp(itNIC->eth_name,"eth0") == 0)
					{
						strTemp = DelSubString(szMACAddr,":");//去掉":"号
						break;
					}
				}
			}
		}
	}

	strTemp = CSysHelper::ToUpper((char*)strTemp.c_str());//转成大写
	strTemp = DelSubString(strTemp,"-");
	strTemp = DelSubString(strTemp,"_");
	if(strTemp.size()>=12)
	{
		int pos = strTemp.length() - 12;
		strTemp = strTemp.substr(pos,12);
		strSerialNo = strTemp;
	}
	else if(strTemp.size()<12)
	{
		char szTemp[20]={0};
		sprintf(szTemp, "%0*d%s", 12-strTemp.size(), 0, strTemp.c_str());
		strSerialNo = szTemp;
	}

	strDeriveKey = strCPUInfo + strSerialNo ;
#endif
	return strDeriveKey;
}

bool CSysHelper::GetIntVersionFormStr(std::string &strVersion,tuint32 &Main,tuint32 &Sub1,tuint32 &Sub2,tuint32 &Sub3)
{
	Main=0;Sub1=0;Sub2=0;Sub3=0;
	std::vector<std::string> dst;
	StringSplit(strVersion,".",dst);
	for (tuint32 i=0;i<dst.size();i++)
	{
		if (i==0)
		{
			sscanf(dst[i].c_str(),"%d",&Main);
		}
		if (i==1)
		{
			sscanf(dst[i].c_str(),"%d",&Sub1);
		}
		if (i==2)
		{
			sscanf(dst[i].c_str(),"%d",&Sub2);
		}
		if (i==3)
		{
			sscanf(dst[i].c_str(),"%d",&Sub3);
		}
	}
	return true;
}

bool CSysHelper::StringSplit(std::string &src, const std::string& splitter, std::vector<std::string> &dst)//分隔符splitter的字符串解析出来子串到vector中
{
	std::string::size_type pos		= 0;
	std::string::size_type next		= pos;
	while ( true )
	{
		string tmp;
		next = src.find(splitter, pos);
		if ( next==std::string::npos )
		{
			tmp = src.substr(pos, src.size()-pos);
			dst.push_back(tmp);
			break;
		}
		else
		{
			tmp = src.substr(pos, next-pos);
			dst.push_back(tmp);
			pos = next+splitter.size();
		}
	}

	return true;
}

//替换字符串中指定子串
std::string& CSysHelper::replace_all_distinct(std::string& str,const std::string& old_value,const std::string& new_value)     
{     
	for(std::string::size_type pos(0);pos!=string::npos;pos+=new_value.length()) 
	{     
		if((pos=str.find(old_value,pos))!=string::npos)     
			str.replace(pos,old_value.length(),new_value);     
		else   
			break;     
	}     
	return str;     
}

#if (!defined WIN32)
typedef struct MEM_OCCUPY         
{  
	char name1[20];       
	unsigned long MemTotal;  
	char name2[20];  
	unsigned long MemFree;  
	char name3[20];  
	unsigned long Buffers;  
	char name4[20];  
	unsigned long Cached;  
}; 

typedef struct os_line_data
{
	char * val;
	int    len;
} os_line_data;

static char * os_getline(char *sin, os_line_data * line, char delim)
{
	char *out = sin;
	if (*out == '\0') return NULL;
	//	while (*out && (*out == delim)) { out++; }
	line->val = out;
	while (*out && (*out != delim)) { out++; }
	line->len = out - line->val;
	//	while (*out && (*out == delim)) { out++; }
	if (*out && (*out == delim)) { out++; }
	if (*out == '\0') return NULL;
	return out;
}
int Parser_EnvInfo(char * buffer,int size ,MEM_OCCUPY * lpMemory)
{
	int    state = 0;
	char * p     = buffer;
	while (p)
	{
		os_line_data       line = { 0 };
		p = os_getline(p, &line, ':');
		if (p == NULL || line.len <= 0) continue;

		if (line.len == 8&& strncmp(line.val, "MemTotal", 8) == 0)
		{	
			char *point = strtok(p," ");
			memcpy(lpMemory->name1,"MemTotal",8);
			lpMemory->MemTotal = atol(point);

		}
		else if(line.len == 7&& strncmp(line.val, "MemFree", 7) == 0)
		{
			char *point = strtok(p," ");
			memcpy(lpMemory->name2,"MemFree",7);
			lpMemory->MemFree = atol(point);
		}
		else if(line.len == 7&& strncmp(line.val, "Buffers", 7) == 0)
		{
			char *point = strtok(p," ");
			memcpy(lpMemory->name3,"Buffers",7);
			lpMemory->Buffers = atol(point);
		}
		else if(line.len == 6&& strncmp(line.val, "Cached", 6) == 0)
		{
			char *point = strtok(p," ");
			memcpy(lpMemory->name4,"Cached",6);
			lpMemory->Cached = atol(point);
		}

	} 	
}
#endif

//获取机器内存总大小,单位MB
tuint64 CSysHelper::GetTotalMemorySize()
{
#ifdef WIN32
	MEMORYSTATUSEX statex;
	statex.dwLength= sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	return (tuint64)((statex.ullTotalPhys)/(1024*1024));
#elif defined __ENVIRONMENT_MAC__
	return 0;
#elif defined __ENVIRONMENT_LINUX_ANDROID__
    return 0;
#else
	FILE *fd;
	char buff[128]={0};
	fd = fopen("/proc/meminfo", "r"); 
	if(fd <0) return 0;
	fgets(buff, sizeof(buff), fd); 
	MEM_OCCUPY lpMemory;
	memset(&lpMemory,0,sizeof(MEM_OCCUPY));
	Parser_EnvInfo(buff,sizeof(buff),&lpMemory);
    tuint64 nTotalMemSize = (tuint64)((lpMemory.MemTotal)/(1024));
	fclose(fd);
	return nTotalMemSize;
#endif
}

//获取机器是否是ubuntu系统
bool CSysHelper::IsUbuntuOS()
{
	switch(m_nUbuntuOS)
	{
	case 0:
		{
#if (!defined WIN32) && (!defined __ENVIRONMENT_MAC__)
			bool bIsUbuntu = false;

			char sz_TimingTask[1024]={0};
			std::string cmd = "lsb_release -a";
			const char* sysCommamd = cmd.data();

			FILE *fp;
			if((fp = popen(sysCommamd, "r")) != NULL)
			{
				if(fgets(sz_TimingTask, sizeof(sz_TimingTask) -1 ,fp) != NULL)//only first line
				{
					string strTimingTask(sz_TimingTask);
					//NORMAL_OUTPUT("IsUbuntuOS The timing task is %s", strTimingTask.c_str());
					int nPos = 0;
					nPos = strTimingTask.find("Ubuntu");
					if(nPos > 0)
					{
						bIsUbuntu = true;
					}
					pclose(fp);
				}
				else
				{
					//NORMAL_OUTPUT("IsUbuntuOS No timing task!");
					pclose(fp);
				}
			}
			else
			{
				//NORMAL_OUTPUT("IsUbuntuOS Execute \"%s\" default!", cmd.c_str());
			}
			if (bIsUbuntu)
			{
				m_nUbuntuOS = 1;
			}
			else
			{
				m_nUbuntuOS = 2;
			}
			return bIsUbuntu;
#else
			m_nUbuntuOS = 2;
			return false;
#endif
		}
		break;;
	case 1:
		{
			return true;
		}
		break;
	case 2:
		{
			return false;
		}
		break;
	default:
		{
			return false;
		}
		break;
	}
	return false;
}

//取出所有非虚拟网卡的mac地址
string CSysHelper::GetNonVirtualMac()
{
	string strMAC;
	std::list<CPFHelper::TNICInfo> NICList;
	CPFHelper::GetNICList(NICList); //临时记录列表
	std::list<CPFHelper::TNICInfo>::iterator itNIC ;
	for (itNIC = NICList.begin();itNIC!=NICList.end();itNIC++)
	{
		char  szMACAddr[24] = {0};
		memset(szMACAddr, 0, sizeof(szMACAddr));
		CSysHelper::MACAddressToStr(itNIC->MAC,szMACAddr,sizeof(szMACAddr));
		if (CSysHelper::IsVirtualMac(szMACAddr) == false)
		{
			strMAC.append(szMACAddr);
			strMAC.append(",");
		}
	}
	if(strMAC.size() > 2)
	{
		//去掉最后的逗号
		string strTempMac = strMAC.substr(strMAC.size()-1,1);
		if (strcmp(strTempMac.c_str(),",") == 0)
		{
			strMAC = strMAC.substr(0,strMAC.size() -1);
		}
	}
	return strMAC;
}

bool CSysHelper::encryptionIp(const std::string& strIp,std::string& strEncryptIp)
{
#ifdef DESENSITIZATION
	char szIpEncryption[32] = {0};
	tuint32 n1=0, n2=0, n3=0, n4=0;
	int n = sscanf(strIp.c_str(), "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
	sprintf(szIpEncryption,"%d.%d.%d.*",n1,n2,n3);

	strEncryptIp = szIpEncryption;
	if (n == 4)
	{
		return true;
	}
#else
	strEncryptIp = strIp;
	return true;
#endif
	return false;
}

//摄氏度转成华氏度
double CSysHelper::temperatureTotemperatureF(const double temperature)
{
	return temperature*9.0/5.0+32.0;
}
//华氏度转成摄氏度
double CSysHelper::temperatureFTotemperature(const double temperature)
{
	return (temperature - 32.0)*5.0/9.0;
}

//获取linux机器的出厂产品型号
std::string CSysHelper::GetLinuxDevProduct()
{
    if(m_strLinuxDevProduct.size()<=0)
    {
        ReadLinuxDevAppInfo();
    }
	return m_strLinuxDevProduct.c_str();
}

//获取linux机器的定制产品型号
std::string CSysHelper::GetLinuxDevCustomerModel()
{
    if(m_strLinuxDevCustomerModel.size()<=0)
    {
        ReadLinuxDevAppInfo();
    }
    return m_strLinuxDevCustomerModel.c_str();
}

void CSysHelper::ReadLinuxDevAppInfo()
{
#if (!defined WIN32) && (!defined __ENVIRONMENT_LINUX_ANDROID__) && (!defined __ENVIRONMENT_MAC__)
    FILE *fp;
    if((fp = fopen("/opt/NVMS/AppPackInfo.ini", "r")) != NULL)
    {
        char szTemp[512]={0};
        std::string strProduct = "Product=";
        std::string strCustomerModel = "CustomerModel=";
        while(fgets(szTemp, sizeof(szTemp) -1 ,fp) != NULL)//only first line
        {
            string strTemp(szTemp);
            int nPos1 = -1,nPos2 = -1;
            nPos1 = strTemp.find(strProduct.c_str(),0);
            nPos2 = strTemp.find(strCustomerModel.c_str(),0);
            if(nPos1>=0)
            {
                m_strLinuxDevProduct = strTemp.substr(strProduct.size(),strTemp.size()-strProduct.size()-1);
            }
            if(nPos2>=0)
            {
                m_strLinuxDevCustomerModel = strTemp.substr(strCustomerModel.size(),strTemp.size()-strCustomerModel.size()-1);
            }

            memset(szTemp,0,512);
            strTemp.clear();
        }
        fclose(fp);
    }
#endif
}

std::string CSysHelper::GetConfigPath()
{
    char   dirSeptr[4] = {'\0'};
    char   exeDir[256] = {0};
    CPFHelper::GetModuleFileDir(NULL, exeDir, sizeof(exeDir));
    sprintf(dirSeptr, "%c", CPFHelper::DirectorySeparator);

    std::string strTmpDataPath(exeDir);
    strTmpDataPath += "configInfo";
#ifdef WIN32
    ::CreateDirectoryA(strTmpDataPath.c_str(), NULL);
#else
    mkdir(strTmpDataPath.c_str(), 0777);
#endif

    strTmpDataPath += dirSeptr;
    strTmpDataPath += "Config";
#ifdef WIN32
    ::CreateDirectoryA(strTmpDataPath.c_str(), NULL);
#else
    mkdir(strTmpDataPath.c_str(), 0777);
#endif
    strTmpDataPath += dirSeptr;

    return strTmpDataPath;
}
