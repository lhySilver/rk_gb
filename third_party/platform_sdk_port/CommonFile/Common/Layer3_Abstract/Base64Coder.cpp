
#include <stdlib.h>  
#include <stdio.h>
#include <string.h>
#include"Base64Coder.h"
#include "WarningDisable.h"



static const char *g_pCodes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="; 



static const unsigned char g_pMap[256] =  
{  
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48, 
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};  
 
CBase64Coder::CBase64Coder()
{
}

CBase64Coder::~CBase64Coder()
{
} 

bool CBase64Coder::Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen)
{  
	unsigned long i, len2, leven; 
	unsigned char *p;  
					 
	if(pOut == NULL || *uOutLen == 0)  
		return false; 
	
	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));   
	
	len2 = ((uInLen + 2) / 3) << 2; 
	if((*uOutLen) < (len2 + 1)) return false; 
	
	p = pOut; 
	leven = 3 * (uInLen / 3);  
	for(i = 0; i < leven; i += 3)  	
	{  
		*p++ = g_pCodes[pIn[0] >> 2];  	  
		*p++ = g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];  	 
		*p++ = g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];  	
		*p++ = g_pCodes[pIn[2] & 0x3f];  		 
		pIn += 3;  							
	}  		 
		   
	if (i < uInLen)		
	{  	
		unsigned char a = pIn[0];  
		unsigned char b = ((i + 1) < uInLen) ? pIn[1] : 0;   
		unsigned char c = 0;  
		
		*p++ = g_pCodes[a >> 2];  
		*p++ = g_pCodes[((a & 3) << 4) + (b >> 4)];
		*p++ = ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';  
		*p++ = '=';   
	}  		 
	
	*p = 0; // Append NULL byte   
	*uOutLen = p - pOut;  	 
	return true;  	
}  			  

bool CBase64Coder::Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut) 
{  		 
	unsigned long i, len2, leven;  
	strOut = ""; 
	
	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL)); 
	
	len2 = ((uInLen + 2) / 3) << 2;    
	//if((*uOutLen) < (len2 + 1)) return false; 
	
	//p = pOut;    
	leven = 3 * (uInLen / 3);  	
	for(i = 0; i < leven; i += 3)  
	{  	
		strOut += g_pCodes[pIn[0] >> 2];  
		strOut += g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)]; 
		strOut += g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)]; 
		strOut += g_pCodes[pIn[2] & 0x3f]; 
		pIn += 3;  
	}  	  
	
	if (i < uInLen)  
	{  	  
		unsigned char a = pIn[0];  	
		unsigned char b = ((i + 1) < uInLen) ? pIn[1] : 0; 
		unsigned char c = 0;  
		
		strOut += g_pCodes[a >> 2]; 
		strOut += g_pCodes[((a & 3) << 4) + (b >> 4)];    
		strOut += ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		strOut += '='; 
	}  	
	
	//*p = 0; // Append NULL byte  	
	//*uOutLen = p - pOut;    
	return true;  	
}  
		  
bool CBase64Coder::Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen)
{ 
	unsigned long t, x, y, z;  
	unsigned char c;  	 
	unsigned long g = 3; 
	
	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));   
	
	for(x = y = z = t = 0; x < strIn.length(); x++)    
	{  
		c = g_pMap[strIn[x]];
		if(c == 255) continue;  
		if(c == 254) { c = 0; g--; }  	   
		
		t = (t << 6) | c; 
		
		if(++y == 4)  
		{ 
			if((z + g) > *uOutLen) { return false; } // Buffer overflow   
			pOut[z++] = (unsigned char)((t>>16)&255); 
			if(g > 1) pOut[z++] = (unsigned char)((t>>8)&255);   
			if(g > 2) pOut[z++] = (unsigned char)(t&255);
			y = t = 0;  
		}  	   
	}  

	*uOutLen = z;  
	return true;  
}




//////////////////////////////////////////////////////////////////////////
void CBase64Coder::doNumConvert(long num,char *rStr,int *strLen,long *x)
{
	  if((*x=num/10))

		doNumConvert(*x,rStr,strLen,x);

	  *(rStr+(*(strLen))++)=num%10+'0';
}


int CBase64Coder::convertToString(double num,char *rStr)
{
	  int    strLen=0;
	  long   x;


	  doNumConvert((long) num,rStr,&strLen,&x);   

	  *(rStr+strLen)=0;


	  return strLen;
}

char *CBase64Coder::numStr(double num,char *dest)
{
	convertToString(num,dest);
	return dest;
}

int CBase64Coder::utf_8_encoded_size(unsigned char ch)
{

	if (ch<128)
		return 1;

	if (ch>=252)
		return 6;

	if (ch>=248)
		return 5;

	if (ch>=240)
		return 4;

	if (ch>=224)
		return 3;

	if (ch>=192)
		return 2;

	return 0;
}

int CBase64Coder::utf_8_get_code_point(unsigned char *ch,unsigned int *cp)
{

	int				i;
	int				numBytes;

	numBytes=utf_8_encoded_size(ch[0]);

	if (numBytes==0) 
	{

		*cp=0;
	}
	else 
	{

		if (numBytes==1) 
		{
			*cp=ch[0];
		} 
		else 
		{
			/*mask out bytes count bits, and make room for rest of character*/
			*cp=(ch[0] & (255>>numBytes))<<((numBytes-1)*6);

			for (i=1;i<numBytes;i++)

			/*mask out (& 0011 1111) meta bits (first two), and make room*/
			*cp+=((ch[i] & 63)<<((numBytes-1-i)*6));
		}
	}

	return numBytes;
}


unsigned int CBase64Coder::utf_8_code_point(unsigned char *ch)
{

	unsigned int cp;

	utf_8_get_code_point(ch,&cp);

	return cp;
}

char *CBase64Coder::utf_8_uri_encoded(char *dest,char *src,char *prefix,char *suffix)
{
	int				len;
	int				srcIndex=0;
	unsigned long	ch;
	char			cp_buff[12];

	len=strlen(src);
	
	while (srcIndex<len) 
	{
		ch=utf_8_code_point((unsigned char *) (src+srcIndex));
		if (!(ch==0)) 
		{
			if (ch==(unsigned char) src[srcIndex]) 
			{
				dest[strlen(dest)]=src[srcIndex];
			}
			else 
			{
				strcat(dest,prefix);
				strcat(dest,numStr(ch,cp_buff));
				strcat(dest,suffix);
			}
		}

		srcIndex++;
	}

	return dest;
}

char CBase64Coder::encode(unsigned char u) 
{

	if(u < 26)  return 'A'+u;
	if(u < 52)  return 'a'+(u-26);
	if(u < 62)  return '0'+(u-52);
	if(u == 62) return '+';
  
	return '/';

}


/*
 * Base64 encode and return size data in 'src'. The caller must free the
 * returned string.
 * @param size The size of the data in src
 * @param src The data to be base64 encode
 * @return encoded string otherwise NULL
 */
void CBase64Coder::b64encode(unsigned char *src) 
{

	int i, size;
	char *out, *p;
	size=strlen((char *)src);
	int destSize=size*4/3+4+4;
	out=(char *)malloc(destSize);
	memset(out,0,destSize);
	p=out;
	for(i=0; i<size; i+=3) 
	{
      
		unsigned char b1=0, b2=0, b3=0, b4=0, b5=0, b6=0, b7=0;
		b1 = src[i];
		if(i+1<size)
			b2 = src[i+1];
		if(i+2<size)
			b3 = src[i+2];
      
		b4= b1>>2;
		b5= ((b1&0x3)<<4)|(b2>>4);
		b6= ((b2&0xf)<<2)|(b3>>6);
		b7= b3&0x3f;
      
		*p++= encode(b4);
		*p++= encode(b5);
      
		if(i+1<size) 
		{
			*p++= encode(b6);
		} 
		else 
		{
			*p++= '=';
		}
      
		if(i+2<size)
		{
			*p++= encode(b7);
		} 
		else 
		{
			*p++= '=';
		}

	}
	memset(src,0,strlen((char*)src));
	strcpy((char*)src,out);
	free(out);
}

int	CBase64Coder:: base64decone(const char *pInput, char *pOutput)
{
	int i = 0;
	int iCnt = 0;
	int iSrcLen = (int)strlen(pInput);
	char * p = pOutput;
	for (i=0; i < iSrcLen; i++)
	{
		if (pInput[i] == '=') return p-pOutput+1;
		int a = BVal((int)pInput[i]); 
		if (a == 255) continue;
		switch (iCnt)
		{
		case 0:
			{
				*p = a << 2;
				iCnt++;
			}
			break;
		case 1:
			{
				*p++ |= a >> 4;
				*p = a << 4;
				iCnt++;
			}
			break;
		case 2:
			{
				*p++ |= a >> 2;
				*p = a << 6;
				iCnt++;
			}
			break;
		case 3:
			{
				*p++ |= a;
				iCnt = 0;
			}
			break;
		} 
	}
	*p = 0x00;
	return p-pOutput;
}


