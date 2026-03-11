#include "Storage.h"
#include <assert.h>
#include <unistd.h>

#include "DiskManager.h"

static bool s_bMuteRecord = false;
static int onRecord_fn(int media_chn,
        int media_type,
        int media_subtype,
        unsigned long long frame_pts,
        unsigned char *frame_data,
        int frame_len,
        int frame_end_flag)
{
	int ret = 0;
	unsigned char *pFrmBuff = frame_data;
	unsigned int uiFrmSize = (unsigned int)frame_len;
	unsigned long long ullTimeStamp = frame_pts;
	stream_data_header_S stStreamDataHeader = {0};
	
	// printf("----------------------------->onRecord, stream %d, size = %d, ret = %d\n", media_chn, uiFrmSize, ret);

	if (DMC_MEDIA_VIDEO_MAIN_STREAM == media_chn)
	{
		{
			stStreamDataHeader.iStreamType = 1;

			if (media_subtype == DMC_MEDIA_SUBTYPE_IFRAME)
				stStreamDataHeader.iFrameType = 1;
			else
				stStreamDataHeader.iFrameType = 2;
			
			stStreamDataHeader.ullTimestamp = ullTimeStamp;
					
			g_StorageManager->WriteFrameData((unsigned char*)pFrmBuff, uiFrmSize, (void*)&stStreamDataHeader);

			// printf("----------------------------->main, stream %d, size = %d, ret = %d\n", media_chn, uiFrmSize, ret);	
		}
	}
	else if  (DMC_MEDIA_VIDEO_SUB_STREAM == media_chn)
	{
	
	}
	else if  (DMC_MEDIA_AUDIO_FRIST_STREAM == media_chn)
	{
	
	}
	else if (DMC_MEDIA_AUDIO_SECOND_STREAM == media_chn)
	{
		if (!s_bMuteRecord)
		{
			stStreamDataHeader.iStreamType = 2;

			stStreamDataHeader.ullTimestamp = ullTimeStamp;
			
			g_StorageManager->WriteFrameData((unsigned char*)pFrmBuff, uiFrmSize, (void*)&stStreamDataHeader);

			// printf("----------------------------->audio, stream %d, size = %d, ullTimeStamp = %lld ret = %d\n", media_chn, uiFrmSize, ullTimeStamp,ret);
		}
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/// 录像管理类
PATTERN_SINGLETON_IMPLEMENT(CRecordManager);

CRecordManager::CRecordManager() : CThread("RecordManager", TP_REC), m_RecordCtrlMutex(MUTEX_RECURSIVE)
{	
	m_started = false;
}

CRecordManager::~CRecordManager()
{
	g_configManager.detach(getConfigName(CFG_RECORD), IConfigManager::Proc(&CRecordManager::onConfigRecord, this));
}

bool CRecordManager::Start()
{
	int iRet = 0;
	CConfigTable tableGeneral, tableEncode;

	printf("CRecordManager::start()\n");
	g_configManager.attach(getConfigName(CFG_RECORD), IConfigManager::Proc(&CRecordManager::onConfigRecord, this));

	CConfigTable tableRecord;
	struct RecordConfig &config = m_cCfgRecord.vRecordConfigAll[0];
	g_configManager.getConfig(getConfigName(CFG_RECORD),tableRecord);
	TExchangeAL<RecordConfigAll>::getConfigV2(tableRecord, m_cCfgRecord, 1);
	
	printf("config.iRecordMode		= %d\n", config.iRecordMode);

	s_bMuteRecord = config.bMuteRecord;

	if (config.bRecordEnable)
	{
		switch(config.iRecordMode)
		{
			case RECORD_MODE_FULLTIME:
				m_recMode = REC_TIM;
				startRecord(m_recMode, 0);
				break;
			case RECORD_MODE_ALARM:
				m_recMode = REC_ALM;
				startRecord(m_recMode, 0);
				break;
		}
	}
	g_AVManager.VideoInit();
	g_AVManager.RecordStreamStart(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO, onRecord_fn);

	return true;
}

// 录像总路数为本地编码路数+网络视频传送的路数
int CRecordManager::getChannels()
{
	return 1;
}

bool CRecordManager::Stop()
{
	CGuard l_cGuard(m_RecordCtrlMutex);
	AppInfo("CRecordManager::stop()\n");
	m_started = false;

	g_AVManager.RecordStreamStop(DMC_MEDIA_TYPE_H264 | DMC_MEDIA_TYPE_H265 | DMC_MEDIA_TYPE_AUDIO);

	g_configManager.detach(getConfigName(CFG_RECORD), IConfigManager::Proc(&CRecordManager::onConfigRecord, this));

	return TRUE;
}

void CRecordManager::ThreadProc()
{
	while (m_bLoop) 
	{
		CTime::sleep(200);
	}
}

uint CRecordManager::getState(ulong mode)
{
	return m_recMode;
}

/*!
	\b Description		:	开始录像\n
	\b Argument			:	Param arg, int channel
	\param	arg			:	录像模式(预录像、手工录像...)
	\param	channel		:	录像的通道数
	\return	以掩码表示成功的通道

	\b Revisions		:	
*/
uint CRecordManager::startRecord(Param arg, int iChannel)
{
	g_StorageManager->StartRec(arg);
	return 0;
}

//手工开始录象,跟配置无关,配置改动后,效果消失
void CRecordManager::StartRecordManual(int iChannel)
{
	// m_cCfgRecord.vRecordConfigAll[iChannel].iRecordMode = RECORD_MODE_MANUAL;
	// startRecord(REC_MAN,  iChannel);
}

//手工停止录象,跟配置无关,配置改动后,效果消失
void CRecordManager::StopRecordManual(int iChannel)
{
	// m_cCfgRecord.vRecordConfigAll[iChannel].iRecordMode = RECORD_MODE_CLOSED;
	// CGuard l_cGuard(m_RecordCtrlMutex);
	// g_StorageManager->StopRec();
}


//触发报警录像
void CRecordManager::DoRecord(int iChannel)
{
#ifdef RC0240_LGV10
	AppErr("[record]---do lg record...\n");
	g_StorageManager->DoAlarmRecord();
#else
	AppErr("[record]---do record...\n");
	if (m_recMode == REC_ALM) 
	{
		g_StorageManager->DoAlarmRecord();
	}
#endif
}

//消除报警录像
void CRecordManager::ClearRecord(int iChannel)
{
#ifdef RC0240_LGV10
	AppErr("[record]---clear lg record...\n");
	g_StorageManager->ClearAlarmRecord();
#else
	AppErr("[record]---clear record...\n");
	if (m_recMode == REC_ALM) 
	{
		g_StorageManager->ClearAlarmRecord();
	}
#endif
}

/*!
	\b Description		:	停止录像\n
	\b Argument			:	Param arg, int channel
	\param	arg			:	录像模式
	\param	channel		:	录像的通道数
	\return	以掩码表示成功的通道	

	\b Revisions		:	
*/
uint CRecordManager::stopRecord(Param arg, int iChannel, std::string strCode, bool bNotify)
{
	g_StorageManager->StopRec();
	return 0;
}

void CRecordManager::onConfigRecord(const CConfigTable& table, int& iRet)
{
	CGuard l_cGuard(m_RecordCtrlMutex);
	TStrWrapper<RecordConfigAll> pCfgReocrd;
	TExchangeAL<RecordConfigAll>::getConfigV2(table, *pCfgReocrd, 1);

	{
		RecordConfig& configOld = m_cCfgRecord.vRecordConfigAll[0];
		RecordConfig& configNew = pCfgReocrd->vRecordConfigAll[0];

		AppInfo("configOld.iRecordMode=%d\n",configOld.iRecordMode);
		AppInfo("configNew.iRecordMode=%d\n",configNew.iRecordMode);

		if (configOld.bMuteRecord != configNew.bMuteRecord)
		{
			s_bMuteRecord = configNew.bMuteRecord;
		}
		
		if (configOld.iRecordMode != configNew.iRecordMode)
		{
			switch(configNew.iRecordMode)
			{
				case RECORD_MODE_FULLTIME:
					stopRecord(REC_CLS);
					m_recMode = REC_TIM;
					startRecord(m_recMode, 0);
					break;
				case RECORD_MODE_ALARM:
					stopRecord(REC_CLS);
					m_recMode = REC_ALM;
					startRecord(m_recMode, 0);
					break;
			}
		}
		
		if (configOld.bRecordEnable != configNew.bRecordEnable)
		{
			if( configNew.bRecordEnable )
			{
				if ( RECORD_MODE_FULLTIME == configOld.iRecordMode )
					m_recMode = REC_TIM;
				else if ( RECORD_MODE_ALARM == configOld.iRecordMode )
					m_recMode = REC_ALM;

				startRecord(m_recMode, 0);
			}
			else
			{
				m_recMode = REC_CLS;//关闭
				stopRecord(REC_CLS);
			}
		}
		
		configOld = configNew;
	}
}

uint CRecordManager::saveRecordConfig()
{
	CConfigTable table;

	TExchangeAL<RecordConfigAll>::setConfigV2(m_cCfgRecord, table, 1);
	return g_configManager.setConfig(getConfigName(CFG_RECORD), table);
}
