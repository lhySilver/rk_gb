#ifndef __PLAYBACK_MANAGER_H__
#define __PLAYBACK_MANAGER_H__

#include "RingBuffer.h"

#include "Infra/Thread.h"



class CPlaybackManager : public CThread
{
public:
	CPlaybackManager();
	~CPlaybackManager();

	
	/// 线程执行体
	void ThreadProc();

	/// 开启
	bool start();

	/// 停止
	bool stop();

	Int32 StartPlay();
};


#endif 	//__PLAYBACK_MANAGER_H__
