
#ifndef __INTER_AUDIOMANAGER_H__
#define __INTER_AUDIOMANAGER_H__

#include "Common.h"

class IAudioManager
{
public:
	
	///音频输出模式
	///优先级由高到低:语音对讲->语音提示->回放->监视
	enum AudioOutType
	{
		AUDIO_TALK_TYPE = 0,	//语音对讲
		AUDIO_TIP_TYPE, 		//语音提示
		AUDIO_PLAY_TYPE,		//回放
		AUDIO_MONITOR_TYPE, 	//监视
		AUDIO_TYPE_NR
	};

	///音频数据源类型
	enum AUDIO_SRC_T
	{
		aSrcMute = 0, /*静音*/
		aSrcAudioIn, /*本地音频输入，如模拟通道预览音频*/
		aSrcDecAVMix, /*通过视频解码通道获取音频数据，如回放或者数字通道音频*/
		aSrcRaw,	  /*外部调用PlayVoice发送单一音频数据，如对讲，或提示*/
	};

	static IAudioManager* instance();

	///开启语音输出
	///iAudioType[in]	:  音频输出模式
	///iChannel[in]	:  对应iAudioType下的通道
	///iAssist[in] :	仅在iAudioType==AUDIO_MONITOR_TYPE && srcType == aSrcDecAVMix时有用
	///					表示对应的预览通道
	///srcType[in]	:  音频数据源的类型
	///返回值: 开启成功返回TRUE，失败FALSE
	virtual bool StartAudioOut(AudioOutType iAudioType) = 0;

	///停止音频输出
	///iAudioType[in]	: 音频输出模式 
	virtual void StopAudioOut(AudioOutType iAudioType, bool bForce = false) = 0;	
	
	///播放音频数据
	///data[in] 	: 音频数据buf
	///length[in] 	: 音频数据长度
	///iAudioType[in] : 音频输出模式，目前仅支持AUDIO_TALK_TYPE和AUDIO_TIP_TYPE
	///format[in]	: 音频格式
	///返回值: 成功TRUE，失败FALSE
	virtual bool PlayVoice(unsigned char *data, int length, AudioOutType iAudioType, int format = 0) = 0;

	///设置音频音量，实时生效,目前仅支持预览音量设置
	///iChannel[in] : 通道
	///dwLVolume[in] : 左声道音量
	///dwRVolume[in] : 右声道音量
	///返回值: 成功TRUE，失败FALSE
	virtual bool SetAudioOutVolume(int iChannel, unsigned int dwLVolume, unsigned int dwRVolume) = 0;

protected:
	IAudioManager(){}
	virtual ~IAudioManager(){};
};

#define g_AudioManager (*IAudioManager::instance())

//优先级由高到低:语音对讲->语音提示->回放->监视
class CAudioManager : public CThread, public IAudioManager
{
	PATTERN_SINGLETON_DECLARE(CAudioManager);
public:
	bool StartAudioOut(IAudioManager::AudioOutType iAudioType);
	void StopAudioOut(IAudioManager::AudioOutType iAudioType, bool bForce = false);												//关闭语音提示	
	bool PlayVoice(uchar *data, int length, IAudioManager::AudioOutType iAudioType, int format = 0);
	bool SetAudioOutVolume(int iChannel, unsigned int dwLVolume, unsigned int dwRVolume);
	
private:
	void ThreadProc();
	
private:	
	uint	m_dwAudioState;						//按位表示语音类型
	IAudioManager::AudioOutType		m_VoiceType;
	
	CMutex	m_AudioMutex;						//保护状态位的修改
	uint m_uiTalkFrameCount;
	uint m_uiAllFrameCount;
};

#endif 
