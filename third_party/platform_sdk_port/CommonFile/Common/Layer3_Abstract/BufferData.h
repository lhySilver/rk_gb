
#ifndef __BUFFER_DATA__
#define __BUFFER_DATA__
#include "Lock.h"

class CBufferData
{
public:
	CBufferData(const void *pFrameData, int iBufLen, bool bIsAutoDestroy = FALSE);
	virtual ~CBufferData(void);

	///////////////计数加1
	int Add();
	///////////////计数减1，如果是自动删除的模式，为0的时候会销毁对象，非自动模式无效
	int Dec();	

	///////////////获取缓冲区的长度
	int GetBufferDataLen() const;

	///////////////获取缓冲区
	const BYTE * GetBufferData() const;
private:
	//////////数据缓冲区
	BYTE *m_pFrameData;	
	
	//////////缓冲区的长度
	int   m_iLen;

	////////数量计算
	int   m_Count;

	///////////锁
	CLock     m_Lock;

	////////是否自动销毁传进来的内存及对象自身
	bool                 m_bAutoDestroy;
};

#endif