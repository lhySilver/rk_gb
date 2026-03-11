#ifndef _AVMANAGERH_
#define _AVMANAGERH_


#include "Common.h"

// class CAVManager : public CThread
class CAVManager
{
public:
	CAVManager();
	~CAVManager();

	static CAVManager* instance();

	///	视频
	bool VideoInit();
	bool VideoDeInit();

	///	音频
	bool AudioInit();
	bool AudioDeInit();
	/// 播放
	bool PlayVoice(uchar *data, int length, int type, int format);

	//AV
	bool RealTimeStreamStart(int media_type, dmc_media_input_fn proc);
	bool RealTimeStreamStop(int media_type);
	bool RecordStreamStart(int media_type, dmc_media_input_fn proc);
	bool RecordStreamStop(int media_type);

	// void ThreadProc();

private:

	int		m_iChannel;
	uint	m_dwBytes;
	uint	m_dwBytesOld;
	uint	m_dwBytesMask;
	uint	m_Time;
	int     m_iUser;
	bool    m_init;
	bool    m_audioInit;
	bool    m_videInit;

	CMutex m_Mutex;

	static CAVManager* _instance;
};

#define g_AVManager (*CAVManager::instance())

#endif
