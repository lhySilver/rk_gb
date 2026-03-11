#include "Infra/Security.h"
#include "Infra/Types.h"
#include <memory.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* POINTER defines a generic pointer type */
typedef unsigned char* POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short  UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long  UIN;

/* MD5 context. */
typedef struct
{
	UIN state[4];        /* state (ABCD) */
	UIN count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];        /* input buffer */
}
MD5_CTX;

// initial permutation IP
const static char IP_Table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
// final permutation IP^-1 
const static char IPR_Table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41,  9, 49, 17, 57, 25
};
// expansion operation matrix
static const char E_Table[48] = {
	32,  1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32,  1
};
// 32-bit permutation function P used on the output of the S-boxes 
const static char P_Table[32] = {
	16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23, 26, 5,  18, 31, 10,
	2,  8, 24, 14, 32, 27, 3,  9,  19, 13, 30, 6,  22, 11, 4,  25
};
// permuted choice table (key) 
const static char PC1_Table[56] = {
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};
// permuted choice key (table) 
const static char PC2_Table[48] = {
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};
// number left rotations of pc1 
const static char LOOP_Table[16] = {
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};
// The (in)famous S-boxes 
const static char S_Box[8][4][16] = {
	// S1 
	{{14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
	{0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
	{4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
    {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}},
	// S2 
    {{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
	{3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
	{0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
    {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}},
	// S3 
    {{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
	{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
	{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
    {1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}},
	// S4 
    {{7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
	{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
	{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
    {3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}},
	// S5 
    {{2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
	{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
	{4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
    {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}},
	// S6 
    {{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
	{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
	{9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
    {4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}},
	// S7 
    {{4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
	{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
	{1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
    {6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}},
	// S8 
    {{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
	{1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
	{7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
    {2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}}
};

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

enum    {ENCRYPT,DECRYPT};

typedef bool    (*PSubKey)[16][48];

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

static void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type);//标准DES加/解密
static void SetKey(const char* Key, int len);// 设置密钥
static void SetSubKey(PSubKey pSubKey, const char Key[8]);// 设置子密钥
static void F_func(bool In[32], const bool Ki[48]);// f 函数
static void S_func(bool Out[32], const bool In[48]);// S 盒代替
static void Transform(bool *Out, bool *In, const char *Table, int len);// 变换
static void Xor(bool *InA, const bool *InB, int len);// 异或
static void RotateL(bool *In, int len, int loop);// 循环左移
static void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
static void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

static bool SubKey[2][16][48];// 16圈子密钥
static bool Is3DES;// 3次DES标志
static char Tmp[256], deskey[16];

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

bool DesEncrypt(char *pResult, 
				char *pOrig, 
				long iOrigLen, 
				const char *pKey, 
				int iKeylen)
{
    if( !( pResult && pOrig && pKey && (iOrigLen=(iOrigLen+7)&0xfffffff8) ) ) 
		return false;
	SetKey(pKey, iKeylen);
	if( !Is3DES ) {   // 1次DES
		for(long i=0,j=iOrigLen>>3; i<j; ++i,pResult+=8,pOrig+=8)
			DES(pResult, pOrig, &SubKey[0], ENCRYPT);
	} else{   // 3次DES 加密:加(key0)-解(key1)-加(key0) 解密::解(key0)-加(key1)-解(key0)
		for(long i=0,j=iOrigLen>>3; i<j; ++i,pResult+=8,pOrig+=8) {
			DES(pResult, pOrig,  &SubKey[0], ENCRYPT);
			DES(pResult, pResult, &SubKey[1], !ENCRYPT);
			DES(pResult, pResult, &SubKey[0], ENCRYPT);
		}
	}
	return true;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

bool DesDecrypt(char *pResult, 
				char *pOrig, 
				long iOrigLen, 
				const char *pKey, 
				int iKeylen)
{
    if( !( pResult && pOrig && pKey && (iOrigLen=(iOrigLen+7)&0xfffffff8) ) ) 
		return false;
	SetKey(pKey, iKeylen);
	if( !Is3DES ) {   // 1次DES
		for(long i=0,j=iOrigLen>>3; i<j; ++i,pResult+=8,pOrig+=8)
			DES(pResult, pOrig, &SubKey[0], DECRYPT);
	} else{   // 3次DES 加密:加(key0)-解(key1)-加(key0) 解密::解(key0)-加(key1)-解(key0)
		for(long i=0,j=iOrigLen>>3; i<j; ++i,pResult+=8,pOrig+=8) {
			DES(pResult, pOrig,  &SubKey[0], DECRYPT);
			DES(pResult, pResult, &SubKey[1], !DECRYPT);
			DES(pResult, pResult, &SubKey[0], DECRYPT);
		}
	}
	return true;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void SetKey(const char* Key, int len)
{
	memset(deskey, 0, 16);
	memcpy(deskey, Key, len>16?16:len);
	SetSubKey(&SubKey[0], &deskey[0]);
	Is3DES = len>8 ? (SetSubKey(&SubKey[1], &deskey[8]), true) : false;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void DES(char Out[8], char In[8], const PSubKey pSubKey, bool Type)
{
    static bool M[64], tmp[32], *Li=&M[0], *Ri=&M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP_Table, 64);
    if( Type == ENCRYPT ){
        for(int i=0; i<16; ++i) {
            memcpy(tmp, Ri, 32);
            F_func(Ri, (*pSubKey)[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, tmp, 32);
        }
    }else{
        for(int i=15; i>=0; --i) {
            memcpy(tmp, Li, 32);
            F_func(Li, (*pSubKey)[i]);
            Xor(Li, Ri, 32);
            memcpy(Ri, tmp, 32);
        }
	}
    Transform(M, M, IPR_Table, 64);
    BitToByte(Out, M, 64);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void SetSubKey(PSubKey pSubKey, const char Key[8])
{
    static bool K[64], *KL=&K[0], *KR=&K[28];
    ByteToBit(K, Key, 64);
    Transform(K, K, PC1_Table, 56);
    for(int i=0; i<16; ++i) {
        RotateL(KL, 28, LOOP_Table[i]);
        RotateL(KR, 28, LOOP_Table[i]);
        Transform((*pSubKey)[i], K, PC2_Table, 48);
    }
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void F_func(bool In[32], const bool Ki[48])
{
    static bool MR[48];
    Transform(MR, In, E_Table, 48);
    Xor(MR, Ki, 48);
    S_func(In, MR);
    Transform(In, In, P_Table, 32);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void S_func(bool Out[32], const bool In[48])
{
    for(int i=0,j,k; i<8; ++i,In+=6,Out+=4) {
        j = (In[0]<<1) + In[5];
        k = (In[1]<<3) + (In[2]<<2) + (In[3]<<1) + In[4];
		ByteToBit(Out, &S_Box[i][j][k], 4);
    }
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void Transform(bool *Out, bool *In, const char *Table, int len)
{
    for(int i=0; i<len; ++i)
        Tmp[i] = In[ Table[i]-1 ];
    memcpy(Out, Tmp, len);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void Xor(bool *InA, const bool *InB, int len)
{
    for(int i=0; i<len; ++i)
        InA[i] ^= InB[i];
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void RotateL(bool *In, int len, int loop)
{
    memcpy(Tmp, In, loop);
    memcpy(In, In+loop, len-loop);
    memcpy(In+len-loop, Tmp, loop);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void ByteToBit(bool *Out, const char *In, int bits)
{
    for(int i=0; i<bits; ++i)
        Out[i] = (In[i>>3]>>(i&7)) & 1;
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

void BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, bits>>3);
    for(int i=0; i<bits; ++i)
        Out[i>>3] |= In[i]<<(i&7);
}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

//////////////////////////////////////////////////////////////////////////
/// MD5算法
/*
* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
* rights reserved.
*  
* License to copy and use this software is granted provided that it
* is identified as the "RSA Data Security, Inc. MD5 Message-Digest
* Algorithm" in all material mentioning or referencing this software
* or this function.
*  
* License is also granted to make and use derivative works provided
* that such works are identified as "derived from the RSA Data
* Security, Inc. MD5 Message-Digest Algorithm" in all material
* mentioning or referencing the derived work.
*  
* RSA Data Security, Inc. makes no representations concerning either
* the merchantability of this software or the suitability of this
* software for any particular purpose. It is provided "as is"
* without express or implied warranty of any kind.
*  
* These notices must be retained in any copies of any part of this
* documentation and/or software.
*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/* Constants for MD5Transform routine.
*/
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static void MD5Transform(UIN  aa[4], unsigned char bb[64]);
static void Encode(unsigned char *aa, UIN *bb, uint cc);
static void Decode(UIN *aa, unsigned char *bb, uint cc);
static void MD5_memcpy(POINTER aa, POINTER bb, uint cc);
static void MD5_memset(POINTER aa, int bb, uint cc);

static unsigned char PADDING[64] =
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (UIN)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (UIN)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (UIN)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}

#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (UIN)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
	}


/* MD5 initialization. Begins an MD5 operation, writing a new context.
*/
void MD5Init(MD5_CTX *context)
{
	context->count[0] = context->count[1] = 0;

	/* Load magic initialization constants.
	*/
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5Update(MD5_CTX *context, unsigned char *input, uint inputLen)
{
	uint i, index, partLen;

	/* Compute number of bytes mod 64 */
	index = (uint)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((UIN)inputLen << 3))
		< ((UIN)inputLen << 3))
	{
		context->count[1]++;
	}
	context->count[1] += ((UIN)inputLen >> 29);

	partLen = 64 - index;

	/* Transform as many times as possible.
	*/
	if (inputLen >= partLen)
	{
		MD5_memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen);
		MD5Transform (context->state, context->buffer);

		for (i = partLen; i + 63 < inputLen; i += 64)
		{
			MD5Transform (context->state, &input[i]);
		}

		index = 0;
	}
	else
	{
		i = 0;
	}

	/* Buffer remaining input */
	MD5_memcpy((POINTER)&context->buffer[index], (POINTER)&input[i],\
		inputLen - i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void MD5Final(unsigned char digest[16], MD5_CTX  *context)
{
	unsigned char bits[8];
	uint index, padLen;

	/* Save number of bits */
	Encode (bits, context->count, 8);

	/* Pad out to 56 mod 64.
	*/
	index = (uint)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update (context, PADDING, padLen);

	/* Append length (before padding) */
	MD5Update (context, bits, 8);

	/* Store state in digest */
	Encode (digest, context->state, 16);

	/* Zeroize sensitive information.
	*/
	MD5_memset ((POINTER)context, 0, sizeof (*context));
}

/* MD5 basic transformation. Transforms state based on block.
*/
static void MD5Transform(UIN state[4], unsigned char block[64])
{
	UIN a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.
	*/
	MD5_memset ((POINTER)x, 0, sizeof (x));
}

/* Encodes input (UINT4) into output (unsigned char). Assumes len is
a multiple of 4.
*/
static void Encode(unsigned char *output, UIN *input, uint len)
{
	uint i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is
a multiple of 4.
*/
static void Decode(UIN *output, unsigned char *input, uint len)
{
	uint i, j;

	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[i] = ((UIN)input[j]) | (((UIN)input[j + 1]) << 8) |
			(((UIN)input[j + 2]) << 16) | (((UIN)input[j + 3]) << 24);
	}
}

/* Note: Replace "for loop" with standard memcpy if possible.
*/
static void MD5_memcpy(POINTER output, POINTER input, uint len)
{
	uint i;

	for (i = 0; i < len; i++)
	{
		output[i] = input[i];
	}
}

/* Note: Replace "for loop" with standard memset if possible.
*/
static void MD5_memset(POINTER output, int value, uint len)
{
	uint i;

	for (i = 0; i < len; i++)
	{
		((char *)output)[i] = (char)value;
	}
}

void md5_calc(unsigned char *output, unsigned char *input, uint inlen)
{
	MD5_CTX context;

	MD5Init(&context);
	MD5Update(&context, input, inlen);
	MD5Final(output, &context);
}

void MD5Encrypt(signed char *strOutput, unsigned char *strInput)
{
	unsigned char outputtemp[16];
	unsigned char *input;

	input = strInput;

	md5_calc(outputtemp, input, strlen((char*)input));

	for (int i = 0; i <= 7; i++)
	{
		strOutput[i] = ( (outputtemp[2 * i] + outputtemp[2 * i + 1]) % 62 );

		if ((strOutput[i] >= 0) && (strOutput[i] <= 9))
		{
			strOutput[i] += 48;
		}
		else
		{
			if ((strOutput[i] >= 10) && (strOutput[i] <= 35))
			{
				strOutput[i] += 55;
			}
			else
			{
				strOutput[i] += 61;
			}
		}
	}
}

void MD5EncryptV2(signed char *strOutput, unsigned char *strInput, int mode)
{
	unsigned char outputtemp[16];
	unsigned char *input;
	unsigned char tempOut;
	
	input = strInput;
	md5_calc(outputtemp, input, strlen((char*)input));

	if((MD5_16_LOWERCASE == mode) || (MD5_16_UPPERCASE == mode))
	{
		for(int i = 0; i < 8; i++)
		{
			tempOut = outputtemp[i + 4];
			if(MD5_16_LOWERCASE == mode)
			{
				sprintf( (char*)(strOutput + i * 2), "%02x", tempOut);
			}
			else
			{
				sprintf( (char*)(strOutput + i * 2), "%02X", tempOut);
			}
		}
	}
	else if((MD5_32_LOWERCASE == mode) || (MD5_32_UPPERCASE == mode))
	{
		for(int i = 0; i < 16; i++)
		{
			tempOut = outputtemp[i];
			if(MD5_32_LOWERCASE == mode)
			{
				sprintf( (char*)(strOutput + i * 2), "%02x", tempOut);
			}
			else
			{
				sprintf( (char*)(strOutput + i * 2), "%02X", tempOut);
			}
		}
	}
}

signed char *
SuperPasswd(signed char *strOutput, int iOutLen, unsigned char *strInput)
{
	signed char EncStr_1[9];
	unsigned char EncStr_2[9];

	int ii;
	int kk;

	if (NULL == strOutput
		|| NULL == strInput)
	{
		return NULL;
	}

	MD5Encrypt(EncStr_1, strInput);
	EncStr_1[8] = '\0';

	//
	// chg to num
	//
	for (ii=0; ii<iOutLen; )
	{
		for (kk=0; kk<8; kk++)
		{
			if (isdigit(EncStr_1[kk]))
			{
				*(strOutput+ii) = EncStr_1[kk];

				ii++;

				if (ii>=iOutLen)
				{
					break;
				}
			}
		}

		memcpy(EncStr_2, EncStr_1, sizeof(signed char)*9);
		MD5Encrypt(EncStr_1, EncStr_2);
		EncStr_1[8] = '\0';
	} // for

	return strOutput;
}
//////////////////////////////////////////////////////////////////////////
/// Blowfish加解密算法
#include <string>

class CSCHelper
{
public:
	CSCHelper();
	~CSCHelper();

	std::string MakeSecurityCode(const char * lpszAdminAccount, const char * lpszAdminPassword);
	bool GetAccountFromCode(const char * lpszSecurityCode, std::string& strAccount, std::string& strPassword);
	bool Encrypt(const char *pSrc,char *pDst);
	bool Decrypt(const char *pSrc,char *pDst);
};

/*
http://www.counterpane.com/vectors.txt
Test vectors by Eric Young.  These tests all assume Blowfish with 16
rounds.

All data is shown as a hex string with 012345 loading as
data[0]=0x01;
data[1]=0x23;
data[2]=0x45;
ecb test data (taken from the DES validation tests)

key bytes               clear bytes             cipher bytes
0000000000000000        0000000000000000        4EF997456198DD78
FFFFFFFFFFFFFFFF        FFFFFFFFFFFFFFFF        51866FD5B85ECB8A
3000000000000000        1000000000000001        7D856F9A613063F2  ???
1111111111111111        1111111111111111        2466DD878B963C9D
0123456789ABCDEF        1111111111111111        61F9C3802281B096
1111111111111111        0123456789ABCDEF        7D0CC630AFDA1EC7
0000000000000000        0000000000000000        4EF997456198DD78
FEDCBA9876543210        0123456789ABCDEF        0ACEAB0FC6A0A28D
7CA110454A1A6E57        01A1D6D039776742        59C68245EB05282B
0131D9619DC1376E        5CD54CA83DEF57DA        B1B8CC0B250F09A0
07A1133E4A0B2686        0248D43806F67172        1730E5778BEA1DA4
3849674C2602319E        51454B582DDF440A        A25E7856CF2651EB
04B915BA43FEB5B6        42FD443059577FA2        353882B109CE8F1A
0113B970FD34F2CE        059B5E0851CF143A        48F4D0884C379918
0170F175468FB5E6        0756D8E0774761D2        432193B78951FC98
43297FAD38E373FE        762514B829BF486A        13F04154D69D1AE5
07A7137045DA2A16        3BDD119049372802        2EEDDA93FFD39C79
04689104C2FD3B2F        26955F6835AF609A        D887E0393C2DA6E3
37D06BB516CB7546        164D5E404F275232        5F99D04F5B163969
1F08260D1AC2465E        6B056E18759F5CCA        4A057A3B24D3977B
584023641ABA6176        004BD6EF09176062        452031C1E4FADA8E
025816164629B007        480D39006EE762F2        7555AE39F59B87BD
49793EBC79B3258F        437540C8698F3CFA        53C55F9CB49FC019
4FB05E1515AB73A7        072D43A077075292        7A8E7BFA937E89A3
49E95D6D4CA229BF        02FE55778117F12A        CF9C5D7A4986ADB5
018310DC409B26D6        1D9D5C5018F728C2        D1ABB290658BC778
1C587F1C13924FEF        305532286D6F295A        55CB3774D13EF201
0101010101010101        0123456789ABCDEF        FA34EC4847B268B2
1F1F1F1F0E0E0E0E        0123456789ABCDEF        A790795108EA3CAE
E0FEE0FEF1FEF1FE        0123456789ABCDEF        C39E072D9FAC631D
0000000000000000        FFFFFFFFFFFFFFFF        014933E0CDAFF6E4
FFFFFFFFFFFFFFFF        0000000000000000        F21E9A77B71C49BC
0123456789ABCDEF        0000000000000000        245946885754369A
FEDCBA9876543210        FFFFFFFFFFFFFFFF        6B5C5A9C5D9E0A5A

set_key test data
data[8]= FEDCBA9876543210
c=F9AD597C49DB005E k[ 1]=F0
c=E91D21C1D961A6D6 k[ 2]=F0E1
c=E9C2B70A1BC65CF3 k[ 3]=F0E1D2
c=BE1E639408640F05 k[ 4]=F0E1D2C3
c=B39E44481BDB1E6E k[ 5]=F0E1D2C3B4
c=9457AA83B1928C0D k[ 6]=F0E1D2C3B4A5
c=8BB77032F960629D k[ 7]=F0E1D2C3B4A596
c=E87A244E2CC85E82 k[ 8]=F0E1D2C3B4A59687
c=15750E7A4F4EC577 k[ 9]=F0E1D2C3B4A5968778
c=122BA70B3AB64AE0 k[10]=F0E1D2C3B4A596877869
c=3A833C9AFFC537F6 k[11]=F0E1D2C3B4A5968778695A
c=9409DA87A90F6BF2 k[12]=F0E1D2C3B4A5968778695A4B
c=884F80625060B8B4 k[13]=F0E1D2C3B4A5968778695A4B3C
c=1F85031C19E11968 k[14]=F0E1D2C3B4A5968778695A4B3C2D
c=79D9373A714CA34F k[15]=F0E1D2C3B4A5968778695A4B3C2D1E ???
c=93142887EE3BE15C k[16]=F0E1D2C3B4A5968778695A4B3C2D1E0F
c=03429E838CE2D14B k[17]=F0E1D2C3B4A5968778695A4B3C2D1E0F00
c=A4299E27469FF67B k[18]=F0E1D2C3B4A5968778695A4B3C2D1E0F0011
c=AFD5AED1C1BC96A8 k[19]=F0E1D2C3B4A5968778695A4B3C2D1E0F001122
c=10851C0E3858DA9F k[20]=F0E1D2C3B4A5968778695A4B3C2D1E0F00112233
c=E6F51ED79B9DB21F k[21]=F0E1D2C3B4A5968778695A4B3C2D1E0F0011223344
c=64A6E14AFD36B46F k[22]=F0E1D2C3B4A5968778695A4B3C2D1E0F001122334455
c=80C7D7D45A5479AD k[23]=F0E1D2C3B4A5968778695A4B3C2D1E0F00112233445566
c=05044B62FA52D080 k[24]=F0E1D2C3B4A5968778695A4B3C2D1E0F0011223344556677

chaining mode test data
key[16]   = 0123456789ABCDEFF0E1D2C3B4A59687
iv[8]     = FEDCBA9876543210
data[29]  = "7654321 Now is the time for " (includes trailing '\0')
data[29]  = 37363534333231204E6F77206973207468652074696D6520666F722000
cbc cipher text
cipher[32]= 6B77B4D63006DEE605B156E27403979358DEB9E7154616D959F1652BD5FF92CC
cfb64 cipher text cipher[29]= 
E73214A2822139CAF26ECF6D2EB9E76E3DA3DE04D1517200519D57A6C3 
ofb64 cipher text cipher[29]= 
E73214A2822139CA62B343CC5B65587310DD908D0C241B2263C2CF80DA

*/

class CBlowFish
{
public:
	//Block Structure
	struct SBlock
	{
		//Constructors
		SBlock(unsigned int l=0, unsigned int r=0) : m_uil(l), m_uir(r) {}
		//Copy Constructor
		SBlock(const SBlock& roBlock) : m_uil(roBlock.m_uil), m_uir(roBlock.m_uir) {}
		SBlock& operator^=(SBlock& b) { m_uil ^= b.m_uil; m_uir ^= b.m_uir; return *this; }
		unsigned int m_uil, m_uir;
	};
public:
	enum { ECB=0, CBC=1, CFB=2 };

	//Constructor - Initialize the P and S boxes for a given Key
	CBlowFish(unsigned char* ucKey, size_t n, const SBlock& roChain = SBlock(0UL,0UL));

	//Resetting the chaining block
	void ResetChain() { m_oChain = m_oChain0; }

	// Encrypt/Decrypt Buffer in Place
	void Encrypt(unsigned char* buf, size_t n, int iMode=ECB);
	void Decrypt(unsigned char* buf, size_t n, int iMode=ECB);

	// Encrypt/Decrypt from Input Buffer to Output Buffer
	void Encrypt(const unsigned char* in, unsigned char* out, size_t n, int iMode=ECB);
	void Decrypt(const unsigned char* in, unsigned char* out, size_t n, int iMode=ECB);

	//Private Functions
private:
	unsigned int FFF(unsigned int ui);
	void Encrypt(SBlock&);
	void Decrypt(SBlock&);

private:
	//The Initialization Vector, by default {0, 0}
	SBlock m_oChain0;
	SBlock m_oChain;
	unsigned int m_auiP[18];
	unsigned int m_auiS[4][256];
	static const unsigned int scm_auiInitP[18];
	static const unsigned int scm_auiInitS[4][256];
};

//Extract low order byte
inline unsigned char Byte(unsigned int ui)
{
	return (unsigned char)(ui & 0xff);
}

//Function F
unsigned int CBlowFish::FFF(unsigned int ui)
{
	return ((m_auiS[0][Byte(ui>>24)] + m_auiS[1][Byte(ui>>16)]) ^ m_auiS[2][Byte(ui>>8)]) + m_auiS[3][Byte(ui)];
}

//Initialization with a fixed string which consists of the hexadecimal digits of PI (less the initial 3)
//P-array, 18 32-bit subkeys
const unsigned int CBlowFish::scm_auiInitP[18] = {
	0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344,
	0xa4093822, 0x299f31d0, 0x082efa98, 0xec4e6c89,
	0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
	0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917,
	0x9216d5d9, 0x8979fb1b
};

//Four 32-bit S-boxes with 256 entries each
const unsigned int CBlowFish::scm_auiInitS[4][256] = {
	//0
	{0xd1310ba6, 0x98dfb5ac, 0x2ffd72db, 0xd01adfb7,
	0xb8e1afed, 0x6a267e96, 0xba7c9045, 0xf12c7f99,
	0x24a19947, 0xb3916cf7, 0x0801f2e2, 0x858efc16,
	0x636920d8, 0x71574e69, 0xa458fea3, 0xf4933d7e,
	0x0d95748f, 0x728eb658, 0x718bcd58, 0x82154aee,
	0x7b54a41d, 0xc25a59b5, 0x9c30d539, 0x2af26013,
	0xc5d1b023, 0x286085f0, 0xca417918, 0xb8db38ef,
	0x8e79dcb0, 0x603a180e, 0x6c9e0e8b, 0xb01e8a3e,
	0xd71577c1, 0xbd314b27, 0x78af2fda, 0x55605c60,
	0xe65525f3, 0xaa55ab94, 0x57489862, 0x63e81440,
	0x55ca396a, 0x2aab10b6, 0xb4cc5c34, 0x1141e8ce,
	0xa15486af, 0x7c72e993, 0xb3ee1411, 0x636fbc2a,
	0x2ba9c55d, 0x741831f6, 0xce5c3e16, 0x9b87931e,
	0xafd6ba33, 0x6c24cf5c, 0x7a325381, 0x28958677,
	0x3b8f4898, 0x6b4bb9af, 0xc4bfe81b, 0x66282193,
	0x61d809cc, 0xfb21a991, 0x487cac60, 0x5dec8032,
	0xef845d5d, 0xe98575b1, 0xdc262302, 0xeb651b88,
	0x23893e81, 0xd396acc5, 0x0f6d6ff3, 0x83f44239,
	0x2e0b4482, 0xa4842004, 0x69c8f04a, 0x9e1f9b5e,
	0x21c66842, 0xf6e96c9a, 0x670c9c61, 0xabd388f0,
	0x6a51a0d2, 0xd8542f68, 0x960fa728, 0xab5133a3,
	0x6eef0b6c, 0x137a3be4, 0xba3bf050, 0x7efb2a98,
	0xa1f1651d, 0x39af0176, 0x66ca593e, 0x82430e88,
	0x8cee8619, 0x456f9fb4, 0x7d84a5c3, 0x3b8b5ebe,
	0xe06f75d8, 0x85c12073, 0x401a449f, 0x56c16aa6,
	0x4ed3aa62, 0x363f7706, 0x1bfedf72, 0x429b023d,
	0x37d0d724, 0xd00a1248, 0xdb0fead3, 0x49f1c09b,
	0x075372c9, 0x80991b7b, 0x25d479d8, 0xf6e8def7,
	0xe3fe501a, 0xb6794c3b, 0x976ce0bd, 0x04c006ba,
	0xc1a94fb6, 0x409f60c4, 0x5e5c9ec2, 0x196a2463,
	0x68fb6faf, 0x3e6c53b5, 0x1339b2eb, 0x3b52ec6f,
	0x6dfc511f, 0x9b30952c, 0xcc814544, 0xaf5ebd09,
	0xbee3d004, 0xde334afd, 0x660f2807, 0x192e4bb3,
	0xc0cba857, 0x45c8740f, 0xd20b5f39, 0xb9d3fbdb,
	0x5579c0bd, 0x1a60320a, 0xd6a100c6, 0x402c7279,
	0x679f25fe, 0xfb1fa3cc, 0x8ea5e9f8, 0xdb3222f8,
	0x3c7516df, 0xfd616b15, 0x2f501ec8, 0xad0552ab,
	0x323db5fa, 0xfd238760, 0x53317b48, 0x3e00df82,
	0x9e5c57bb, 0xca6f8ca0, 0x1a87562e, 0xdf1769db,
	0xd542a8f6, 0x287effc3, 0xac6732c6, 0x8c4f5573,
	0x695b27b0, 0xbbca58c8, 0xe1ffa35d, 0xb8f011a0,
	0x10fa3d98, 0xfd2183b8, 0x4afcb56c, 0x2dd1d35b,
	0x9a53e479, 0xb6f84565, 0xd28e49bc, 0x4bfb9790,
	0xe1ddf2da, 0xa4cb7e33, 0x62fb1341, 0xcee4c6e8,
	0xef20cada, 0x36774c01, 0xd07e9efe, 0x2bf11fb4,
	0x95dbda4d, 0xae909198, 0xeaad8e71, 0x6b93d5a0,
	0xd08ed1d0, 0xafc725e0, 0x8e3c5b2f, 0x8e7594b7,
	0x8ff6e2fb, 0xf2122b64, 0x8888b812, 0x900df01c,
	0x4fad5ea0, 0x688fc31c, 0xd1cff191, 0xb3a8c1ad,
	0x2f2f2218, 0xbe0e1777, 0xea752dfe, 0x8b021fa1,
	0xe5a0cc0f, 0xb56f74e8, 0x18acf3d6, 0xce89e299,
	0xb4a84fe0, 0xfd13e0b7, 0x7cc43b81, 0xd2ada8d9,
	0x165fa266, 0x80957705, 0x93cc7314, 0x211a1477,
	0xe6ad2065, 0x77b5fa86, 0xc75442f5, 0xfb9d35cf,
	0xebcdaf0c, 0x7b3e89a0, 0xd6411bd3, 0xae1e7e49,
	0x00250e2d, 0x2071b35e, 0x226800bb, 0x57b8e0af,
	0x2464369b, 0xf009b91e, 0x5563911d, 0x59dfa6aa,
	0x78c14389, 0xd95a537f, 0x207d5ba2, 0x02e5b9c5,
	0x83260376, 0x6295cfa9, 0x11c81968, 0x4e734a41,
	0xb3472dca, 0x7b14a94a, 0x1b510052, 0x9a532915,
	0xd60f573f, 0xbc9bc6e4, 0x2b60a476, 0x81e67400,
	0x08ba6fb5, 0x571be91f, 0xf296ec6b, 0x2a0dd915,
	0xb6636521, 0xe7b9f9b6, 0xff34052e, 0xc5855664,
	0x53b02d5d, 0xa99f8fa1, 0x08ba4799, 0x6e85076a},

	//1
	{0x4b7a70e9, 0xb5b32944, 0xdb75092e, 0xc4192623,
	0xad6ea6b0, 0x49a7df7d, 0x9cee60b8, 0x8fedb266,
	0xecaa8c71, 0x699a17ff, 0x5664526c, 0xc2b19ee1,
	0x193602a5, 0x75094c29, 0xa0591340, 0xe4183a3e,
	0x3f54989a, 0x5b429d65, 0x6b8fe4d6, 0x99f73fd6,
	0xa1d29c07, 0xefe830f5, 0x4d2d38e6, 0xf0255dc1,
	0x4cdd2086, 0x8470eb26, 0x6382e9c6, 0x021ecc5e,
	0x09686b3f, 0x3ebaefc9, 0x3c971814, 0x6b6a70a1,
	0x687f3584, 0x52a0e286, 0xb79c5305, 0xaa500737,
	0x3e07841c, 0x7fdeae5c, 0x8e7d44ec, 0x5716f2b8,
	0xb03ada37, 0xf0500c0d, 0xf01c1f04, 0x0200b3ff,
	0xae0cf51a, 0x3cb574b2, 0x25837a58, 0xdc0921bd,
	0xd19113f9, 0x7ca92ff6, 0x94324773, 0x22f54701,
	0x3ae5e581, 0x37c2dadc, 0xc8b57634, 0x9af3dda7,
	0xa9446146, 0x0fd0030e, 0xecc8c73e, 0xa4751e41,
	0xe238cd99, 0x3bea0e2f, 0x3280bba1, 0x183eb331,
	0x4e548b38, 0x4f6db908, 0x6f420d03, 0xf60a04bf,
	0x2cb81290, 0x24977c79, 0x5679b072, 0xbcaf89af,
	0xde9a771f, 0xd9930810, 0xb38bae12, 0xdccf3f2e,
	0x5512721f, 0x2e6b7124, 0x501adde6, 0x9f84cd87,
	0x7a584718, 0x7408da17, 0xbc9f9abc, 0xe94b7d8c,
	0xec7aec3a, 0xdb851dfa, 0x63094366, 0xc464c3d2,
	0xef1c1847, 0x3215d908, 0xdd433b37, 0x24c2ba16,
	0x12a14d43, 0x2a65c451, 0x50940002, 0x133ae4dd,
	0x71dff89e, 0x10314e55, 0x81ac77d6, 0x5f11199b,
	0x043556f1, 0xd7a3c76b, 0x3c11183b, 0x5924a509,
	0xf28fe6ed, 0x97f1fbfa, 0x9ebabf2c, 0x1e153c6e,
	0x86e34570, 0xeae96fb1, 0x860e5e0a, 0x5a3e2ab3,
	0x771fe71c, 0x4e3d06fa, 0x2965dcb9, 0x99e71d0f,
	0x803e89d6, 0x5266c825, 0x2e4cc978, 0x9c10b36a,
	0xc6150eba, 0x94e2ea78, 0xa5fc3c53, 0x1e0a2df4,
	0xf2f74ea7, 0x361d2b3d, 0x1939260f, 0x19c27960,
	0x5223a708, 0xf71312b6, 0xebadfe6e, 0xeac31f66,
	0xe3bc4595, 0xa67bc883, 0xb17f37d1, 0x018cff28,
	0xc332ddef, 0xbe6c5aa5, 0x65582185, 0x68ab9802,
	0xeecea50f, 0xdb2f953b, 0x2aef7dad, 0x5b6e2f84,
	0x1521b628, 0x29076170, 0xecdd4775, 0x619f1510,
	0x13cca830, 0xeb61bd96, 0x0334fe1e, 0xaa0363cf,
	0xb5735c90, 0x4c70a239, 0xd59e9e0b, 0xcbaade14,
	0xeecc86bc, 0x60622ca7, 0x9cab5cab, 0xb2f3846e,
	0x648b1eaf, 0x19bdf0ca, 0xa02369b9, 0x655abb50,
	0x40685a32, 0x3c2ab4b3, 0x319ee9d5, 0xc021b8f7,
	0x9b540b19, 0x875fa099, 0x95f7997e, 0x623d7da8,
	0xf837889a, 0x97e32d77, 0x11ed935f, 0x16681281,
	0x0e358829, 0xc7e61fd6, 0x96dedfa1, 0x7858ba99,
	0x57f584a5, 0x1b227263, 0x9b83c3ff, 0x1ac24696,
	0xcdb30aeb, 0x532e3054, 0x8fd948e4, 0x6dbc3128,
	0x58ebf2ef, 0x34c6ffea, 0xfe28ed61, 0xee7c3c73,
	0x5d4a14d9, 0xe864b7e3, 0x42105d14, 0x203e13e0,
	0x45eee2b6, 0xa3aaabea, 0xdb6c4f15, 0xfacb4fd0,
	0xc742f442, 0xef6abbb5, 0x654f3b1d, 0x41cd2105,
	0xd81e799e, 0x86854dc7, 0xe44b476a, 0x3d816250,
	0xcf62a1f2, 0x5b8d2646, 0xfc8883a0, 0xc1c7b6a3,
	0x7f1524c3, 0x69cb7492, 0x47848a0b, 0x5692b285,
	0x095bbf00, 0xad19489d, 0x1462b174, 0x23820e00,
	0x58428d2a, 0x0c55f5ea, 0x1dadf43e, 0x233f7061,
	0x3372f092, 0x8d937e41, 0xd65fecf1, 0x6c223bdb,
	0x7cde3759, 0xcbee7460, 0x4085f2a7, 0xce77326e,
	0xa6078084, 0x19f8509e, 0xe8efd855, 0x61d99735,
	0xa969a7aa, 0xc50c06c2, 0x5a04abfc, 0x800bcadc,
	0x9e447a2e, 0xc3453484, 0xfdd56705, 0x0e1e9ec9,
	0xdb73dbd3, 0x105588cd, 0x675fda79, 0xe3674340,
	0xc5c43465, 0x713e38d8, 0x3d28f89e, 0xf16dff20,
	0x153e21e7, 0x8fb03d4a, 0xe6e39f2b, 0xdb83adf7},

	//2
	{0xe93d5a68, 0x948140f7, 0xf64c261c, 0x94692934,
	0x411520f7, 0x7602d4f7, 0xbcf46b2e, 0xd4a20068,
	0xd4082471, 0x3320f46a, 0x43b7d4b7, 0x500061af,
	0x1e39f62e, 0x97244546, 0x14214f74, 0xbf8b8840,
	0x4d95fc1d, 0x96b591af, 0x70f4ddd3, 0x66a02f45,
	0xbfbc09ec, 0x03bd9785, 0x7fac6dd0, 0x31cb8504,
	0x96eb27b3, 0x55fd3941, 0xda2547e6, 0xabca0a9a,
	0x28507825, 0x530429f4, 0x0a2c86da, 0xe9b66dfb,
	0x68dc1462, 0xd7486900, 0x680ec0a4, 0x27a18dee,
	0x4f3ffea2, 0xe887ad8c, 0xb58ce006, 0x7af4d6b6,
	0xaace1e7c, 0xd3375fec, 0xce78a399, 0x406b2a42,
	0x20fe9e35, 0xd9f385b9, 0xee39d7ab, 0x3b124e8b,
	0x1dc9faf7, 0x4b6d1856, 0x26a36631, 0xeae397b2,
	0x3a6efa74, 0xdd5b4332, 0x6841e7f7, 0xca7820fb,
	0xfb0af54e, 0xd8feb397, 0x454056ac, 0xba489527,
	0x55533a3a, 0x20838d87, 0xfe6ba9b7, 0xd096954b,
	0x55a867bc, 0xa1159a58, 0xcca92963, 0x99e1db33,
	0xa62a4a56, 0x3f3125f9, 0x5ef47e1c, 0x9029317c,
	0xfdf8e802, 0x04272f70, 0x80bb155c, 0x05282ce3,
	0x95c11548, 0xe4c66d22, 0x48c1133f, 0xc70f86dc,
	0x07f9c9ee, 0x41041f0f, 0x404779a4, 0x5d886e17,
	0x325f51eb, 0xd59bc0d1, 0xf2bcc18f, 0x41113564,
	0x257b7834, 0x602a9c60, 0xdff8e8a3, 0x1f636c1b,
	0x0e12b4c2, 0x02e1329e, 0xaf664fd1, 0xcad18115,
	0x6b2395e0, 0x333e92e1, 0x3b240b62, 0xeebeb922,
	0x85b2a20e, 0xe6ba0d99, 0xde720c8c, 0x2da2f728,
	0xd0127845, 0x95b794fd, 0x647d0862, 0xe7ccf5f0,
	0x5449a36f, 0x877d48fa, 0xc39dfd27, 0xf33e8d1e,
	0x0a476341, 0x992eff74, 0x3a6f6eab, 0xf4f8fd37,
	0xa812dc60, 0xa1ebddf8, 0x991be14c, 0xdb6e6b0d,
	0xc67b5510, 0x6d672c37, 0x2765d43b, 0xdcd0e804,
	0xf1290dc7, 0xcc00ffa3, 0xb5390f92, 0x690fed0b,
	0x667b9ffb, 0xcedb7d9c, 0xa091cf0b, 0xd9155ea3,
	0xbb132f88, 0x515bad24, 0x7b9479bf, 0x763bd6eb,
	0x37392eb3, 0xcc115979, 0x8026e297, 0xf42e312d,
	0x6842ada7, 0xc66a2b3b, 0x12754ccc, 0x782ef11c,
	0x6a124237, 0xb79251e7, 0x06a1bbe6, 0x4bfb6350,
	0x1a6b1018, 0x11caedfa, 0x3d25bdd8, 0xe2e1c3c9,
	0x44421659, 0x0a121386, 0xd90cec6e, 0xd5abea2a,
	0x64af674e, 0xda86a85f, 0xbebfe988, 0x64e4c3fe,
	0x9dbc8057, 0xf0f7c086, 0x60787bf8, 0x6003604d,
	0xd1fd8346, 0xf6381fb0, 0x7745ae04, 0xd736fccc,
	0x83426b33, 0xf01eab71, 0xb0804187, 0x3c005e5f,
	0x77a057be, 0xbde8ae24, 0x55464299, 0xbf582e61,
	0x4e58f48f, 0xf2ddfda2, 0xf474ef38, 0x8789bdc2,
	0x5366f9c3, 0xc8b38e74, 0xb475f255, 0x46fcd9b9,
	0x7aeb2661, 0x8b1ddf84, 0x846a0e79, 0x915f95e2,
	0x466e598e, 0x20b45770, 0x8cd55591, 0xc902de4c,
	0xb90bace1, 0xbb8205d0, 0x11a86248, 0x7574a99e,
	0xb77f19b6, 0xe0a9dc09, 0x662d09a1, 0xc4324633,
	0xe85a1f02, 0x09f0be8c, 0x4a99a025, 0x1d6efe10,
	0x1ab93d1d, 0x0ba5a4df, 0xa186f20f, 0x2868f169,
	0xdcb7da83, 0x573906fe, 0xa1e2ce9b, 0x4fcd7f52,
	0x50115e01, 0xa70683fa, 0xa002b5c4, 0x0de6d027,
	0x9af88c27, 0x773f8641, 0xc3604c06, 0x61a806b5,
	0xf0177a28, 0xc0f586e0, 0x006058aa, 0x30dc7d62,
	0x11e69ed7, 0x2338ea63, 0x53c2dd94, 0xc2c21634,
	0xbbcbee56, 0x90bcb6de, 0xebfc7da1, 0xce591d76,
	0x6f05e409, 0x4b7c0188, 0x39720a3d, 0x7c927c24,
	0x86e3725f, 0x724d9db9, 0x1ac15bb4, 0xd39eb8fc,
	0xed545578, 0x08fca5b5, 0xd83d7cd3, 0x4dad0fc4,
	0x1e50ef5e, 0xb161e6f8, 0xa28514d9, 0x6c51133c,
	0x6fd5c7e7, 0x56e14ec4, 0x362abfce, 0xddc6c837,
	0xd79a3234, 0x92638212, 0x670efa8e, 0x406000e0},

	//3
	{0x3a39ce37, 0xd3faf5cf, 0xabc27737, 0x5ac52d1b,
	0x5cb0679e, 0x4fa33742, 0xd3822740, 0x99bc9bbe,
	0xd5118e9d, 0xbf0f7315, 0xd62d1c7e, 0xc700c47b,
	0xb78c1b6b, 0x21a19045, 0xb26eb1be, 0x6a366eb4,
	0x5748ab2f, 0xbc946e79, 0xc6a376d2, 0x6549c2c8,
	0x530ff8ee, 0x468dde7d, 0xd5730a1d, 0x4cd04dc6,
	0x2939bbdb, 0xa9ba4650, 0xac9526e8, 0xbe5ee304,
	0xa1fad5f0, 0x6a2d519a, 0x63ef8ce2, 0x9a86ee22,
	0xc089c2b8, 0x43242ef6, 0xa51e03aa, 0x9cf2d0a4,
	0x83c061ba, 0x9be96a4d, 0x8fe51550, 0xba645bd6,
	0x2826a2f9, 0xa73a3ae1, 0x4ba99586, 0xef5562e9,
	0xc72fefd3, 0xf752f7da, 0x3f046f69, 0x77fa0a59,
	0x80e4a915, 0x87b08601, 0x9b09e6ad, 0x3b3ee593,
	0xe990fd5a, 0x9e34d797, 0x2cf0b7d9, 0x022b8b51,
	0x96d5ac3a, 0x017da67d, 0xd1cf3ed6, 0x7c7d2d28,
	0x1f9f25cf, 0xadf2b89b, 0x5ad6b472, 0x5a88f54c,
	0xe029ac71, 0xe019a5e6, 0x47b0acfd, 0xed93fa9b,
	0xe8d3c48d, 0x283b57cc, 0xf8d56629, 0x79132e28,
	0x785f0191, 0xed756055, 0xf7960e44, 0xe3d35e8c,
	0x15056dd4, 0x88f46dba, 0x03a16125, 0x0564f0bd,
	0xc3eb9e15, 0x3c9057a2, 0x97271aec, 0xa93a072a,
	0x1b3f6d9b, 0x1e6321f5, 0xf59c66fb, 0x26dcf319,
	0x7533d928, 0xb155fdf5, 0x03563482, 0x8aba3cbb,
	0x28517711, 0xc20ad9f8, 0xabcc5167, 0xccad925f,
	0x4de81751, 0x3830dc8e, 0x379d5862, 0x9320f991,
	0xea7a90c2, 0xfb3e7bce, 0x5121ce64, 0x774fbe32,
	0xa8b6e37e, 0xc3293d46, 0x48de5369, 0x6413e680,
	0xa2ae0810, 0xdd6db224, 0x69852dfd, 0x09072166,
	0xb39a460a, 0x6445c0dd, 0x586cdecf, 0x1c20c8ae,
	0x5bbef7dd, 0x1b588d40, 0xccd2017f, 0x6bb4e3bb,
	0xdda26a7e, 0x3a59ff45, 0x3e350a44, 0xbcb4cdd5,
	0x72eacea8, 0xfa6484bb, 0x8d6612ae, 0xbf3c6f47,
	0xd29be463, 0x542f5d9e, 0xaec2771b, 0xf64e6370,
	0x740e0d8d, 0xe75b1357, 0xf8721671, 0xaf537d5d,
	0x4040cb08, 0x4eb4e2cc, 0x34d2466a, 0x0115af84,
	0xe1b00428, 0x95983a1d, 0x06b89fb4, 0xce6ea048,
	0x6f3f3b82, 0x3520ab82, 0x011a1d4b, 0x277227f8,
	0x611560b1, 0xe7933fdc, 0xbb3a792b, 0x344525bd,
	0xa08839e1, 0x51ce794b, 0x2f32c9b7, 0xa01fbac9,
	0xe01cc87e, 0xbcc7d1f6, 0xcf0111c3, 0xa1e8aac7,
	0x1a908749, 0xd44fbd9a, 0xd0dadecb, 0xd50ada38,
	0x0339c32a, 0xc6913667, 0x8df9317c, 0xe0b12b4f,
	0xf79e59b7, 0x43f5bb3a, 0xf2d519ff, 0x27d9459c,
	0xbf97222c, 0x15e6fc2a, 0x0f91fc71, 0x9b941525,
	0xfae59361, 0xceb69ceb, 0xc2a86459, 0x12baa8d1,
	0xb6c1075e, 0xe3056a0c, 0x10d25065, 0xcb03a442,
	0xe0ec6e0e, 0x1698db3b, 0x4c98a0be, 0x3278e964,
	0x9f1f9532, 0xe0d392df, 0xd3a0342b, 0x8971f21e,
	0x1b0a7441, 0x4ba3348c, 0xc5be7120, 0xc37632d8,
	0xdf359f8d, 0x9b992f2e, 0xe60b6f47, 0x0fe3f11d,
	0xe54cda54, 0x1edad891, 0xce6279cf, 0xcd3e7e6f,
	0x1618b166, 0xfd2c1d05, 0x848fd2c5, 0xf6fb2299,
	0xf523f357, 0xa6327623, 0x93a83531, 0x56cccd02,
	0xacf08162, 0x5a75ebb5, 0x6e163697, 0x88d273cc,
	0xde966292, 0x81b949d0, 0x4c50901b, 0x71c65614,
	0xe6c6c7bd, 0x327a140a, 0x45e1d006, 0xc3f27b9a,
	0xc9aa53fd, 0x62a80f00, 0xbb25bfe2, 0x35bdd2f6,
	0x71126905, 0xb2040222, 0xb6cbcf7c, 0xcd769c2b,
	0x53113ec0, 0x1640e3d3, 0x38abbd60, 0x2547adf0,
	0xba38209c, 0xf746ce76, 0x77afa1c5, 0x20756060,
	0x85cbfe4e, 0x8ae88dd8, 0x7aaaf9b0, 0x4cf9aa7e,
	0x1948c25c, 0x02fb8a8c, 0x01c36ae4, 0xd6ebe1f9,
	0x90d4f869, 0xa65cdea0, 0x3f09252d, 0xc208e69f,
	0xb74e6132, 0xce77e25b, 0x578fdfe3, 0x3ac372e6}
};

//Constructor - Initialize the P and S boxes for a given Key
CBlowFish::CBlowFish(unsigned char* ucKey, size_t keysize, const SBlock& roChain) : m_oChain0(roChain), m_oChain(roChain)
{
	assert(keysize >= 1);
	//Check the Key - the key length should be between 1 and 56 bytes
	if(keysize>56)
		keysize = 56;
	unsigned char aucLocalKey[56];
	unsigned int i, j;
	memcpy(aucLocalKey, ucKey, keysize);
	//Reflexive Initialization of the Blowfish.
	//Generating the Subkeys from the Key flood P and S boxes with PI
	memcpy(m_auiP, scm_auiInitP, sizeof m_auiP);
	memcpy(m_auiS, scm_auiInitS, sizeof m_auiS);
	//Load P boxes with key bytes
	const unsigned char* p = aucLocalKey;
	unsigned int x=0;
	//Repeatedly cycle through the key bits until the entire P array has been XORed with key bits
	size_t iCount = 0;
	for(i=0; i<18; i++)
	{
		x=0;
		for(int n=4; n--; )
		{
			x <<= 8;
			x |= *(p++);
			iCount++;
			if(iCount == keysize)
			{
				//All bytes used, so recycle bytes 
				iCount = 0;
				p = aucLocalKey;
			}
		}
		m_auiP[i] ^= x;
	}
	//Reflect P and S boxes through the evolving Blowfish
	SBlock block(0UL,0UL); //all-zero block
	for(i=0; i<18; )
		Encrypt(block), m_auiP[i++] = block.m_uil, m_auiP[i++] = block.m_uir;
	for(j=0; j<4; j++)
		for(int k=0; k<256; )
			Encrypt(block), m_auiS[j][k++] = block.m_uil, m_auiS[j][k++] = block.m_uir;
}

//Sixteen Round Encipher of Block
void CBlowFish::Encrypt(SBlock& block)
{
	unsigned int uiLeft = block.m_uil;
	unsigned int uiRight = block.m_uir;
	uiLeft ^= m_auiP[0];
	uiRight ^= FFF(uiLeft)^m_auiP[1]; uiLeft ^= FFF(uiRight)^m_auiP[2];
	uiRight ^= FFF(uiLeft)^m_auiP[3]; uiLeft ^= FFF(uiRight)^m_auiP[4];
	uiRight ^= FFF(uiLeft)^m_auiP[5]; uiLeft ^= FFF(uiRight)^m_auiP[6];
	uiRight ^= FFF(uiLeft)^m_auiP[7]; uiLeft ^= FFF(uiRight)^m_auiP[8];
	uiRight ^= FFF(uiLeft)^m_auiP[9]; uiLeft ^= FFF(uiRight)^m_auiP[10];
	uiRight ^= FFF(uiLeft)^m_auiP[11]; uiLeft ^= FFF(uiRight)^m_auiP[12];
	uiRight ^= FFF(uiLeft)^m_auiP[13]; uiLeft ^= FFF(uiRight)^m_auiP[14];
	uiRight ^= FFF(uiLeft)^m_auiP[15]; uiLeft ^= FFF(uiRight)^m_auiP[16];
	uiRight ^= m_auiP[17];
	block.m_uil = uiRight;
	block.m_uir = uiLeft;
}

//Sixteen Round Decipher of SBlock
void CBlowFish::Decrypt(SBlock& block)
{
	unsigned int uiLeft = block.m_uil;
	unsigned int uiRight = block.m_uir;
	uiLeft ^= m_auiP[17];
	uiRight ^= FFF(uiLeft)^m_auiP[16]; uiLeft ^= FFF(uiRight)^m_auiP[15];
	uiRight ^= FFF(uiLeft)^m_auiP[14]; uiLeft ^= FFF(uiRight)^m_auiP[13];
	uiRight ^= FFF(uiLeft)^m_auiP[12]; uiLeft ^= FFF(uiRight)^m_auiP[11];
	uiRight ^= FFF(uiLeft)^m_auiP[10]; uiLeft ^= FFF(uiRight)^m_auiP[9];
	uiRight ^= FFF(uiLeft)^m_auiP[8]; uiLeft ^= FFF(uiRight)^m_auiP[7];
	uiRight ^= FFF(uiLeft)^m_auiP[6]; uiLeft ^= FFF(uiRight)^m_auiP[5];
	uiRight ^= FFF(uiLeft)^m_auiP[4]; uiLeft ^= FFF(uiRight)^m_auiP[3];
	uiRight ^= FFF(uiLeft)^m_auiP[2]; uiLeft ^= FFF(uiRight)^m_auiP[1];
	uiRight ^= m_auiP[0];
	block.m_uil = uiRight;
	block.m_uir = uiLeft;
}

//Semi-Portable Byte Shuffling
inline void BytesToBlock(unsigned char const* p, CBlowFish::SBlock& b)
{
	unsigned int y;
	//Left
	b.m_uil = 0;
	y = *p++;
	y <<= 24;
	b.m_uil |= y;
	y = *p++;
	y <<= 16;
	b.m_uil |= y;
	y = *p++;
	y <<= 8;
	b.m_uil |= y;
	y = *p++;
	b.m_uil |= y;
	//Right
	b.m_uir = 0;
	y = *p++;
	y <<= 24;
	b.m_uir |= y;
	y = *p++;
	y <<= 16;
	b.m_uir |= y;
	y = *p++;
	y <<= 8;
	b.m_uir |= y;
	y = *p++;
	b.m_uir |= y;
}

inline void BlockToBytes(CBlowFish::SBlock const& b, unsigned char* p)
{
	unsigned int y;
	//Right
	y = b.m_uir;
	*--p = Byte(y);
	y = b.m_uir >> 8;
	*--p = Byte(y);
	y = b.m_uir >> 16;
	*--p = Byte(y);
	y = b.m_uir >> 24;
	*--p = Byte(y);
	//Left
	y = b.m_uil;
	*--p = Byte(y);
	y = b.m_uil >> 8;
	*--p = Byte(y);
	y = b.m_uil >> 16;
	*--p = Byte(y);
	y = b.m_uil >> 24;
	*--p = Byte(y);
}

//Encrypt Buffer in Place
//Returns false if n is multiple of 8
void CBlowFish::Encrypt(unsigned char* buf, size_t n, int iMode)
{
	assert(n && (n % 8 == 0));
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
	{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8)
		{
			BytesToBlock(buf, work);
			work ^= chain;
			Encrypt(work);
			chain = work;
			BlockToBytes(work, buf+=8);
		}
	}
	else if(iMode == CFB) //CFB mode, using the Chain
	{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8)
		{
			Encrypt(chain);
			BytesToBlock(buf, work);
			work ^= chain;
			chain = work;
			BlockToBytes(work, buf+=8);
		}
	}
	else //ECB mode, not using the Chain
	{
		for(; n >= 8; n -= 8)
		{
			BytesToBlock(buf, work);
			Encrypt(work);
			BlockToBytes(work, buf+=8);
		}
	}
}

//Decrypt Buffer in Place
//Returns false if n is multiple of 8
void CBlowFish::Decrypt(unsigned char* buf, size_t n, int iMode)
{
	assert(n && (n % 8 == 0));
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
	{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8)
		{
			BytesToBlock(buf, work);
			crypt = work;
			Decrypt(work);
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, buf+=8);
		}
	}
	else if(iMode == CFB) //CFB mode, using the Chain, not using Decrypt()
	{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8)
		{
			BytesToBlock(buf, work);
			Encrypt(chain);
			crypt = work;
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, buf+=8);
		}
	}
	else //ECB mode, not using the Chain
	{
		for(; n >= 8; n -= 8)
		{
			BytesToBlock(buf, work);
			Decrypt(work);
			BlockToBytes(work, buf+=8);
		}
	}
}

//Encrypt from Input Buffer to Output Buffer
//Returns false if n is multiple of 8
void CBlowFish::Encrypt(const unsigned char* in, unsigned char* out, size_t n, int iMode)
{
	//Check the buffer's length - should be > 0 and multiple of 8
	assert(n && (n % 8 == 0));
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
	{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
		{
			BytesToBlock(in, work);
			work ^= chain;
			Encrypt(work);
			chain = work;
			BlockToBytes(work, out+=8);
		}
	}
	else if(iMode == CFB) //CFB mode, using the Chain
	{
		SBlock chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
		{
			Encrypt(chain);
			BytesToBlock(in, work);
			work ^= chain;
			chain = work;
			BlockToBytes(work, out+=8);
		}
	}
	else //ECB mode, not using the Chain
	{
		for(; n >= 8; n -= 8, in += 8)
		{
			BytesToBlock(in, work);
			Encrypt(work);
			BlockToBytes(work, out+=8);
		}
	}
}

//Decrypt from Input Buffer to Output Buffer
//Returns false if n is multiple of 8
void CBlowFish::Decrypt(const unsigned char* in, unsigned char* out, size_t n, int iMode)
{
	//Check the buffer's length - should be > 0 and multiple of 8
	assert(n && (n % 8 == 0));
	SBlock work;
	if(iMode == CBC) //CBC mode, using the Chain
	{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
		{
			BytesToBlock(in, work);
			crypt = work;
			Decrypt(work);
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, out+=8);
		}
	}
	else if(iMode == CFB) //CFB mode, using the Chain, not using Decrypt()
	{
		SBlock crypt, chain(m_oChain);
		for(; n >= 8; n -= 8, in += 8)
		{
			BytesToBlock(in, work);
			Encrypt(chain);
			crypt = work;
			work ^= chain;
			chain = crypt;
			BlockToBytes(work, out+=8);
		}
	}
	else //ECB mode, not using the Chain
	{
		for(; n >= 8; n -= 8, in += 8)
		{
			BytesToBlock(in, work);
			Decrypt(work);
			BlockToBytes(work, out+=8);
		}
	}
}

static bool SC_TO_DC(const char * lpszSrc, std::string& strDst, int nLen)
{
	strDst.clear();
	for(int i = 0; i < nLen; ++i)
	{
		uchar byte[2];
		memset(byte, 0, 2);
		uchar bt = *(uchar *)(lpszSrc +i);
		byte[0] = bt / 16;
		byte[1] = bt % 16;
		for(int j=0; j<2; j++)
		{
			assert(byte[j] < 16);
			if(byte[j] >= 0 && byte[j] <= 9)
			{
				uchar tmp = ('0' + byte[j]);
				strDst += tmp;
			}	
			else
			{
				uchar tmp = ('A' + byte[j] - 10);
				strDst += tmp;
			}
		}
	}
	return true;
}
static inline bool DC_TO_SC(const std::string& strSrc, std::string& strDst)
{
	assert(strSrc.length() % 2 ==0);
	if(strSrc.length() % 2 !=0) return false;

	strDst.clear();

	for(size_t i = 0; i < strSrc.length() / 2; ++i)
	{
		char ch = 0;
		for(int j = 0; j < 2; ++j)
		{
			char _ch = strSrc[i * 2 + j];
			if(_ch >= '0' && _ch <= '9')
			{
				ch = (ch << 4) + _ch - '0';
			}
			else if(_ch >= 'A' && _ch <= 'F')
			{
				ch = (ch << 4) + _ch - 'A' + 10;
			}
			else
			{
				//		bRes = true;
				//		break;
				return false;
			}
		}
		//		if ( bRes )
		//		{
		//			break;
		//		}
		strDst += ch;
	}
	return true;
}

static unsigned char sc_key[8] = {1, 9, 7, 9, 1, 1, 1, 2};
static unsigned char sd_key[8] = {2, 1, 1, 1, 9, 7, 9, 1};


//加密
bool EncryptString(const std::string& strSrc, std::string& strDst, bool bAccOrPas)
{
	size_t nLen = strSrc.length();

	nLen >>= 3;
	nLen <<= 3;
	nLen += 8;
	char *pSrc = new char[nLen + 1];
	memset(pSrc, 0, nLen + 1);
	strcpy(pSrc, strSrc.c_str());

	char *pDst = new char[nLen + 1];
	memset(pDst, 0, nLen + 1);

	try{
		//		CBlowFish oBlowFish((uchar *)lpszKey, _tcslen(lpszKey));
		CBlowFish oBlowFish(bAccOrPas ? sc_key : sd_key, 8);
		oBlowFish.Encrypt((uchar *)pSrc, (uchar *)pDst, nLen);
	}catch (...) {
		delete[] pSrc;
		delete[] pDst;
		return false;
	}

	SC_TO_DC(pDst, strDst, nLen);

	delete[] pSrc;
	delete[] pDst;

	return true;
}

//解密
bool DecryptString(const std::string& strSrc, std::string& strDst, bool bAccOrPas)
{
	std::string strTemp;
	if(!DC_TO_SC(strSrc, strTemp))
	{
		return false;
	}

	size_t nLen = strTemp.length();
	//ASSERT(nLen % 8 == 0);
	if(nLen % 8 != 0) return false;

	char *pSrc = new char[nLen + 1];
	memcpy(pSrc, strTemp.c_str(), nLen +1 );

	char *pDst = new char[nLen + 1];
	memset(pDst, 0, nLen + 1);

	try{
		//		CBlowFish oBlowFish((uchar *)lpszKey, _tcslen(lpszKey));
		CBlowFish oBlowFish(bAccOrPas ? sc_key : sd_key, 8);
		oBlowFish.Decrypt((uchar *)pSrc, (uchar *)pDst, nLen);
	} catch (...) {
		delete[] pSrc;
		delete[] pDst;
		return false;
	}

	strDst = pDst;

	delete[] pSrc;
	delete[] pDst;

	return true;
}

//加密
bool BlowfishEncrypt(const char *sSrc,char *sDst, bool bAccOrPas)
{
	bool bResult = false;
	std::string strSrc;
	std::string strDst;

	strSrc = sSrc;
	bResult =  EncryptString(strSrc,strDst,bAccOrPas);
	if (bResult)
	{
		strcpy(sDst,strDst.c_str());		
	}
	return bResult;
}

//解密
bool BlowfishDecrypt(const char *sSrc,char *sDst, bool bAccOrPas)
{
	bool bResult = false;
	std::string strSrc;
	std::string strDst;

	strSrc = sSrc;
	bResult = DecryptString(strSrc,strDst,bAccOrPas);
	if (bResult)
	{
		strcpy(sDst,strDst.c_str());		
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSCHelper::CSCHelper(){}
CSCHelper::~CSCHelper(){}

//////////////////////////////////////////////////////////////////////////
// Interface                                                                     
//////////////////////////////////////////////////////////////////////////

std::string CSCHelper::MakeSecurityCode(
									const char * lpszAdminAccount, 
									const char * lpszAdminPassword)
{
	char szAccount[/*8*/16];
	char szPassword[/*8*/16];

	memset(szAccount, 0, sizeof(szAccount));
	memset(szPassword, 0, sizeof(szPassword));

	if(lpszAdminAccount)
	{
		strcpy(szAccount, lpszAdminAccount);
	}

	if(lpszAdminPassword)
	{
		strcpy(szPassword, lpszAdminPassword);
	}

	std::string strAccount;
	std::string strPassword;
	EncryptString(szAccount, strAccount, true);
	EncryptString(szPassword, strPassword, false);

	return strAccount + strPassword;
}

bool CSCHelper::GetAccountFromCode(
								   const char * lpszSecurityCode, 
								   std::string& strAccount, 
								   std::string& strPassword)
{
	if(!lpszSecurityCode) return false;
	size_t nLen = strlen(lpszSecurityCode);
	if(nLen <= 16) return false;

	char szAccount[17];
	char szPassword[17];

	memset(szAccount, 0, sizeof(szAccount));
	memset(szPassword, 0, sizeof(szPassword));
	memcpy(szAccount, lpszSecurityCode, 16);
	memcpy(szPassword, lpszSecurityCode + 16, 16);
	//	std::string strAccount;
	//	std::string strPassword;
	DecryptString(szAccount, strAccount, true);
	DecryptString(szPassword, strPassword, false);

	return true;
}
bool CSCHelper::Encrypt(const char *pSrc,char *pDst)
{
	std::string strDst;
	bool bReturn = EncryptString(pSrc, strDst, true);
	if(bReturn)
	{
		strcpy(pDst, strDst.c_str());
	}
	return bReturn;
}
bool CSCHelper::Decrypt(const char *pSrc,char *pDst)
{
	std::string strDst;
	bool bReturn = DecryptString(pSrc, strDst, true);
	if(bReturn)
	{
		strcpy(pDst, strDst.c_str());
	}
	return bReturn;
}
