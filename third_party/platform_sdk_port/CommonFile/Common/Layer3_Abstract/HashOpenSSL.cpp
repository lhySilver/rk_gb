#include "base_type_define.h"
#include "HashOpenSSL.h"
#include "openssl/sha.h"

class CHashBase
{
public:
	virtual ~CHashBase(){}
	virtual void Update(const void *pIn, tuint32 dwInLen)=0;
	virtual bool Final(char *szOut, tuint32 &dwOutLen, OPENSSL_CODING_TYPE CodingType)=0;
};

class CSHA512OpenSSLImp :public CHashBase
{
public:
	CSHA512OpenSSLImp()
	{
		SHA512_Init(&m_Content);
	}

	virtual void Update(const void *pIn, tuint32 dwInLen)
	{
		SHA512_Update(&m_Content, pIn, dwInLen);
	}

	virtual bool HexToStr(const char *szHexMD,char* szOut,tuint32 nLen)
	{
		tuint32 i,j=0;
		char l_byTmp;
		for (i = 0; i < nLen; i++)
		{
			l_byTmp = (szHexMD[i] & 0xf0) >> 4;
			if(l_byTmp < 10)
			{
				szOut[j++] = l_byTmp +'0';
			}
			else
			{
				szOut[j++] = l_byTmp - 10 +'A';
			}
			l_byTmp = szHexMD[i] & 0x0f;
			if(l_byTmp < 10)
			{
				szOut[j++] = l_byTmp +'0';
			}
			else
			{
				szOut[j++] = l_byTmp - 10 +'A';
			}

			szOut[j++] = ' ';
		}
		szOut[j] = 0;

		return true;
	}

	virtual bool CoverOutPut(const unsigned char *szHexMD, tuint32 dwMDLen, char *szOut, tuint32 &dwOutLen, OPENSSL_CODING_TYPE CodingType)
	{
		switch (CodingType)
		{
		case OPENSSL_CODING_TYPE_HEX_UPPER:
		case OPENSSL_CODING_TYPE_HEX_LOWER:
			if (HexToStr((const char *)szHexMD, szOut,dwMDLen))
			{
				dwOutLen = dwMDLen * 2;
				return true;
			}
			break;

		case OPENSSL_CODING_TYPE_NUMBER:
			if (dwOutLen >= dwMDLen)
			{
				memcpy(szOut, szHexMD, dwMDLen);
				dwOutLen = dwMDLen;

				return true;
			}
			break;
		default:
			ASSERT(false);
			break;
		}

		ASSERT(false);
		return false;
	}

	virtual bool Final(char *szOut, tuint32 &dwOutLen, OPENSSL_CODING_TYPE CodingType)
	{
		unsigned char Digest[SHA512_DIGEST_LENGTH];
		SHA512_Final(Digest, &m_Content);
		SHA512_Init(&m_Content);
		return CoverOutPut(Digest, sizeof(Digest), szOut, dwOutLen, CodingType);
	}
private:
	SHA512_CTX m_Content;
};

CHashOpenSSL::CHashOpenSSL(OPENSSL_HASH_TYPE HashType)
{
	switch (HashType)
	{
	case OPENSSL_HASH_TYPE_SHA512:
		m_pHashImp = new CSHA512OpenSSLImp;
		break;
	default:
		printf("HashType = %d\n", HashType);
		ASSERT(false);
		m_pHashImp = NULL;
		break;
	}
}

CHashOpenSSL::~CHashOpenSSL()
{
	if (m_pHashImp)
	{
		delete m_pHashImp;
	}
}

bool CHashOpenSSL::Encrypt(const void *pIn, tuint32 dwInLen, char *szOut, tuint32 &dwOutLen, OPENSSL_HASH_TYPE HashType, OPENSSL_CODING_TYPE CodingType/* = OPENSSL_CODING_TYPE_HEX_UPPER*/)
{
	switch (HashType)
	{
	case OPENSSL_HASH_TYPE_SHA512:
		{
			CSHA512OpenSSLImp SHA512Imp;
			SHA512Imp.Update(pIn, dwInLen);
			SHA512Imp.Final(szOut, dwOutLen, CodingType);
			break;
		}
	default:
		printf("HashType = %d\n", HashType);
		ASSERT(false);
		return false;
		break;
	}
	return true;
}

void CHashOpenSSL::Update(const void *pIn, tuint32 dwInLen)
{
	if (m_pHashImp)
	{
		m_pHashImp->Update(pIn, dwInLen);
	}
}

bool CHashOpenSSL::Final(char *szOut, tuint32 &dwOutLen, OPENSSL_CODING_TYPE CodingType)
{
	if (m_pHashImp)
	{
		return m_pHashImp->Final(szOut, dwOutLen, CodingType);
	}
	return false;
}




