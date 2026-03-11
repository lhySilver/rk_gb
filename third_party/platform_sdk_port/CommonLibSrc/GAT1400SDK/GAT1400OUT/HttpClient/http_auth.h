#ifndef __1400_HTTP_AUTH_H_
#define __1400_HTTP_AUTH_H_

#include <string>

enum HTTP_AUTHTYPE
{
	TYPE_NULL,
	TYPE_BASIC,
	TYPE_DIGEST
};

class CHttpAuth
{
public:
	CHttpAuth(const std::string& url, const std::string& method, const std::string& name, const std::string& pwd);

	int  HttpAuthParse(const std::string& wwwAuth, std::string& strAuthorization);

private:
	int  HttpRespCheck( const std::string& strRep);
	void HttpDigest(std::string& strRealm, std::string& strNonce, std::string& strQop,  
		std::string& strCNonce, std::string& strNc, char *pResponce);
	void Authentication(char * pNonce);
	void CreateNonce( char* pNonce);
	bool Base64Encode( const char *pInBuf,  char *pOutBuf);
private:
	std::string m_url;
	std::string m_reqMethod;
	std::string m_Name;
	std::string m_password;

private:
	HTTP_AUTHTYPE m_wAuthType;
	std::string m_strRealm;
	std::string m_strQop;
	std::string m_strOpaque;
	std::string m_strNonce;
};

#endif