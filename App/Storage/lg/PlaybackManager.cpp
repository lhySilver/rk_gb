#include "PlaybackManager.h"


CPlaybackManager::CPlaybackManager() : CThread("CPlaybackManager", 50)
{

}

CPlaybackManager::~CPlaybackManager()
{

}

/// 线程执行体
void CPlaybackManager::ThreadProc()
{
	
}

/// 开启
bool CPlaybackManager::start()
{
	return CreateThread();
}

/// 停止
bool CPlaybackManager::stop()
{
	return DestroyThread();
}


