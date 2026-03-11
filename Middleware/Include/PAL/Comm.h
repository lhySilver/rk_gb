#ifndef __PAL_COMM_H__
#define __PAL_COMM_H__

#include "Types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct COMM_ATTR
{
	unsigned int	baudrate;	//
	unsigned char	databits;	//
	unsigned char	parity; 	//
	unsigned char	stopbits;	//
	unsigned char	reserved;	//
} COMM_ATTR;

enum comm_stopbits_t 
{
	COMM_ONESTOPBIT= 1,	///< 1 stop bit
	COMM_ONE5STOPBITS,	///< 1.5 stop bit
	COMM_TWOSTOPBITS	///< 2 stop bit
};

enum comm_parity_t 
{
	COMM_NOPARITY,	///< No parity
	COMM_ODDPARITY, ///< Odd parity
	COMM_EVENPARITY,///< Even parity
	COMM_MARK,		///< 
	COMM_SPACE		///< 
};

int CommCreate(void);
int CommOpen(void);
int CommDestory(void);
int CommClose(void);
int CommSetAttribute(COMM_ATTR *pattr);
int CommRead(void *pdata, int nbytes);
int CommWrite(void *pdata, int len);

#ifdef __cplusplus
}
#endif

#endif 
