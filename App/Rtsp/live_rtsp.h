#ifndef _LIVE_RTSP_H
#define _LIVE_RTSP_H

void StopRtspPthread(void);
int StartRtspPthread(void);
int RtspPutFrame_Main(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen);
int RtspPutFrame_Sub(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen);
int RtspPutFrame_Audio(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen);
int RtspPutFrame_Audio1(int iFrmType, unsigned long long *pullTimestamp, char *pData, int iDataLen);

#endif
