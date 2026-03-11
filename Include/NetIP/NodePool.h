#ifndef _NET_NODE_POOL_
#define _NET_NODE_POOL_
#include "NetIP/NetDef.h"

class CNetNodePool
{
public:
	CNetNodePool();
	virtual ~CNetNodePool();

	int Init(uint nTotalNum);
	NODE_HEAD_T* GetNode(uint nNodeNum);
	int ReleaseNode(NODE_HEAD_T *pNode, uint nNodeNum);
	int DeleteNodePool();
	int GetNodeTotalNum();
private:
	NODE_POOL m_NodePool;
	CMutex			m_Mutex;			/*!< 同步锁 */
};
#endif

