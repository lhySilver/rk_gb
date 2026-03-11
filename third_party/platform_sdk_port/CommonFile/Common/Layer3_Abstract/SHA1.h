//sha1.h：对字符串进行sha1加密
#ifndef __SHA1_H__
#define __SHA1_H__

#undef BIG_ENDIAN_HOST
typedef unsigned int u32;

/****************
 * Rotate a 32 bit integer by n bytes
 ****************/
#if defined(__GNUC__) && defined(__i386__)
static inline u32 rol( u32 x, int n)
{
	__asm__("roll %%cl,%0"
            :"=r" (x)
            :"0" (x),"c" (n));
	return x;
}
#else
#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )
#endif

#define  SHA1_RESULT_LEN (20)

typedef struct {
	u32  h0,h1,h2,h3,h4;
	u32  nblocks;
	unsigned char buf[64];
	int  count;
} SHA1_CONTEXT;

void sha1_init( SHA1_CONTEXT *hd );
void transform( SHA1_CONTEXT *hd, unsigned char *data );
void sha1_write( SHA1_CONTEXT *hd, unsigned char *inbuf, size_t inlen);
void sha1_final(SHA1_CONTEXT *hd);

void PUB_SHA1Encrypt(const void *pIn, int iLenIn, char *szOut);
#endif
