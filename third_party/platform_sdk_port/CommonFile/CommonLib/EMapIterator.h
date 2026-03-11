#ifndef __EMAP_ITERATOR__
#define __EMAP_ITERATOR__

#include <list>
#include "EMapSDK.h"
#include "ConfigPack_Data_Item_ID_Def.h"

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

class CConfigPack;

class _EXPORT_EMAP_CLASS CEMapNode
{
public:
	virtual bool GetNodeInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperatorType) const = 0;	//获取电子地图节点数据信息
	virtual bool GetHotSpotEventInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperatorType) const = 0;	//获取热点事件信息,只热点有效

	//获取电子地图节点ID
	virtual tuint32 GetNodeID(void) const = 0;
	virtual const GUID &GetNodeGUID(void) const = 0;

	virtual bool GetNodeName(char *szName, int nBufLen) const = 0;		//获取节点名

	//节点类型判断
	virtual bool IsEMapNode(void) const = 0;
	virtual bool IsHotAreaNode(void) const = 0;
	virtual bool IsHotSpotNode(void) const = 0;

	//获得父节点的ID,对于热区和热点则为所在的地图ID
	virtual tuint32 GetParentEMapID(void) const = 0;
	virtual GUID GetParentEMapGUID(void) const = 0;

	//获得资源的ID,如通道,Sensor,Emap. 对热区和热点有效
	virtual const GUID &GetResourceGUID(void) const = 0;

	virtual const char *GetEMapData(tuint32 &nDataLen)  const = 0;	//获取地图信息,对地图节点有效 二进制内容
	virtual bool GetEMapFileExtension(char *szBuf, int nBufLen) const = 0;	//获取文件扩展名

	virtual bool GetPosition(char *xPos,int xPosLen,char *yPos,int yPosLen) const = 0;	//获取热区或热点的位置,对热区热点节点有效 地图获取初始坐标值
	virtual bool GetPosition(float &x, float &y) const = 0;
	virtual tuint32 GetZoomRate(void) const = 0;	

	virtual unsigned char GetImageIndex(void) const = 0;			//获取图标索引,对热区热点节点有效
	virtual bool GetTextColor(char *szText, int nBufLen) const = 0;					//获取文本颜色,对热区热点节点有效

	virtual tuint32 GetEMapFileType(void) const = 0;//地图类型，对地图节点有效

	virtual tuint32 GetAttribute(void) const = 0;//地图用途属性	

protected:
	CEMapNode(void) {}
	virtual ~CEMapNode(void) {}
};

class CEMapNodeEx;
class _EXPORT_EMAP_CLASS CEMapIteratorBase
{
public:
	virtual bool HasNext(void);
	virtual CEMapNode *GetNext(void);	

protected:
	CEMapIteratorBase(void);
	virtual ~CEMapIteratorBase(void);

	std::list<CEMapNodeEx *> m_NodeList;//用来存储已经使用的节点，在析构时Release
	std::list<CEMapNodeEx *>::iterator m_Iter;
};

//电子地图先根迭代器
class _EXPORT_EMAP_CLASS CEMapTreeNLRIterator : public CEMapIteratorBase
{
public:
	explicit CEMapTreeNLRIterator(tuint32 dwRootID = 0);		//遍历此电子地图及其所有子电子地图节点(不包括地图里的热区热点)
	explicit CEMapTreeNLRIterator(const GUID &RootGUID);
	virtual ~CEMapTreeNLRIterator(void) {}
};

class _EXPORT_EMAP_CLASS CEMapTreeLRNIterator : public CEMapIteratorBase
{
public:
	explicit CEMapTreeLRNIterator(tuint32 dwRootID = 0);		//遍历此电子地图及其所有子电子地图节点(不包括地图里的热区热点)
	explicit CEMapTreeLRNIterator(const GUID &RootGUID);
	virtual ~CEMapTreeLRNIterator(void) {}
};

//单节点迭代器,可以是地图,热区,热点节点
class _EXPORT_EMAP_CLASS CEMapOneNodeIterator
{
public:
	explicit CEMapOneNodeIterator(tuint32 dwNodeID);
	explicit CEMapOneNodeIterator(const GUID &NodeGUID);
	~CEMapOneNodeIterator(void);

	CEMapNode *operator ->(void);
	operator CEMapNode *(void);

private:
	CEMapNodeEx *m_pCurNode;
};

class _EXPORT_EMAP_CLASS CEMapHotAreaAndSpotIterator : public CEMapIteratorBase
{
public:
	explicit CEMapHotAreaAndSpotIterator(tuint32 dwNodeID);			//遍历此电子地图节点下的所有热区和热点
	explicit CEMapHotAreaAndSpotIterator(const GUID &NodeGUID);
	virtual ~CEMapHotAreaAndSpotIterator(void) {}
};

class _EXPORT_EMAP_CLASS CEMapHotAreaIterator : public CEMapIteratorBase
{
public:
	explicit CEMapHotAreaIterator(tuint32 dwNodeID);			//遍历此电子地图节点下的所有热区
	explicit CEMapHotAreaIterator(const GUID &NodeGUID);
	virtual ~CEMapHotAreaIterator(void) {}

private:
	void ExcludeHotSpotNode(void);
};

class _EXPORT_EMAP_CLASS CEMapHotSpotIterator : public CEMapIteratorBase
{
public:
	explicit CEMapHotSpotIterator(tuint32 dwNodeID);			//遍历此电子地图节点下的所有热点
	explicit CEMapHotSpotIterator(const GUID &NodeGUID);
	virtual ~CEMapHotSpotIterator(void) {}

private:
	void ExcludeHotAreaNode(void);
};

class _EXPORT_EMAP_CLASS CEMapFileIterator : public CEMapIteratorBase
{
public:
	explicit CEMapFileIterator();//所有电子地图文件
	explicit CEMapFileIterator(tuint32 dwNodeID);
	explicit CEMapFileIterator(const GUID &NodeGUID);
	virtual ~CEMapFileIterator(void) {}
};

class _EXPORT_EMAP_CLASS CHotAreaIterator : public CEMapIteratorBase
{
public:
	explicit CHotAreaIterator();//所有热区	
	explicit CHotAreaIterator(const char *szName);				//依名称迭代出热区
	explicit CHotAreaIterator(tuint32 dwNodeID);
	explicit CHotAreaIterator(const GUID &NodeGUID);
	virtual ~CHotAreaIterator(void) {}
};

class _EXPORT_EMAP_CLASS CHotSpotIterator : public CEMapIteratorBase
{
public:
	explicit CHotSpotIterator();//所有热点
	explicit CHotSpotIterator(const char *szName);				//依名称迭代出热点
	explicit CHotSpotIterator(tuint32 dwNodeID);
	explicit CHotSpotIterator(const GUID &NodeGUID);
	virtual ~CHotSpotIterator(void) {}
};

class _EXPORT_EMAP_CLASS CRHotAreaIterator : public CEMapIteratorBase
{
public:
	CRHotAreaIterator(const GUID &ResNodeGUID);			//反向迭代器,由地图节点ID迭代出被应用于哪些热区
	virtual ~CRHotAreaIterator(void) {}
};

class _EXPORT_EMAP_CLASS CRHotSpotIterator : public CEMapIteratorBase
{
public:
	CRHotSpotIterator(const GUID &ResNodeGUID);			//反向迭代器,由资源(通道或Sensor)节点ID迭代出被应用于哪些热点
	virtual ~CRHotSpotIterator(void) {}
};

//普通地图
class _EXPORT_EMAP_CLASS CEMapFileCommonIterator : public CEMapIteratorBase
{
public:
	explicit CEMapFileCommonIterator();//所有普通电子地图文件
	virtual ~CEMapFileCommonIterator(void) {}
};
//智能地图
class _EXPORT_EMAP_CLASS CEMapFileSmartIterator : public CEMapIteratorBase
{
public:
	explicit CEMapFileSmartIterator();//所有智能电子地图文件
	virtual ~CEMapFileSmartIterator(void) {}
};


#endif //__EMAP_ITERATOR__