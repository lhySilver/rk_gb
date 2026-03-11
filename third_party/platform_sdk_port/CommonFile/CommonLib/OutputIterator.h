
#ifndef _OUTPUT_ITERATOR_H_
#define _OUTPUT_ITERATOR_H_

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "DecodeAndOutResManagerSDK.h"
#include "ConfigPack_Data_Item_ID_Def.h"
#include "ConfigPack_Struct_Def.h"
#include <list>	

class CConfigPack;
class _EXPORT_DECODE_AND_OUT_RES_MANAGER_CLASS COutputNode
{
public:
	virtual tuint32 GetNodeID(void) const = 0; ////获取输出ID
	virtual const GUID &GetNodeGUID(void) const = 0;////获取输出GUID
	
	virtual bool GetNodeInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperateType) const = 0;		//把节点信息打成配置包

	virtual int GetNodeIndex(void) const = 0;///获取输出的索引号
	virtual bool GetNodeName(char *szName, int nSize) const = 0;  ////获取输出的名称

 	virtual bool IsStartPreplan(void) const = 0;/////是否启动预案
	virtual const GUID &GetPreplanGUID(void) const = 0;	////获取预案GUID

 	virtual bool IsAlarmOut(void) const = 0;////是否为报警触发输出

	virtual int GetSplitScreenNumber(void) const = 0;////获取分屏数量

	virtual const GUID &GetOwnerGUID(void) const = 0;	////获取解码设备的GUID
	virtual tuint32 GetOwnerID(void) const = 0;////获取解码设备ID	
protected:
	COutputNode(void) {}
	virtual ~COutputNode(void) {}
};

class COutputNodeEx;
class _EXPORT_DECODE_AND_OUT_RES_MANAGER_CLASS COutputItemIterator
{
public:
	COutputItemIterator(void);
	explicit COutputItemIterator(tuint32 dwID);
	explicit COutputItemIterator(const GUID &NodeGUID);
 	explicit COutputItemIterator(int nIndex);

	~COutputItemIterator(void);

	bool HasNext(void);
	COutputNode *GetNext(void);

private:
	std::list<COutputNodeEx *> m_NodeList;
	std::list<COutputNodeEx *>::iterator m_Iter;
};

#endif //_OUTPUT_ITERATOR_H_

