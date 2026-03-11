
#ifndef __SYS_HELPER__
#define __SYS_HELPER__
using namespace std;
#include <map>
#include <string>
#include <vector>
#include "base_type_define.h"
class CSysHelper
{
public:
	///////////获取CPU的个数
	static int GetCPUCount();

	static bool CreateGUID(GUID & guid);

	static GUID GetGuidFromString(const char *pGUIDStr);

	static bool GuidToString(const GUID &guid,char *pGUIDStr);

	static std::string GuidToString(const GUID &guid);
	//guid脱敏处理
	static bool GuidToEncrypString(const GUID &guid,char *pGUIDStr);
	//guid脱敏处理
	static std::string GuidToEncrypString(const GUID &guid);

    static bool MACAddressToStr(const BYTE bMAC[6], char *pStrMac,int strLen);

    static bool StrToMACAddress(const char* strMAC, BYTE bMAC[6]);

    static bool IPAddressToStr(const tuint32 dwIP, char *pStrIP,int strLen);
    static bool StrToIPAddress(const char *strIP, tuint32 &dwIP);

	static bool ullToString(const tuint64 ullValue, char* strValue);////转换的字符串的长度为64
	static bool stringToUll(const char* strValue, tuint64 &ullValue);////如果字符串包含的字符除‘1’，‘0’以外还有其他字符就会转换失败

    static bool IsValidIP(const char *szIP);  //是否为合法的IPv4地址
    static bool IsValidSubnet(const char *szSubnet);  //是否为合法的子网掩码
    static tuint32 GetBuildDate(void);  //得到程序编译日期
    static void ParseBuildDate(tuint32 dwBuildDate, int &nYear, int &nMonth, int &nDay, int &nHour);

	//是否是虚拟网卡
	static bool IsVirtualMac(const char * mac);
	//取出所有非虚拟网卡的mac地址
	static string GetNonVirtualMac();
	//获取主机硬件标识(windows用的cpu+mac地址,linux用的cpu+硬盘)
	static std::string GetDecrInfo();
	//获取主机硬件标识(windows用的cpu+硬盘,linux用的cpu+硬盘) 由于网卡禁用后GetDecrInfo会变化成新的网卡，所以改成采用硬盘防止变化
	static std::string GetDecrInfoByDisk();
	//linux获取主机硬件标识(linux用的cpu+硬盘)
	static std::string Linux_GetDecrInfo();
	//字符串转换成大写
	static char* ToUpper(char *str);
	//获取cpu标识
	static std::string GetCpuID();
	//多字节编码字符串转UTF8
    static void MByteToUTF8( char *pMbyte, size_t mbyteLen, char *pUTF8, size_t utf8Len);
	//UTF8转多字节编码字符串
	static void UTF8ToMByte(const  char *pUTF8, size_t utf8Len,char *pMbyte, size_t mbyteLen);
	//删除strSrc中的strSub
	static std::string DelSubString( std::string strSrc, std::string strSub);

	static bool GetIntVersionFormStr(std::string &strVersion,tuint32 &Main,tuint32 &Sub1,tuint32 &Sub2,tuint32 &Sub3);

	static bool StringSplit(std::string &src, const std::string& splitter, std::vector<std::string> &dst);//分隔符splitter的字符串解析出来子串到vector中

	//替换字符串中指定子串
	static std::string& replace_all_distinct(std::string& str,const std::string& old_value,const std::string& new_value);  

	//获取机器内存总大小,单位MB
	static tuint64 GetTotalMemorySize();  
	//获取机器是否是ubuntu系统
	static bool IsUbuntuOS();
	//对IP进行加密
	static bool encryptionIp(const std::string& strIp,std::string& strEncryptIp);

	//摄氏度转成华氏度
	static double temperatureTotemperatureF(const double temperature);
	//华氏度转成摄氏度
	static double temperatureFTotemperature(const double temperature);

	//获取linux机器的出厂产品型号
	static std::string GetLinuxDevProduct();
	//获取linux机器的定制产品型号
	static std::string GetLinuxDevCustomerModel();
    static std::string GetConfigPath();

private:	
	/////构造
	CSysHelper(void);
	////析构
	~CSysHelper(void);	
	//定义是否是ubuntu系统，静态变量，避免重复调用
	static tuint32 m_nUbuntuOS;//0未知 1是 2不是
	static std::string m_strLinuxDevProduct;//出厂产品型号
	static std::string m_strLinuxDevCustomerModel;//出厂产品型号

    static void ReadLinuxDevAppInfo();
};

#endif //__SYS_HELPER__
