#ifndef HASH_OPENSSL_HEAD
#define HASH_OPENSSL_HEAD

typedef enum _tag_openssl_hash_type
{
	OPENSSL_HASH_TYPE_NULL = 0,
	OPENSSL_HASH_TYPE_MD5,
	OPENSSL_HASH_TYPE_SHA1,
	OPENSSL_HASH_TYPE_SHA224,
	OPENSSL_HASH_TYPE_SHA256,
	OPENSSL_HASH_TYPE_SHA384,
	OPENSSL_HASH_TYPE_SHA512,
}OPENSSL_HASH_TYPE;

typedef enum _tag_openssl_coding_type
{
	OPENSSL_CODING_TYPE_NULL = 0,
	OPENSSL_CODING_TYPE_HEX_UPPER,		//16进制字符串, 大写
	OPENSSL_CODING_TYPE_HEX_LOWER,		//16进制字符串, 小写
	OPENSSL_CODING_TYPE_NUMBER,			//数值, 无转换
	OPENSSL_CODING_TYPE_BASE64,			//数值基础上用Base64编码
	OPENSSL_CODING_TYPE_BASE64_NO_PADDING,	//数值基础上用Base64编码,but No fill '='
}OPENSSL_CODING_TYPE;

class CHashBase;

//基于OpenSSL实现的Hash算法, 包含: MD5, SHA1, SHA224, SHA256, SHA384, SHA512
class CHashOpenSSL
{
public:
	CHashOpenSSL(OPENSSL_HASH_TYPE HashType);
	~CHashOpenSSL(void);
	static bool Encrypt(const void *pIn, tuint32 dwInLen, char *szOut, tuint32 &dwOutLen, OPENSSL_HASH_TYPE HashType, OPENSSL_CODING_TYPE CodingType = OPENSSL_CODING_TYPE_HEX_UPPER);	//注意szOut不会有字符串结束符

	bool HexToStr(const char *szHexMD,char* szOut,tuint32 nLen);
	void Update(const void *pIn, tuint32 dwInLen);
	bool Final(char *szOut, tuint32 &dwOutLen, OPENSSL_CODING_TYPE CodingType = OPENSSL_CODING_TYPE_HEX_UPPER);	//注意szOut不会有字符串结束符
private:
	CHashBase  *m_pHashImp;
};

#endif	//HASH_OPENSSL_HEAD
