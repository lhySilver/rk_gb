#include "http_auth.h"
#include "PUB_md5.h"
#include "PFHelper.h"
#include "Base64Coder.h"

CHttpAuth::CHttpAuth(const std::string& url, const std::string& method, const std::string& name, const std::string& pwd)
	: m_url(url)
	, m_reqMethod(method)
	, m_Name(name)
	, m_password(pwd)
{
	int ip1,ip2,ip3,ip4;
	int port = 0;
	char szUrl[128] = {0};
	sscanf(url.c_str(), "http://%d.%d.%d.%d:%d%s", &ip1, &ip2, &ip3, &ip4, &port, szUrl);
	m_url = szUrl;
}

int CHttpAuth::HttpAuthParse(const std::string& wwwAuth, std::string& strAuthorization)
{
	HttpRespCheck(wwwAuth);
	
	char szHttpHead[1024] = {0};
	char szResponse[512] = { 0};
	std::string strNc="00000001";

	char szNonce[64] = {0};
	CreateNonce( szNonce);
	std::string strCNonce = szNonce;

	if(m_wAuthType == TYPE_BASIC)
	{
		Authentication(szResponse);
		strAuthorization = std::string("Authorization: Basic ")+szResponse;
	}
	else
	{
		HttpDigest(m_strRealm, m_strNonce, m_strQop, strCNonce, strNc, szResponse);
		strAuthorization = std::string("Authorization: Digest username=\"")+m_Name+"\", "+"realm=\""+m_strRealm\
			+"\", "+"nonce=\""+m_strNonce+"\", " +"uri=\""+m_url+"\", " +"cnonce=\""+ strCNonce+"\", "\
			+"nc="+strNc+", "+"qop="+m_strQop+", response=\""+szResponse+"\""+", algorithm=\"MD5\"";
	}
	return 0;
}

int CHttpAuth::HttpRespCheck( const std::string& strRep)
{
	int nStart = 0;
	int nFind = 0;

	// if ( (nFind = strRep.find("Unauthorized")) !=std::string::npos || (nFind = strRep.find("Invalid Authority")) !=std::string::npos )
	{
		if ( (nFind = strRep.find("Basic")) !=std::string::npos)
		{
			m_wAuthType = TYPE_BASIC;
		}
		if ( (nFind = strRep.find("Digest")) !=std::string::npos)
		{
			m_wAuthType = TYPE_DIGEST;
			if ( (nStart = strRep.find("realm="))!=std::string::npos)
			{
				nStart += strlen("realm=")+1;
				if(  (nFind = strRep.find('\"',nStart))!=std::string::npos)
				{
					m_strRealm =  strRep.substr(nStart, nFind-nStart);
				}
			}

			if ( (nStart = strRep.find("qop="))!=std::string::npos)
			{
				nStart += strlen("qop=")+1;
				if((nFind = strRep.find('\"',nStart))!=std::string::npos)
				{
					m_strQop =  strRep.substr(nStart, nFind-nStart);
				}
			}

			if ( (nStart = strRep.find("nonce="))!=std::string::npos)
			{
				nStart += strlen("nonce=")+1;
				if((nFind = strRep.find('\"',nStart))!=std::string::npos)
				{
					m_strNonce =  strRep.substr(nStart, nFind-nStart);
				}
			}
			
			if ( (nStart = strRep.find("opaque="))!=std::string::npos)
			{
				nStart += strlen("opaque=")+1;
				if((nFind = strRep.find('\"',nStart))!=std::string::npos)
				{
					m_strOpaque =  strRep.substr(nStart, nFind-nStart);
				}
			}
		}

		return 0;
	}
	if((nFind = strRep.find("ERROR")) !=std::string::npos || (nFind = strRep.find("Server internal error")) !=std::string::npos )
	{
		return -1;
	}

	return 0;
}

void CHttpAuth::HttpDigest(std::string& strRealm, std::string& strNonce, std::string& strQop,  std::string& strCNonce, std::string& strNc, char *pResponce)
{
	//MD5(MD5(username:realm:password):nonce:nc:cnonce:qop:MD5(<request-method>:url))
	//md5(username:realm:password)
	char szTmp[2048] = { 0};
	snprintf( szTmp, sizeof(szTmp), "%s:%s:%s", m_Name.c_str(), strRealm.c_str(), m_password.c_str());
	char szHA1[2048] = { 0};
	PUB_MD5Encrypt( szTmp, strlen(szTmp), szHA1, false);

	//md5(method:digesturi)
	memset( szTmp, 0, sizeof(szTmp));
	snprintf( szTmp, sizeof(szTmp), "%s:%s", m_reqMethod.c_str(), m_url.c_str());
	char szHA2[2048] = { 0};
	PUB_MD5Encrypt( szTmp, strlen(szTmp), szHA2, false);

	//md5
	std::string strTmp  ;
	memset( szTmp, 0, sizeof(szTmp));
	if(!strNonce.empty())
	{
		strTmp=strTmp +":" +strNonce;
	}
	if(!strNc.empty())
	{
		strTmp=strTmp +":" +strNc;
	}
	if(!strCNonce.empty())
	{
		strTmp=strTmp +":" +strCNonce;

	}
	if(!strQop.empty())
	{
		strTmp=strTmp +":" +strQop;
	}
	if(strTmp.empty())
	{
		snprintf( szTmp, sizeof(szTmp), "%s:%s", szHA1 ,szHA2);
	}
	else
	{
		snprintf( szTmp, sizeof(szTmp), "%s%s:%s", szHA1,strTmp.c_str(), szHA2);
	}
	PUB_MD5Encrypt( szTmp, strlen(szTmp), pResponce, false);
}

void CHttpAuth::Authentication(char * pNonce)
{
	char szBuff[64] = {0};
	switch(m_wAuthType)
	{
	case TYPE_BASIC:
		sprintf(szBuff, "%s:%s", m_Name.c_str(), m_password.c_str());
		Base64Encode(szBuff, pNonce);
		break;
	case TYPE_DIGEST:
		break;
	default:
		snprintf(szBuff, sizeof(szBuff), "%s:%s", m_Name.c_str(), m_password.c_str());
		Base64Encode(szBuff, pNonce);
		break;
	}

}

void CHttpAuth::CreateNonce( char* pNonce)
{
	unsigned int dwSeed = 0;
	static unsigned int s_dwSeed = 0;
	++s_dwSeed;
	dwSeed = CPFHelper::GetTickCount() + s_dwSeed;

	PUB_MD5Encrypt(&dwSeed, 4, pNonce, false);
}

bool CHttpAuth::Base64Encode( const char *pInBuf,  char *pOutBuf)
{
	tuint32 inSize = strlen( pInBuf);
	unsigned long outSize = (((inSize)+2) / 3 * 4 + 1);//AV_BASE64_SIZE
	return CBase64Coder::Encode((const unsigned char *)pInBuf, inSize, (unsigned char*)pOutBuf, &outSize);
}