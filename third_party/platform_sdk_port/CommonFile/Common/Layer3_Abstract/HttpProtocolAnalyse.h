#ifndef HTTP_PROTOCOL_ANALYSE_HEAD
#define HTTP_PROTOCOL_ANALYSE_HEAD

#include "HttpDefine.h"
#include "Lock.h"
#include <list>
using namespace std;

class CHttpProtocolAnalyse
{
public:
	CHttpProtocolAnalyse(void);
	CHttpProtocolAnalyse(const std::string &strHttp);
	CHttpProtocolAnalyse(const char *szBuff, const int nBuffLen);

	~CHttpProtocolAnalyse(void);

	CHttpProtocolAnalyse &operator=(const CHttpProtocolAnalyse &other);

	// 设置配置包
	void SetHTTPConfig(const std::string &strCMD, const char *pData, const tuint32 nLen , const std::string &strVerify="");

	// 设置URL
	void SetURL(const std::string &strURL);

	// 获取URL，返回命令或请求的文件
	const char* GetUrl(void) const;
	// 获取method
	const std::string GetMethod(void) const;
	//检查是否含有校验字段
	bool CheckAuthorization(void) const;
    // 获取校验字段信息
    bool GetAuthiruzation(std::string&) const;
	// 获取HTTP负载数据
	const char* GetPayLoad(tint32 &nLen) const;

	// 设置响应数据包
	bool SetResPackage(HTTP_DEFINE::HttpStatusCode nStatusCode, const char *szBuf, int nLen, const std::string &strAuthorization="", std::string nonStanadardHeader="");

	// 设置响应数据包
	bool SetResPackage_1400(HTTP_DEFINE::HttpStatusCode nStatusCode, const char *szBuf, int nLen);

	// 设置HTTP响应头 
	void SetResponseHeader(const HTTP_DEFINE::HttpStatusCode nStatusCode);	

    bool SetResPackage_downLoadFile(HTTP_DEFINE::HttpStatusCode nStatusCode, const char *szBuf, int nLen,const string& strFileName);


	// 获取http响应包。
	const char* GetBuf(tint32 &nLen) const;

	// 获取状态码
	HTTP_DEFINE::HttpStatusCode GetStatusCode(void)const;


	// 是否是命令请求
	bool IsRequestCmd(void) const;

	// 是否是文件请求
	bool IsRequestFile(void) const;

	// 是否是回复包
	bool IsResponse(void) const;

	// 从HTTP头部获取校验信息
	bool GetVerifyInfo(std::string &strVerify) const;
    //从HTTP头部获取校验信息
    bool GetSessionInfo(std::string &strSession) const;
	bool GetCertPassword(std::string &strSession) const;
	// 设置/获取头部附加参数
	bool SetHeaderExtra(const char *pExtraKey, tuint32 dwParameter);
	bool GetHeaderExtra(const char *pExtraKey, tuint32 &dwParameter)const;

	//获取url中的参数
	bool GetUrlParam(std::string& strParam);

    bool SetHeaderLine(const char* pLine);
    std::string URLEncode(const char *sIn) const;
    bool CheckHttpHeaderType(HTTP_DEFINE::HTTP_MSG_TYPE_H& MsgTypeCode)const;
	bool GetSessionID(std::string &strSessionID) const;
	
private:
	std::string URLDecode(const char *sIn);								// 解码HTTP负载数据
	char fromHex(const char &x);										// 十六进制转换
	void FilterURL(void);												// 过滤URL
	const std::string GetMimeType(void);									// 获取Content-type

	int CreateHttpHeader(const HTTP_DEFINE::HttpStatusCode nStatusCode
		, char *pBuff, const int nDataLen
		, const std::string &strAuthorization=""
		, const std::string& nonStanadardHeader="");						// 创建http包头

	int CreateHttpHeader_1400(const HTTP_DEFINE::HttpStatusCode nStatusCode
		, char *pBuff, const int nDataLen);						// 创建GB1400协议http包头

     int CreateHttpHeader_downLoadFile(const HTTP_DEFINE::HttpStatusCode nStatusCode
                                   , char *pBuff, const int nDataLen,const std::string& fileNmae); // 创建HTTP响应头download file

	void  AnalyzeHttpPack(const char *szBuff, const int nBuffLen);			// 解析HTTP包
	tuint32 GetHttpHeader(const char *szBuff, std::string &strHeader) const;	// 获取头部信息

	char fromDec(const short &x) const; 
private:
	HTTP_DEFINE::HTTP_PROTOCOL m_HttpProtocol;		// HTTP协议信息
};

#endif //HTTP_PROTOCOL_ANALYSE_HEAD
