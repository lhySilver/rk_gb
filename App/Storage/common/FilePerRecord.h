#ifndef __FILEPRERECORD_H__
#define __FILEPRERECORD_H__

#include "RingBuffer.h"

#include "Infra/Thread.h"



class CFilePreRecord
{
public:
	CFilePreRecord();
	~CFilePreRecord();

private:
	FIlE *m_pFp;
	int m_iFileSize;
	int m_iReadPos;
	int m_iWritePos;
};


#endif 	//__FILEPRERECORD_H__