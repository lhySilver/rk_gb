#pragma once

#include <string>
#include <list>

class  CGBMANSRTSPReq
{
private:
	typedef struct __header
	{
		std::string strField;
		std::string strValue;
	}RTSP_HEADER;

public:
	CGBMANSRTSPReq(void);
	~CGBMANSRTSPReq(void);

	bool parseFromStr(char* pBuf, int nLen);
	std::string ToStr();
	std::string GenRspStr(int nStatusCode, char* pReason = NULL);

public:
	std::string Method() const { return m_method; }
	void Method(std::string val) { m_method = val; }
	std::string Body() const { return m_strBody; }
	void Body(std::string val) { m_strBody = val; }

	int PushHeader(std::string strField, std::string strValue);
	int ClearHeaders();
	std::string GetFieldValue(const std::string& strField);
	std::string GetRangeStr();
	std::string GetScaleStr();
    static std::string GetResponseMessage(int status,const std::string& description,const std::string& cseq);
    std::string GetCseq();
private:
	int ParseRequestLine(const char *buf, const char *&next);
	int ParseHeaders(const char *buf, const char *&next);
	int ParseBody(const char *buf, const char *&next);

private:
	std::string m_method;
	std::string m_version;

	std::list<RTSP_HEADER > m_HeaderList;
	std::string m_strBody;
};



class CGBMANSRTSPRsp
{
public:
	CGBMANSRTSPRsp(void);
	~CGBMANSRTSPRsp(void);

public:
	bool parseFromStr(char* pBuf, int nLen);

private:
	int ParseStatusLine(const char *buf, const char *&next);

private:
	std::string m_strReason;
	int m_nStatusCode;

	std::string m_version;
};

