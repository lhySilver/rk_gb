// HttpSocket.cpp: implementation of the CHttpSocket class.
//
//////////////////////////////////////////////////////////////////////
#include "base_type_define.h"
#include "HttpProtocolAnalyse.h"
#include "MySingleLock.h"
#include <sstream>
#include "TVTXMLParse.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Safe_Del_ptrArry(x) if(x){ delete []x; x = 0;}

CHttpProtocolAnalyse::CHttpProtocolAnalyse(void)
{

}

CHttpProtocolAnalyse::CHttpProtocolAnalyse(const std::string &strHttp)
{
	AnalyzeHttpPack(strHttp.c_str(), strHttp.length());
}

CHttpProtocolAnalyse::CHttpProtocolAnalyse(const char *szBuff, const int nBuffLen)
{
	AnalyzeHttpPack(szBuff, nBuffLen);
}

CHttpProtocolAnalyse::~CHttpProtocolAnalyse(void)
{
}

CHttpProtocolAnalyse &CHttpProtocolAnalyse::operator=(const CHttpProtocolAnalyse &other)
{
	if (this != &other)
	{
		this->m_HttpProtocol = other.m_HttpProtocol;
	}

	return *this;
}

bool CHttpProtocolAnalyse::IsRequestCmd(void) const
{
	if (m_HttpProtocol.m_strURL == "")	// 如果URL为空，则请求主页面，如：index.html.
	{
		return false;
	}

	int nFirst = m_HttpProtocol.m_strURL.find('.');
	if (nFirst >= 0)	// 在url中查找'.'，如果存在则请求文件，否则url请求的是命令
	{
		int npos = m_HttpProtocol.m_strURL.find(".shtml");
		if(npos >= 0){//针对请求shtml文件当作命令处理（wifi探针）
			return true;
		}
		return false;
	}

	return true;
}

bool CHttpProtocolAnalyse::IsRequestFile(void) const 
{
	return (!IsRequestCmd());
}

bool CHttpProtocolAnalyse::IsResponse(void) const 
{
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);

	int nFirst = strHeader.find("HTTP");
	if (nFirst == 0)	// 如果HTTP出现在开始位置，则为响应包
	{
		return true;
	}

	return false;
}

void CHttpProtocolAnalyse::SetURL(const std::string &strURL)
{
	m_HttpProtocol.m_strURL = strURL;
}

const char* CHttpProtocolAnalyse::GetUrl(void) const
{
	return m_HttpProtocol.m_strURL.c_str();
}

// 获取method
const std::string CHttpProtocolAnalyse::GetMethod(void) const
{
	std::string strMethod;
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	int nFirst = strHeader.find(" ");
	if (nFirst > 0)	// 如果HTTP出现在开始位置，则为响应包
	{
		strMethod = strHeader.substr(0,nFirst);		
	}
	return strMethod.c_str();
}

//检查是否含有校验字段
bool CHttpProtocolAnalyse::CheckAuthorization(void) const
{
	// 解析校验字段
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	tint32 nFirstPos = -1;
	nFirstPos = strHeader.find(HTTP_DEFINE::CHECK_AUTHORIZATION);
	if (nFirstPos > 0)
	{
		return true;
	}
	return false;
}

bool CHttpProtocolAnalyse::GetAuthiruzation(std::string& strAuthor) const
{
    if (CheckAuthorization())
    {
        std::string strHeader;
        GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
        tint32 nFirstPos = -1;
        nFirstPos = strHeader.find(HTTP_DEFINE::CHECK_AUTHORIZATION);
        if (nFirstPos > 0)
        {
            std::string  strAuth = strHeader.substr(nFirstPos);
            tint32 nfirstPos = -1;
            nfirstPos = strAuth.find("Basic");
            if(nfirstPos > 0)
            {
                strAuth = strAuth.substr(nfirstPos);
                nfirstPos = strAuth.find(" ");
                strAuth = strAuth.substr(nfirstPos);
                nfirstPos = strAuth.find("\r\n");
                strAuthor = strAuth.substr(1, nfirstPos);
                return true;
            }
        }
    }
    return false;
}

void CHttpProtocolAnalyse::SetHTTPConfig(const std::string &strCMD, const char *pData, const tuint32 nLen
	, const std::string &strVerify/*=""*/)
{
	stringstream strData;
	strData<<nLen;
	std::string strHeader =  "GET /" + strCMD + " HTTP/1.1\r\n" 
		+ HTTP_DEFINE::AUTHORIZATION_FILED + strVerify + "\r\n"
		+ "Content-type: text/xml" + "\r\n"
		+ HTTP_DEFINE::CONTENT_LENGTH + strData.str()
		+ HTTP_DEFINE::DOUBLE_ENTER_LINE;

	m_HttpProtocol.m_nDataLen = strHeader.length() + nLen;
	m_HttpProtocol.m_nPayLoadLen = nLen;

	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);
	m_HttpProtocol.m_pBuff = new char[m_HttpProtocol.m_nDataLen+1];
	memset(m_HttpProtocol.m_pBuff, 0, m_HttpProtocol.m_nDataLen+1);
	memcpy(m_HttpProtocol.m_pBuff, strHeader.c_str(), strHeader.length());

	m_HttpProtocol.m_strURL = strCMD;

	if (pData != 0 && nLen > 0)
	{
		memcpy(m_HttpProtocol.m_pBuff+strHeader.length(), pData, nLen);
	}
}

const char* CHttpProtocolAnalyse::GetPayLoad(tint32 &nLen) const
{
	nLen = 0;
	if (m_HttpProtocol.m_pBuff == 0 || m_HttpProtocol.m_nPayLoadLen <= 0)
	{
		return "";	// 请求命令时可能不携带负载数据，此时返回负载数据为空，不能返回空指针，否则会出错。
	}

	nLen = m_HttpProtocol.m_nPayLoadLen;
	int nHeader = m_HttpProtocol.m_nDataLen - m_HttpProtocol.m_nPayLoadLen;
	return (m_HttpProtocol.m_pBuff + nHeader);
}

HTTP_DEFINE::HttpStatusCode CHttpProtocolAnalyse::GetStatusCode(void)const
{
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	if (strHeader.length() <= 0)
	{
		return HTTP_DEFINE::HTTP_BAD_REQUEST;
	}
	std::string strStatusCode = strHeader.substr(9,3);

	HTTP_DEFINE::HttpStatusCode nStatusCode = (HTTP_DEFINE::HttpStatusCode)atoi(strStatusCode.c_str());
	return nStatusCode;
}

bool CHttpProtocolAnalyse::SetResPackage(HTTP_DEFINE::HttpStatusCode nStatusCode, const char *szBuf, int nLen
	, const std::string &strAuthorization, std::string nonStanadardHeader)
{
	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);

	m_HttpProtocol.m_pBuff = new char[HTTP_DEFINE::HTTP_HEADER_LEN+nLen+1];
	memset(m_HttpProtocol.m_pBuff, 0, HTTP_DEFINE::HTTP_HEADER_LEN+nLen+1);

	int nHeaderLen = CreateHttpHeader(nStatusCode, m_HttpProtocol.m_pBuff, nLen, strAuthorization, nonStanadardHeader); // 创建HTTP响应头
	m_HttpProtocol.m_nDataLen = nHeaderLen + nLen;	// 返回的长度包括头部和负载两部分

	if (m_HttpProtocol.m_pBuff != 0)
	{
		if (szBuf != 0 && nLen > 0)
		{
			// 把传入的负载数据写到HTTP头部后面
			m_HttpProtocol.m_nPayLoadLen = nLen;
			memcpy(m_HttpProtocol.m_pBuff + nHeaderLen, szBuf, nLen);
		}
		return true;
	}
	return false;
}

bool CHttpProtocolAnalyse::SetResPackage_downLoadFile(HTTP_DEFINE::HttpStatusCode nStatusCode, const char *szBuf, int nLen,const string& fileName)
{
    Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);

    m_HttpProtocol.m_pBuff = new char[HTTP_DEFINE::HTTP_HEADER_LEN+nLen+1];
    memset(m_HttpProtocol.m_pBuff, 0, HTTP_DEFINE::HTTP_HEADER_LEN+nLen+1);

    int nHeaderLen = CreateHttpHeader_downLoadFile(nStatusCode, m_HttpProtocol.m_pBuff, nLen, fileName); // 创建HTTP响应头
    m_HttpProtocol.m_nDataLen = nHeaderLen + nLen;	// 返回的长度包括头部和负载两部分

    if (m_HttpProtocol.m_pBuff != 0)
    {
        if (szBuf != 0 && nLen > 0)
        {
            // 把传入的负载数据写到HTTP头部后面
            m_HttpProtocol.m_nPayLoadLen = nLen;
            memcpy(m_HttpProtocol.m_pBuff + nHeaderLen, szBuf, nLen);
        }
        return true;
    }
    return false;
}

// 设置响应数据包
bool CHttpProtocolAnalyse::SetResPackage_1400(HTTP_DEFINE::HttpStatusCode nStatusCode, const char *szBuf, int nLen)
{
	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);

	m_HttpProtocol.m_pBuff = new char[HTTP_DEFINE::HTTP_HEADER_LEN+nLen+1];
	memset(m_HttpProtocol.m_pBuff, 0, HTTP_DEFINE::HTTP_HEADER_LEN+nLen+1);

	int nHeaderLen = CreateHttpHeader_1400(nStatusCode, m_HttpProtocol.m_pBuff, nLen); // 创建HTTP响应头
	m_HttpProtocol.m_nDataLen = nHeaderLen + nLen;	// 返回的长度包括头部和负载两部分

	if (m_HttpProtocol.m_pBuff != 0)
	{
		if (szBuf != 0 && nLen > 0)
		{
			// 把传入的负载数据写到HTTP头部后面
			m_HttpProtocol.m_nPayLoadLen = nLen;
			memcpy(m_HttpProtocol.m_pBuff + nHeaderLen, szBuf, nLen);
		}

		return true;
	}

	return false;
}

const char *CHttpProtocolAnalyse::GetBuf(tint32 &nLen) const
{
	nLen = 0;
	if (m_HttpProtocol.m_pBuff != 0 && m_HttpProtocol.m_nDataLen > 0)
	{
		nLen = m_HttpProtocol.m_nDataLen;
		return m_HttpProtocol.m_pBuff;
	}

	return "";
}

void CHttpProtocolAnalyse::AnalyzeHttpPack(const char *szBuff, const int nBuffLen)
{
	if (szBuff == 0 || nBuffLen <= 0)
	{
		return;
	}

	std::string strHeader;
	tuint32 nHeaderLen = GetHttpHeader(szBuff, strHeader);

	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);
	m_HttpProtocol.m_nDataLen = nBuffLen;
	m_HttpProtocol.m_nPayLoadLen = nBuffLen - nHeaderLen;

	m_HttpProtocol.m_pBuff = new char[m_HttpProtocol.m_nDataLen+1];
	memset(m_HttpProtocol.m_pBuff, 0, m_HttpProtocol.m_nDataLen+1);
	memcpy(m_HttpProtocol.m_pBuff, szBuff, nBuffLen);

	// 解析出URL	
	int nEnd = strHeader.find(" HTTP");
	if (nEnd != -1)
	{
		int nFirst = strHeader.find('/');
		m_HttpProtocol.m_strURL = strHeader.substr(nFirst+1, nEnd-nFirst-1);
		FilterURL();
	}
}

bool CHttpProtocolAnalyse::GetUrlParam(std::string& strParam)
{
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);

	std::string::size_type nEnd = strHeader.find(" HTTP");
	if (nEnd != std::string::npos) {
		std::string::size_type nFirst = strHeader.find('?');
		if (nFirst != std::string::npos) {
			strParam = strHeader.substr(nFirst+1, nEnd-nFirst-1);
			strParam = URLDecode(strParam.c_str());
		}
	}
	return true;
}

tuint32 CHttpProtocolAnalyse::GetHttpHeader(const char *szBuff, std::string &strHeader) const
{
	strHeader.clear();
	if (szBuff == 0)
	{
		ASSERT(false);
		return 0;
	}

	// 查找HTTP头结束标志
	const char *ptmp = strstr(szBuff, HTTP_DEFINE::DOUBLE_ENTER_LINE);
	if (ptmp)
	{
		int nHeaderLen = (ptmp - szBuff) + strlen(HTTP_DEFINE::DOUBLE_ENTER_LINE);
		strHeader.assign(szBuff, nHeaderLen);
	}

	return strHeader.length();
}

int CHttpProtocolAnalyse::CreateHttpHeader(const HTTP_DEFINE::HttpStatusCode nStatusCode, char *pBuff, const int nDataLen
	, const std::string &strAuthorization/*=""*/, const string& nonStanadardHeader)
{
	if (pBuff == 0)
	{
		ASSERT(false);
		return 0;
	}

	map<int, HTTP_DEFINE::HTTP_STATUS_INFO>::const_iterator strIter = HTTP_DEFINE::g_HTTPStatusMap.find(nStatusCode);
	ASSERT(strIter != HTTP_DEFINE::g_HTTPStatusMap.end());

	int nLen = 0;								// HTTP头部长度
	const string strMimeType = GetMimeType();	// 获取Content-type

	// 默认返回"text/html"类型
	string stResponserMimeType = (nStatusCode == HTTP_DEFINE::HTTP_OK) ? strMimeType : "text/html";
    if(  m_HttpProtocol.m_strURL.find("queryQRInfo") != std::string::npos )
    {
        stResponserMimeType = "image/png";
    }

	// 填充HTTP头部信息
	/*if (nStatusCode == HTTP_DEFINE::HTTP_UNAUTHORIZED)		// 该判断用于浏览器弹出默认校验框，
	{
		nLen = sprintf(pBuff
			, "HTTP/1.1 %d %s\r\nContent-type: %s\r\nContent-Length: %d\r\nConnection: close\r\nWWW-Authenticate: Basic realm=\"NVMS1000_SERVICE\"\r\n"
			, nStatusCode, strIter->second.strStatusKey.c_str()
			, stResponserMimeType.c_str(), nDataLen);
	}
	else*/
	//{
	//	nLen = sprintf(pBuff
	//		, "HTTP/1.1 %d %s\r\nContent-type: %s\r\nContent-Length: %d\r\nConnection: close\r\nAuthInfo: %s\r\n"
	//		, nStatusCode, strIter->second.strStatusKey.c_str()
	//		, stResponserMimeType.c_str(), nDataLen, strAuthorization.c_str());
	//}

#ifdef VIXHZ_SDK_EXPORTS
	nLen = sprintf(pBuff
		, "HTTP/1.1 %d %s\r\nContent-Length: %d\r\nContent-type: %s\r\nConnection: keep-alive\r\n%s\r\n"
		, nStatusCode, strIter->second.strStatusKey.c_str()
		, nDataLen, stResponserMimeType.c_str()
		, nonStanadardHeader);
	strcat(pBuff, "\r\n");
	nLen += 2;
	return nLen;

#else
	if (strAuthorization.empty()) {
		nLen = sprintf(pBuff
			, "HTTP/1.1 %d %s\r\nContent-type: %s\r\nContent-Length: %d\r\nConnection: close\r\n"
			, nStatusCode, strIter->second.strStatusKey.c_str()
			, stResponserMimeType.c_str(), nDataLen);
		strcat(pBuff, "\r\n");	//头部结束标志
		nLen += 2;
	}
	else {
		nLen = sprintf(pBuff
			, "HTTP/1.1 %d %s\r\nContent-type: %s\r\nContent-Length: %d\r\nConnection: close\r\nAuthInfo: %s\r\n"
			, nStatusCode, strIter->second.strStatusKey.c_str()
			, stResponserMimeType.c_str(), nDataLen, strAuthorization.c_str());
		strcat(pBuff, "\r\n");	//头部结束标志
		nLen += 2;
	}
	return nLen;
#endif
 // VIXHZ_SDK_EXPORTS


	strcat(pBuff, "\r\n");	//头部结束标志
	nLen += 2;

	return nLen;
}

int CHttpProtocolAnalyse::CreateHttpHeader_1400(const HTTP_DEFINE::HttpStatusCode nStatusCode
	, char *pBuff, const int nDataLen)						// 创建GB1400协议http包头
{
    if (pBuff == 0)
    {
        ASSERT(false);
        return 0;
    }
    int nLen = 0;	
    map<int, HTTP_DEFINE::HTTP_STATUS_INFO>::const_iterator strIter = HTTP_DEFINE::g_HTTPStatusMap.find(nStatusCode);
    ASSERT(strIter != HTTP_DEFINE::g_HTTPStatusMap.end());
    if (nStatusCode == HTTP_DEFINE::HTTP_OK)
    {
        nLen = sprintf(pBuff,
            "HTTP/1.1 %d %s\r\n"
            "Server: GAT1400/1.0\r\n"     
            "Content-Type: application/json;charset=UTF-8\r\n"
            "Content-Length: %d\r\n"
            "Connection:close\r\n"
            "\r\n",nStatusCode,strIter->second.strStatusKey.c_str(),nDataLen);

    }
    else
    {
        nLen = sprintf(pBuff,
            "HTTP/1.1 %d %s\r\n"
            "WWW-Authenticate: Basic realm=\"Basic\"\r\n"
            "Server: GAT1400/1.0\r\n"     
            "Content-Type: application/json;charset=UTF-8\r\n"
            "Content-Length: %d\r\n"
            "Connection:close\r\n"
            "\r\n",nStatusCode,strIter->second.strStatusKey.c_str(),nDataLen);
    }

    //strcat(pBuff, "\r\n");	//头部结束标志
    //nLen += 2;
    return nLen;
}

int CHttpProtocolAnalyse::CreateHttpHeader_downLoadFile(const HTTP_DEFINE::HttpStatusCode nStatusCode
    , char *pBuff, const int nDataLen,const string& strFileName)
{
    if (pBuff == 0)
    {
        ASSERT(false);
        return 0;
    }
    int nLen = 0;
    map<int, HTTP_DEFINE::HTTP_STATUS_INFO>::const_iterator strIter = HTTP_DEFINE::g_HTTPStatusMap.find(nStatusCode);
    ASSERT(strIter != HTTP_DEFINE::g_HTTPStatusMap.end());
    if (nStatusCode == HTTP_DEFINE::HTTP_OK)
    {
        nLen = sprintf(pBuff,
            "HTTP/1.1 %d %s\r\n"
            "Content-Type: application/octet-stream;charset=UTF-8\r\n"
            "Content-Disposition: attachment; filename=%s\r\n"
            "Content-Length: %d\r\n"
            "Connection:close\r\n"
            "\r\n",nStatusCode,strIter->second.strStatusKey.c_str(),strFileName.c_str(),nDataLen);

    }
    else
    {
        nLen = sprintf(pBuff,
            "HTTP/1.1 %d %s\r\n"
            "Content-Type: application/octet-stream;charset=UTF-8\r\n"
            "Content-Disposition: attachment; filename=%s\r\n"
            "Content-Length: %d\r\n"
            "Connection:close\r\n"
            "\r\n",nStatusCode,strIter->second.strStatusKey.c_str(),strFileName.c_str(),nDataLen);
    }

    return nLen;
}


void CHttpProtocolAnalyse::SetResponseHeader(const HTTP_DEFINE::HttpStatusCode nStatusCode)
{
	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);
	m_HttpProtocol.m_nPayLoadLen = 0;
	m_HttpProtocol.m_pBuff = new char[HTTP_DEFINE::HTTP_HEADER_LEN+1];
	memset(m_HttpProtocol.m_pBuff, 0, HTTP_DEFINE::HTTP_HEADER_LEN+1);

	m_HttpProtocol.m_nDataLen = CreateHttpHeader(nStatusCode, m_HttpProtocol.m_pBuff, 0);
}


bool CHttpProtocolAnalyse::GetVerifyInfo(std::string &strVerify) const
{
	// 解析校验字段
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	tint32 nFirstPos = strHeader.find(HTTP_DEFINE::AUTHORIZATION_FILED);
	if (nFirstPos < 0)
	{
		return false;
	}

	nFirstPos += strlen(HTTP_DEFINE::AUTHORIZATION_FILED);
	tint32 nEndPos = strHeader.find("\r\n", nFirstPos);
	strVerify = strHeader.substr(nFirstPos, nEndPos-nFirstPos);

	return true;
}

bool CHttpProtocolAnalyse::GetSessionInfo(string &strSession) const
{
    std::string strHeader;
    GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
    tint32 nFirstPos = strHeader.find(HTTP_DEFINE::SESSION);
    if(nFirstPos < 0)
    {
        return false;
    }

    nFirstPos += strlen(HTTP_DEFINE::SESSION);
    tint32 nEndPos = strHeader.find("\r\n", nFirstPos);
    strSession = strHeader.substr(nFirstPos, nEndPos - nFirstPos);

    return true;
}

bool CHttpProtocolAnalyse::GetCertPassword(string &key) const
{
	std::string strHeader;
	GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	tint32 nFirstPos = strHeader.find("key: ");
	if (nFirstPos < 0)
	{
		return false;
	}

	nFirstPos += strlen("key: ");
	tint32 nEndPos = strHeader.find("\r\n", nFirstPos);
	key = strHeader.substr(nFirstPos, nEndPos - nFirstPos);
	return true;
}


bool CHttpProtocolAnalyse::SetHeaderExtra(const char *pExtraKey, tuint32 dwParameter)
{
	std::string strHeader;
	tint32 nHeaderLen = GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	if (nHeaderLen == 0)
	{
		ASSERT(false);
		return false;
	}

	stringstream sstream;
	sstream<<dwParameter; 
	strHeader.erase(strHeader.find_last_not_of(HTTP_DEFINE::DOUBLE_ENTER_LINE)+1);
	strHeader += std::string("\r\n") + pExtraKey + std::string(" ") + sstream.str() + HTTP_DEFINE::DOUBLE_ENTER_LINE;

	tint32 nPayLoadLen = 0;
	const char *pPayLoad = GetPayLoad(nPayLoadLen);
	char *pBuff = 0;
	if (nPayLoadLen > 0)
	{
		pBuff = new char[nPayLoadLen+1];
		memset(pBuff, 0, nPayLoadLen+1);
		memcpy(pBuff, pPayLoad, nPayLoadLen);
	}

	m_HttpProtocol.m_nDataLen = strHeader.length() + nPayLoadLen;
	m_HttpProtocol.m_nPayLoadLen = nPayLoadLen;

	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);
	m_HttpProtocol.m_pBuff = new char[m_HttpProtocol.m_nDataLen+1];
	memset(m_HttpProtocol.m_pBuff, 0, m_HttpProtocol.m_nDataLen+1);
	memcpy(m_HttpProtocol.m_pBuff, strHeader.c_str(), strHeader.length());

	if (pBuff != 0)
	{
		memcpy(m_HttpProtocol.m_pBuff+strHeader.length(), pBuff, nPayLoadLen);
		delete []pBuff;
	}

	return true;
}


bool CHttpProtocolAnalyse::SetHeaderLine(const char* pLine)
{
	std::string strHeader;
	tint32 nHeaderLen = GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	if (nHeaderLen == 0)
	{
		ASSERT(false);
		return false;
	}

	strHeader.erase(strHeader.find_last_not_of(HTTP_DEFINE::DOUBLE_ENTER_LINE)+1);
	strHeader += std::string("\r\n") + pLine  + HTTP_DEFINE::DOUBLE_ENTER_LINE;

	tint32 nPayLoadLen = 0;
	const char *pPayLoad = GetPayLoad(nPayLoadLen);
	char *pBuff = 0;
	if (nPayLoadLen > 0)
	{
		pBuff = new char[nPayLoadLen+1];
		memset(pBuff, 0, nPayLoadLen+1);
		memcpy(pBuff, pPayLoad, nPayLoadLen);
	}

	m_HttpProtocol.m_nDataLen = strHeader.length() + nPayLoadLen;
	m_HttpProtocol.m_nPayLoadLen = nPayLoadLen;

	Safe_Del_ptrArry(m_HttpProtocol.m_pBuff);
	m_HttpProtocol.m_pBuff = new char[m_HttpProtocol.m_nDataLen+1];
	memset(m_HttpProtocol.m_pBuff, 0, m_HttpProtocol.m_nDataLen+1);
	memcpy(m_HttpProtocol.m_pBuff, strHeader.c_str(), strHeader.length());

	if (pBuff != 0)
	{
		memcpy(m_HttpProtocol.m_pBuff+strHeader.length(), pBuff, nPayLoadLen);
		delete []pBuff;
	}

	return true;

}
bool CHttpProtocolAnalyse::GetHeaderExtra(const char *pExtraKey, tuint32 &dwParameter)const
{
	dwParameter = 0;
	if (pExtraKey == 0)
	{
		ASSERT(false);
		return false;
	}

	std::string strHeader;
	tint32 nHeaderLen = GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
	if (nHeaderLen == 0)
	{
		return false;
	}

	std::string strExtraKey = pExtraKey + std::string(" ");
	tint32 nFirstPos = strHeader.find(strExtraKey);
	if (nFirstPos == -1)
	{
		return false;
	}

	nFirstPos += strExtraKey.length();
	tint32 nEndPos = strHeader.find("\r\n", nFirstPos);
	if (nEndPos != -1 )
	{
		sscanf(strHeader.substr(nFirstPos, nEndPos-nFirstPos).c_str(), "%u",&dwParameter);
		return true;
	}

	return false;
}


std::string CHttpProtocolAnalyse::URLDecode(const char *sIn)
{
	std::string sOut;
	int len = strlen(sIn);

	for( int ix = 0; ix < len; ix++ )
	{
		char ch = 0;
		if(sIn[ix]=='%')
		{
			ch = (fromHex(sIn[ix+1])<<4);
			ch |= fromHex(sIn[ix+2]);
			ix += 2;
		}
		else if(sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}

char CHttpProtocolAnalyse::fromHex(const char &x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}

void CHttpProtocolAnalyse::FilterURL(void)
{
    // 解码URL(防止一些特殊字符的转义)
    m_HttpProtocol.m_strURL = URLDecode(m_HttpProtocol.m_strURL.c_str());

    // 去掉URL中?后面的参数，该参数目前没有用到，参数主要用来解决Web缓存问题
    int nFirst = m_HttpProtocol.m_strURL.find('?');
    if (nFirst >= 0)
    {
        if( m_HttpProtocol.m_strURL.find("queryQRInfo") == std::string::npos )
            m_HttpProtocol.m_strURL = m_HttpProtocol.m_strURL.substr(0, nFirst);
    }
}

const string CHttpProtocolAnalyse::GetMimeType(void)
{
	string strSuffix("");

	// 如果请求的是命令
	if (IsRequestCmd())
	{
		strSuffix = ".xml";
	}
	else
	{
		int nPos = m_HttpProtocol.m_strURL.find_last_of(".");
		if (nPos >= 0)
		{
			strSuffix = m_HttpProtocol.m_strURL.substr(nPos, m_HttpProtocol.m_strURL.length());
		}
		else
		{
			strSuffix = ".html";
		}
	}
	
	std::map<string, string>::const_iterator typeIter = HTTP_DEFINE::g_ContentTypeMap.begin();
	for (; typeIter!=HTTP_DEFINE::g_ContentTypeMap.end(); typeIter++)
	{
		int nPos = typeIter->first.find(strSuffix);
		if (nPos >= 0)
		{
			break;
		}
	}

	if (typeIter != HTTP_DEFINE::g_ContentTypeMap.end())
	{
		return typeIter->second;
	}
	
	return "";
}
std::string CHttpProtocolAnalyse::URLEncode(const char *sIn) const
{
	std::string sOut;
	int len = strlen(sIn);

	for( int ix = 0; ix < len; ix++ )
	{
		if ( ('0' <= sIn[ix] && sIn[ix] <= '9') || ('a' <= sIn[ix] && sIn[ix] <= 'z') || ('A' <= sIn[ix] && sIn[ix] <= 'Z') || sIn[ix] == '/' || sIn[ix] == 'c')
		{
			sOut += sIn[ix];
		}
		else
		{
			short c = (short)sIn[ix];
			if (c < 0)
			{
				c += 256;
			}
			short c1 = c/16;
			short c2 = c%16;
			sOut += '%';
			sOut += fromDec(c1);
			sOut += fromDec(c2);
		}
	}
	return sOut;
}

char CHttpProtocolAnalyse::fromDec(const short &x) const
{
	if (0 <= x && x <= 9)
	{
		return char(short('0')+x);
	}
	else if (10 <= x && x <= 15)
	{
		return char(short('A')+x-10);
	}

	return char(0);
}


bool CHttpProtocolAnalyse::GetSessionID(std::string &strSessionID)  const 
{
	std::string strBuff(m_HttpProtocol.m_pBuff);
	/*
	 *从http报文中获取session id
	*/
	std::size_t IDBegin = strBuff.find("sessionId=");
	if (IDBegin != string::npos)
	{

		if (strBuff.size() < IDBegin + 10 + 36) {
			return false;
		}
		//10为"sessionId="长度，36为ID长度，报文里面没有带“{}”,需要加上
		strSessionID = strBuff.substr(IDBegin + 10, 36);
		strSessionID = "{" + strSessionID;
		strSessionID = strSessionID + "}";
		return true;
	}

	return false;
}
bool CHttpProtocolAnalyse::CheckHttpHeaderType(HTTP_DEFINE::HTTP_MSG_TYPE_H& MsgTypeCode)const
{
    //string strHeader;
    //string strMessageType;
    //tuint32 lenHeader = GetHttpHeader(m_HttpProtocol.m_pBuff, strHeader);
    //tuint32 lenMsgType = strHeader.find(" ", 0);
    //strMessageType.assign(strHeader, 0, lenMsgType);
    //std::map<string, HTTP_DEFINE::HTTP_MSG_TYPE_H>::const_iterator itor = HTTP_DEFINE::g_HTTPMsgTypeMap.find(strMessageType);
    //if(itor == HTTP_DEFINE::g_HTTPMsgTypeMap.end())
    //{
    //    return false;
    //}
    //else
    //{
    //    MsgTypeCode = itor->second;
    //    return true;
    //}
    return true;
}



