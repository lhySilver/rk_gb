#ifndef BASE64_CODER_DDNS_H
#define BASE64_CODER_DDNS_H

#include <string>

using namespace std;

const int Base64IdxTab[128] =
{
	255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
	255,255,255,255,  255,255,255,255,  255,255,255,255,  255,255,255,255,
	255,255,255,255,  255,255,255,255,  255,255,255,62,   255,255,255,63,
	52,53,54,55,      56,57,58,59,      60,61,255,255,    255,255,255,255,
	255,0,1,2,        3,4,5,6,          7,8,9,10,         11,12,13,14,
	15,16,17,18,      19,20,21,22,      23,24,25,255,     255,255,255,255,
	255,26,27,28,     29,30,31,32,      33,34,35,36,      37,38,39,40,
	41,42,43,44,      45,46,47,48,      49,50,51,255,     255,255,255,255
};
#define BVal(x) Base64IdxTab[x]

class CBase64Coder
{
public:
	CBase64Coder();
	virtual ~CBase64Coder();

	/********************************************************* 
		* 函数说明：将输入数据进行base64编码 
		* 参数说明：[in]pIn      需要进行编码的数据 
					[in]uInLen  输入参数的字节数 
					[out]strOut 输出的进行base64编码之后的字符串 
		* 返回值  ：true处理成功,false失败 
	**********************************************************/   
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut); 

	/********************************************************* 
	* 函数说明：将输入数据进行base64编码 
	* 参数说明：[in]pIn          需要进行编码的数据 
	[in]uInLen      输入参数的字节数 
	[out]pOut       输出的进行base64编码之后的字符串 
	[out]uOutLen    输出的进行base64编码之后的字符串长度 
	* 返回值  ：true处理成功,false失败 
	**********************************************************/ 
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);	

	/********************************************************* 
	* 函数说明：将输入数据进行base64解码 
	* 参数说明：[in]strIn        需要进行解码的数据 
	[out]pOut       输出解码之后的节数数据 
	[out]uOutLen    输出的解码之后的字节数长度 
	* 返回值  ：true处理成功,false失败 
	**********************************************************/ 	
    bool static Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen) ;

 


	//////////////////////////////////////////////////////////////////////////
	void b64encode(unsigned char *src); 
	char *utf_8_uri_encoded(char *dest,char *src,char *prefix,char *suffix);
	int base64decone(const char *pInput, char *pOutput);

protected: 
	void doNumConvert(long num,char *rStr,int *strLen,long *x);
	int convertToString(double num,char *rStr);
	char *numStr(double num,char *dest);
	int utf_8_encoded_size(unsigned char ch);
	int utf_8_get_code_point(unsigned char *ch,unsigned int *cp);
	unsigned int utf_8_code_point(unsigned char *ch);
	static char encode(unsigned char u);


};


#endif	/////BASE64_CODER_DDNS_H



