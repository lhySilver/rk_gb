
#include "FrameTimeConvert.h"

#define new DEBUG_NEW

CFrameTimeConvert::CFrameTimeConvert()
{
	m_LocalTime=0;
	m_RemoteTime=0;
}

CFrameTimeConvert::~CFrameTimeConvert()
{
}

void CFrameTimeConvert::SetFirstTime(LONGLONG localTime,LONGLONG remoteTime)
{
	m_LocalTime=localTime;
	m_RemoteTime=remoteTime;
}
