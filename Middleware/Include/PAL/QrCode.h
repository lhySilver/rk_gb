#ifndef __PAL_QRCODE_H__
#define __PAL_QRCODE_H__

#ifdef __cplusplus
extern "C" {
#endif

int QrcodeInit();
int QrcodeRelease();

typedef struct tagQRCODE_FORMAT
{
	unsigned int	Width;
	unsigned int	Height ;
	int 	EncodeType;
} QRCODE_FORMAT;


typedef int (* QrCodeDecodeCallback)(char *pYUVBuffer, int bufferLen,int w,int h);

void QrCodeSetCallback(QrCodeDecodeCallback cb);

int QrCodeCreate(QrCodeDecodeCallback cb);

int QrCodeDestory(void);

int QrCodeStart();

int QrCodeStop();

int QrCodePutBuffer(unsigned char* pBuffer, int bufoffset);

int QrCodeGetBuffer(unsigned char* pBuffer, unsigned int* pdwCount);



#ifdef __cplusplus
}
#endif

#endif
