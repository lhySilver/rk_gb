
#ifndef _OUTPUT_FRAME_ITERATOR_H_
#define _OUTPUT_FRAME_ITERATOR_H_

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "DecodeAndOutResManagerSDK.h"
#include "ConfigPack_Data_Item_ID_Def.h"
#include "ConfigPack_Struct_Def.h"	   
#include <list>

class CConfigPack;
class _EXPORT_DECODE_AND_OUT_RES_MANAGER_CLASS COutputFrameNode
{
public:
	virtual tuint32 GetNodeID(void) const = 0;
	virtual const GUID &GetNodeGUID(void) const = 0; 

	virtual bool GetNodeInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperateType) const = 0;		//把节点信息打成配置包
	virtual int GetNodeIndex(void) const = 0;///获取索引号
	virtual bool GetNodeName(char *szName, int nSize) const = 0; 	

	virtual bool IsStartCHGroup(void) const = 0;/////是否通道组轮询
	virtual const GUID &GetContentGUID(void) const = 0;	////获取内容GUID

	virtual const GUID &GetOwnerGUID(void) const = 0;	////获取输出的GUID
	virtual tuint32 GetOwnerID(void) const = 0;////获取输出ID

protected:
	COutputFrameNode(void) {}
	virtual ~COutputFrameNode(void) {}
};


class COutputFrameNodeEx;
class _EXPORT_DECODE_AND_OUT_RES_MANAGER_CLASS COutputFrameItemIterator
{
public:
	COutputFrameItemIterator(void);								//所有输出框
	explicit COutputFrameItemIterator(tuint32 dwID);
	explicit COutputFrameItemIterator(const GUID &NodeGUID);

	~COutputFrameItemIterator(void);

	bool HasNext(void);
	COutputFrameNode *GetNext(void);

private:
	std::list<COutputFrameNodeEx *> m_NodeList;
	std::list<COutputFrameNodeEx *>::iterator m_Iter;
};

#endif //_OUTPUT_FRAME_ITERATOR_H_