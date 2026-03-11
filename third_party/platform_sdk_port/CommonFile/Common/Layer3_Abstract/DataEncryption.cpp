#include "string.h"
#include "DataEncryption.h"
#include "PUB_md5.h"
#include "openssl/evp.h"

int CDataEncryption::EncryptDataWithKey(const unsigned char *in, const int inLen,const char* key, unsigned char *out, unsigned int& crc32)
{
    if(0 != inLen % 16)
        return -1;

	//char md5Key[64];
	//memset(md5Key,0, sizeof(md5Key));
	//PUB_MD5Encrypt(key, strlen(key),md5Key,true);

	unsigned char binKey[16];
	memset(binKey,0,sizeof(binKey));
	HexToBin((char*)key, (char*)binKey, sizeof(binKey));


    crc32 = GetCRC32(crc32,(char*)in, inLen);
    AES_KEY aesKey;

    AES_set_encrypt_key(binKey,128, &aesKey);


    int len = inLen;
    int index = 0;
    while (len) 
	{
        const unsigned char* tmpIn = in+index*16;
        if(tmpIn == out)
        {
            unsigned char tmpBuf[16];
            memcpy(tmpBuf, tmpIn,16);

            AES_ecb_encrypt(tmpBuf, out, &aesKey,1);
        }else
        {
            AES_ecb_encrypt(tmpIn, out, &aesKey,1);
        }
        len -= 16;
        out += 16;
        ++index;
    }
    return 0;
}

int CDataEncryption::DecryptDataWithKey(const unsigned char *in, const int inLen,
										const unsigned int crc32, const char *key, unsigned char *out)
{
	/*
	char md5Key[64];
	memset(md5Key,0, sizeof(md5Key));
	PUB_MD5Encrypt(key, strlen(key),md5Key,true);*/

	unsigned char binKey[16];
	memset(binKey,0,sizeof(binKey));
	HexToBin((char*)key, (char*)binKey, sizeof(binKey));

	if(0 != inLen % 16)
		return -1;

	AES_KEY aesKey;
	AES_set_decrypt_key(binKey,128, &aesKey);

	int nRet = DecryptData(in,inLen, aesKey,out);
	if(0 != nRet)
	{
		return nRet;
	}

	tuint32 tmpcrc = 0;
	tmpcrc = GetCRC32(tmpcrc,(char*)out,inLen);
	if(tmpcrc == crc32)
	{
		return 0;
	}else
	{
		return -2;
	}
}

int CDataEncryption::DecryptDataWithHexKey(const unsigned char *in, const int inLen,
	const unsigned int crc32, const char *key, const unsigned int keyLen, unsigned char *out)
{
	if ((keyLen != 16) && (keyLen != 24) && (keyLen != 32)) {
		return -1;
	}

	AES_KEY aesKey;
	AES_set_decrypt_key((const unsigned char*)key, keyLen<<3, &aesKey);

	int nRet = DecryptData(in, inLen, aesKey, out);
	if(0 != nRet)
	{
		return nRet;
	}

	tuint32 tmpcrc = 0;
	tmpcrc = GetCRC32(tmpcrc,(char*)out,inLen);
	if(tmpcrc == crc32)
	{
		return 0;
	}else
	{
		return -2;
	}
}

bool CDataEncryption::ResizeBufToMuitiple(char *&pDataOut, unsigned int &dwOutLen, unsigned int muitiple, const char *pData, unsigned int datalen)
{
	bool bRet = false;
	if(muitiple > 1)
	{
		dwOutLen = (datalen + (muitiple - 1))/muitiple * muitiple;

		//if(dwOutLen != datalen)
		{
			pDataOut = new char[dwOutLen+1];

			memcpy(pDataOut, pData, datalen);

			memset(pDataOut + datalen, 0, sizeof(char) * (dwOutLen - datalen+1));
			bRet = true;
		}
// 		else
// 		{
// 			pDataOut = const_cast<char*>(pData);
// 		}
	}
	else
	{
		pDataOut = const_cast<char*>(pData);
		dwOutLen = datalen;
	}
	return bRet;
}

int CDataEncryption::DecryptData(const unsigned char *in, const int inLen,
								 AES_KEY &aesKey, unsigned char *out)
{
	int len = inLen;
	int index = 0;
	unsigned char* tmpOut = out;
	while (len)
	{
		const unsigned char* tmpIn = in+index*16;
		if(tmpIn == tmpOut)
		{
			unsigned char tmpBuf[16];
			memcpy(tmpBuf, tmpIn,16);

			AES_ecb_encrypt(tmpBuf, tmpOut, &aesKey,0);
		}else
		{

			AES_ecb_encrypt(tmpIn, tmpOut, &aesKey,0);
		}
		len -= 16;
		tmpOut += 16;
		++index;
	}
	return 0;
}


unsigned int CDataEncryption::GetCRC32(unsigned int crc, char *buff, int len)
{
	unsigned int POLYNOMIAL = 0xEDB88320 ;
	static unsigned int table[256] ;
	static bool bInitTable = false;
	if(!bInitTable)
	{
		memset(table, 0, sizeof(table));
		int i = 0, j = 0;
		for (i = 0 ; i < 256 ; i++)
		{
			for (j = 0, table[i] = i ; j < 8 ; j++)
				table[i] = (table[i]>>1)^((table[i]&1)?POLYNOMIAL:0);
		}
		bInitTable = true;
	}

	crc = ~crc;

	for (int i = 0; i < len; i++)
		crc = (crc >> 8) ^ table[(crc ^ buff[i]) & 0xff];

	return ~crc;
}

int CDataEncryption::HexCharToInt(char c)
{
	if ((c>='A')&&(c<='Z')) c |= 32;  //convert to lowercase
	if ((c>='a')&&(c<='f')) return (c - 'a' +10);
	if ((c>='0')&&(c<='9')) return (c -'0');
	return -1;
}

int CDataEncryption::HexToBin(char *Text, char *Buffer, int BufSize)
{
	char c1, c2;
	int  v1, v2;
	int binlen;

	binlen = 0;
	while ((BufSize>0)&&(*Text))
	{
		c1 = *Text;	Text++;     if (*Text=='\0') break;  //断开了
		c2 = *Text;	Text++;
		if ((c1==0)||(c2==0)) break;
		v1 = HexCharToInt(c1);
		v2 = HexCharToInt(c2);
		if ((v1==-1)||(v2==-1)) break;
		v1 = (v1*16 + v2);
		*((unsigned char*)Buffer) = (unsigned char)(v1);
		Buffer ++;
		BufSize --;
		binlen ++;
	}
	return binlen;
}

void CDataEncryption::BinToHex(unsigned char *Buffer, char *Text, int BufSize)
{
	int i;
	for (i=0; i<BufSize; i++, Buffer++, Text+=2)
		sprintf(Text, "%02X", *Buffer);
}

bool CDataEncryption::EncryptCmdData(const char *pData, unsigned int datalen, const char *encryptKey, char *&pDataOut, unsigned int& dwOutLen)
{
	bool bRet = ResizeBufToMuitiple(pDataOut, dwOutLen, 16, pData, datalen);

	tuint32 dwCrc = 0;
	EncryptDataWithKey((unsigned char *)pDataOut, dwOutLen, encryptKey, (unsigned char *)pDataOut, dwCrc);
	return bRet;
}

bool CDataEncryption::DecryptCmdData(const char *pData, unsigned int datalen, const char *encryptKey, char *&pDataOut, unsigned int& dwOutLen)
{
	bool bRet = ResizeBufToMuitiple(pDataOut, dwOutLen, 16, pData, datalen);

	tuint32 dwCrc = 0;

	DecryptDataWithKey((unsigned char *)pDataOut, dwOutLen, dwCrc, encryptKey, (unsigned char *)pDataOut);

	return bRet;
}


int CDataEncryption::EVPDecrypt(const std::string& key, const std::string& Ciphertext, std::string& Plaintxt)
{

	if (key.size() != 16) {
		return -1;
	}

	char userkey[AES_BLOCK_SIZE];

	memcpy(userkey, key.c_str(), 16);
	
	char iv[EVP_MAX_IV_LENGTH];
	
	int ret;
	int tlen = 0;
	int flen = 0;

	EVP_CIPHER_CTX ctx;
	EVP_CIPHER_CTX_init(&ctx);

	ret = EVP_DecryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, (const unsigned char *)userkey, (const unsigned char *)iv);
	if (ret != 1) {
		printf("EVP_DecryptInit_ex failed\n");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}

	unsigned char buf[1024];
	int buf_len = 1024;

	ret = EVP_DecryptUpdate(&ctx, buf, &buf_len, (const unsigned char *)Ciphertext.c_str(), Ciphertext.size());
	if (ret != 1) {
		printf("EVP_DecryptUpdate failed\n");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}

	ret = EVP_DecryptFinal_ex(&ctx, buf + buf_len, &flen);
	if (ret != 1) {
		printf("EVP_DecryptFinal_ex failed\n");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}
	
	tlen = buf_len + flen;
	
	Plaintxt = std::string((char*)buf, tlen);
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 0;
}


int CDataEncryption::EVPEncrypt(const std::string& key, const std::string& Plaintxt, std::string& Ciphertext)
{
	if (key.size() != 16) {
		return -1;
	}

	char userkey[AES_BLOCK_SIZE];

	memcpy(userkey, key.c_str(), 16);

	char iv[EVP_MAX_IV_LENGTH];

	int ret;
	int tlen = 0;
	int flen = 0;

	EVP_CIPHER_CTX ctx;
	/*初始化ctx*/
	EVP_CIPHER_CTX_init(&ctx);
	/*指定加密算法及key和iv(此处IV没有用)*/
	ret = EVP_EncryptInit_ex(&ctx, EVP_aes_128_ecb(), NULL, (const unsigned char *)userkey, (const unsigned char *)iv);
	if (ret != 1) {
		printf("EVP_EncryptInit_ex failed\n");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}

	
	/*进行加密操作*/
	unsigned char buf[1024] = {0};
	int buf_len = 1024;

	ret = EVP_EncryptUpdate(&ctx, buf, &buf_len, (const unsigned char *)Plaintxt.c_str(), Plaintxt.size());
	if (ret != 1) {
		printf("EVP_EncryptUpdate failed\n");
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}
	
	if (!EVP_EncryptFinal_ex(&ctx, buf + buf_len, &flen)){
		printf("ERROR in EVP_EncryptFinal_ex \n");
		return -1;
	}

	tlen = buf_len + flen;
	Ciphertext = std::string((char*)buf, tlen);
	EVP_CIPHER_CTX_cleanup(&ctx);
	return 1;
}