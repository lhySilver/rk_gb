#ifndef DATAENCRYPTION_H
#define DATAENCRYPTION_H

#include "openssl/aes.h"
#include <string>

class CDataEncryption
{
public:
	static int EncryptDataWithKey(const unsigned char *in, const int inLen,const char* key,
		unsigned char *out, unsigned int & crc32);
	static int DecryptDataWithKey(const unsigned char *in, const int inLen,
		const unsigned int crc32, const char *key, unsigned char *out);
	static int DecryptDataWithHexKey(const unsigned char *in, const int inLen,
		const unsigned int crc32, const char *key, const unsigned int keyLen, unsigned char *out);

	static void BinToHex(unsigned char *Buffer, char *Text, int BufSize);
	static int HexToBin(char *Text, char *Buffer, int BufSize);

	static bool EncryptCmdData(const char *pData, unsigned int datalen, const char *encryptKey, char *&pDataOut, unsigned int& dwOutLen);
	static bool DecryptCmdData(const char *pData, unsigned int datalen, const char *encryptKey, char *&pDataOut, unsigned int& dwOutLen);
	static int  EVPDecrypt(const std::string& key, const std::string& Ciphertext, std::string& Plaintxt);
	static int  EVPEncrypt(const std::string& key, const std::string& Plaintxt, std::string& Ciphertext);
private:
	static bool ResizeBufToMuitiple( char *&pDataOut, unsigned int &dwOutLen, unsigned int muitiple, const char *pData,  unsigned int datalen);
	static int DecryptData(const unsigned char *in, const int inLen,
		AES_KEY &aesKey, unsigned char *out);
	static unsigned int GetCRC32(unsigned int crc, char *buff, int len);
	static int HexCharToInt(char c);
	
};

#endif // DATAENCRYPTION_H