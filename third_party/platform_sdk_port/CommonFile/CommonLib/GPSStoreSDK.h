#ifndef GPS_STORE_SDK_HEAD
#define GPS_STORE_SDK_HEAD

#ifdef WIN32

#ifdef GPSSTORE_EXPORTS				//动态库编译时用此宏定义
#define GPSSTORE_API extern "C" __declspec(dllexport)
#else
#ifdef GPSSTORE_EXPORTS_STATIC		//静态库编译和使用静态库都用此宏定义
#define GPSSTORE_API extern "C"
#else								//动态库使用时不用任何宏定义
#define GPSSTORE_API extern "C" __declspec(dllimport)
#endif
#endif

#define STDCALL __stdcall

#else
#define GPSSTORE_API __attribute__ ((visibility("default")))
#define STDCALL

#endif //WIN32

class CGPSStreamCtrl
{
public:
	CGPSStreamCtrl(void) {}
	virtual ~CGPSStreamCtrl(void) {}
	virtual BOOL OpenGPSStream(tuint32 dwDeviceID, DWORD dwStartTime) = 0;
	virtual BOOL CloseGPSStream(tuint32 dwDeviceID) = 0;
};

GPSSTORE_API BOOL GPSSTORE_Initial(CGPSStreamCtrl *pGPSStreamCtrl);
GPSSTORE_API void GPSSTORE_Quit(void);
GPSSTORE_API BOOL GPSSTORE_Start(void);
GPSSTORE_API void GPSSTORE_Stop(void);
GPSSTORE_API DWORD GPSSTORE_GetLastError(void);

GPSSTORE_API BOOL GPSSTORE_AddSpace(const char *szPath);
GPSSTORE_API BOOL GPSSTORE_DelSpace(const char *szPath);

GPSSTORE_API void GPSSTORE_OpenDeviceGPS(DWORD dwDeviceID);
GPSSTORE_API void GPSSTORE_CloseDeviceGPS(DWORD dwDeviceID);

GPSSTORE_API BOOL GPSSTORE_StartTask(const GUID &TaskGUID, DWORD dwDeviceID, DWORD dwStartTime, DWORD dwEndTime);
GPSSTORE_API void GPSSTORE_StopTask(const GUID &TaskGUID);

GPSSTORE_API int GPSSTORE_GetNextFrame(const GUID &TaskGUID, char *pBuf, int nLen);
GPSSTORE_API BOOL GPSSTORE_StreamInput(DWORD dwDeviceID, const void *pBuf, int nLen, DWORD dwTime);

GPSSTORE_API BOOL GPSSTORE_FormatDiskSpace(const char *szPath, const GUID &SpaceGUID);
GPSSTORE_API BOOL GPSSTORE_IsFormat(const char *szPath, GUID &SpaceGUID);

GPSSTORE_API BOOL GPSSTORE_FormatGPSArea(const char *szPath, int nGPSPercent);
GPSSTORE_API int GPSSTORE_GetGPSPercent(const char *szPath);
#endif //GPS_STORE_SDK_HEAD
