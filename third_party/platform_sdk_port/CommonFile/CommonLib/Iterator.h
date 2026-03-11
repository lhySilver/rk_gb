
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "NodeManagerDll.h"
#include "TypeCheck.h"
#include <list>
#include<vector>
#include "SysHelper.h"

class CLocalNodeBase;
class CLocalNode;

//******************************************************************************/
//迭代器基类，实现了列表迭代器基本方法，列表迭代器继承时只需在构造时构造出m_NodeList
class _EXPORT_NODEMANAGER_CLASS CIteratorBase
{
public:
	virtual ~CIteratorBase(void);

	//根据数据集实现相应的取当前项同时移动到下一项函数
	CLocalNode *GetNext(void);

	bool HasNext(void);

protected:
	CIteratorBase(void);

	std::list<CLocalNodeBase *> m_NodeList;		//用来存储全部
	std::list<CLocalNodeBase *>::iterator m_Iter;	//这个用来记录当前迭代位置
};


//******************************************************************************/
//单节点迭代器，可以通过ID迭代出系统中所有类型的CLocalNode
class _EXPORT_NODEMANAGER_CLASS COneNodeIterator
{
public:
	explicit COneNodeIterator(tuint32 dwNodeID);
	explicit COneNodeIterator(const GUID &NodeGUID);

	~COneNodeIterator(void);

	CLocalNode *operator ->(void);
	operator CLocalNode *(void);

private:
	COneNodeIterator(void) {}					//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	CLocalNodeBase *m_pLocalNode;
};

//单类型迭代器，可以迭代出系统某种类型的全部节点
class _EXPORT_NODEMANAGER_CLASS COneTypeIterator : public CIteratorBase
{
public:
	explicit COneTypeIterator(CTypeCheck::ECMS_NODE_TYPE_DEF nodeType);
	virtual ~COneTypeIterator(void) {}

private:
	COneTypeIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出全部的存储服务器节点，和COneTypeIterator（NODE_TYPE_STORAGE_SERVER）效果相同
class _EXPORT_NODEMANAGER_CLASS CMSUIterator : public CIteratorBase
{
public:
	CMSUIterator(void);
	virtual ~CMSUIterator(void) {}
};

//以列表方式迭代出全部的GPS存储服务器节点，和COneTypeIterator（NODE_TYPE_GSU_SERVER）效果相同
class _EXPORT_NODEMANAGER_CLASS CGSUIterator : public CIteratorBase
{
public:
	CGSUIterator(void);
	virtual ~CGSUIterator(void) {}
};

//以列表方式迭代出全部的转发服务器节点，和COneTypeIterator（NODE_TYPE_MEDIA_TRANSFER_SERVER）效果相同
class _EXPORT_NODEMANAGER_CLASS CMDUIterator : public CIteratorBase
{
public:
	CMDUIterator(void);
	virtual ~CMDUIterator(void) {}
};

//以列表方式迭代出全部的接入服务器节点
class _EXPORT_NODEMANAGER_CLASS CJSUIterator : public CIteratorBase
{
public:
	CJSUIterator(void);
	virtual ~CJSUIterator(void) {}
};

//以列表方式迭代出全部的报警服务器节点
class _EXPORT_NODEMANAGER_CLASS CADUIterator : public CIteratorBase
{
public:
	CADUIterator(void);
	virtual ~CADUIterator(void) {}
};

//以列表方式迭代出全部的智能分析服务器节点
class _EXPORT_NODEMANAGER_CLASS CANSIterator : public CIteratorBase
{
public:
	CANSIterator(void);
	virtual ~CANSIterator(void) {}
};

//以列表方式迭代出全部的级联网关服务器节点
class _EXPORT_NODEMANAGER_CLASS CCGUIterator : public CIteratorBase
{
public:
	CCGUIterator(void);
	virtual ~CCGUIterator(void) {}
};

//以列表方式迭代出全部的1400In服务器节点
class _EXPORT_NODEMANAGER_CLASS C1400InIterator : public CIteratorBase
{
public:
	C1400InIterator(void);
	virtual ~C1400InIterator(void) {}
};

//独立运行的媒体服务器
class _EXPORT_NODEMANAGER_CLASS CMediaServerIterator : public CIteratorBase
{
public:
	CMediaServerIterator(void);
	virtual ~CMediaServerIterator(void) {}
};

//以列表方式迭代出服务器节点
class _EXPORT_NODEMANAGER_CLASS CServerIterator : public CIteratorBase
{
public:
	CServerIterator(void);									//迭代所有服务器节点
	explicit CServerIterator(tuint32 dwNodeID);				//迭代指定服务器节点
	explicit CServerIterator(const GUID &NodeGUID);
	CServerIterator(const char *szIP, tuint16 usPort );		//根据IP和端口查找
	virtual ~CServerIterator(void) {}
};

//以列表方式迭代出全部的编码设备节点，和COneTypeIterator（NODE_TYPE_PERIPHERAL_ABSTRACT_DEVICE）效果相同
//不包括IP通道
class _EXPORT_NODEMANAGER_CLASS CPUIterator : public CIteratorBase
{
public:
	CPUIterator(void);
	explicit CPUIterator(tuint32 dwNodeID);
	explicit CPUIterator(const GUID &NodeGUID);
	explicit CPUIterator(const char *szName);					//根据设备名称查找设备
	CPUIterator(const char *szIP, tuint16 usPort);		//根据IP和端口查找设备
	virtual ~CPUIterator(void) {}
};

//以列表方式迭代出全部的GB编码设备节点，和COneTypeIterator（NODE_TYPE_PERIPHERAL_ABSTRACT_DEVICE）效果相同
//不包括IP通道
class _EXPORT_NODEMANAGER_CLASS CGBDeviceIterator : public CIteratorBase
{
public:
	CGBDeviceIterator(void);
	explicit CGBDeviceIterator(const GUID &NodeGUID);
	explicit CGBDeviceIterator(const char *szID);					//根据设备GBID查找设备
	explicit CGBDeviceIterator(tuint32 dwNodeID);					//根据设备id查找设备
	virtual ~CGBDeviceIterator(void) {}
};

//以列表方式迭代出全部的解码设备节点 解码器
class _EXPORT_NODEMANAGER_CLASS CDecoderIterator : public CIteratorBase
{
public:
	CDecoderIterator(void);
	explicit CDecoderIterator(tuint32 dwNodeID);
	explicit CDecoderIterator(const GUID &NodeGUID);
	explicit CDecoderIterator(const char *szName);					//根据设备名称查找设备
	CDecoderIterator(const char *szIP, tuint16 usPort);		//根据IP和端口查找设备
	virtual ~CDecoderIterator(void) {}
};

//通道迭代器
class _EXPORT_NODEMANAGER_CLASS CChannelIterator : public CIteratorBase
{
public:
	CChannelIterator(void);
	explicit CChannelIterator(tuint32 dwNodeID);
	explicit CChannelIterator(const GUID &NodeGUID);
	explicit CChannelIterator(const char *szName);				//根据通道名称(通道节点名称)查找通道
	explicit CChannelIterator(bool bByKeyboardIndex/*是否为根据键盘ID获取通道*/,tuint32 dwKeyboardIndex);
	virtual ~CChannelIterator(void) {}
};

// IP通道迭代器
// class _EXPORT_NODEMANAGER_CLASS CIPChannelIterator : public CIteratorBase
// {
// public:
// 	CIPChannelIterator(void);
// 	explicit CIPChannelIterator(tuint32 dwNodeID);
// 	explicit CIPChannelIterator(const GUID &NodeGUID);
// 	explicit CIPChannelIterator(const char *szName);
// 	CIPChannelIterator(const char *szIP, tuint16 usPort, tuint32 nIndex);		//根据IP和端口查找
// 	virtual ~CIPChannelIterator(void) {}
// };

//全部通道迭代器,包括IP通道
class _EXPORT_NODEMANAGER_CLASS CAllChannelIterator : public CIteratorBase
{
public:
	CAllChannelIterator(void);
	virtual ~CAllChannelIterator(void) {}
};

//迭代出存储位置下的所有通道
class _EXPORT_NODEMANAGER_CLASS CRecordPathChannelIterator : public CIteratorBase
{
public:
	explicit CRecordPathChannelIterator(const GUID &PathGUID);
	virtual ~CRecordPathChannelIterator(void) {}
};

class _EXPORT_NODEMANAGER_CLASS CSensorIterator : public CIteratorBase
{
public:
	CSensorIterator(void);
	explicit CSensorIterator(tuint32 dwNodeID);
	explicit CSensorIterator(const GUID &NodeGUID);
	virtual ~CSensorIterator(void) {}
};

//报警输出迭代器
class _EXPORT_NODEMANAGER_CLASS CAlarmOutIterator : public CIteratorBase
{
public:
	CAlarmOutIterator(void);
	explicit CAlarmOutIterator(tuint32 dwNodeID);
	explicit CAlarmOutIterator(const GUID &NodeGUID);
	virtual ~CAlarmOutIterator(void) {}
};

class _EXPORT_NODEMANAGER_CLASS CAreaIterator : public CIteratorBase
{
public:
	CAreaIterator(void);
	explicit CAreaIterator(const char *szName);
	explicit CAreaIterator(tuint32 dwNodeID);
	explicit CAreaIterator(const GUID &NodeGUID);
	explicit CAreaIterator(const std::string& GBIDStr);					//根据设备GBID查找设备
	//explicit CAreaIterator(const GUID &UserORUserGroupGUID/*用户GUID或者角色GUID*/,bool bIsCreateUserGroupUsed/*是否是新增角色时用到*/);
	virtual ~CAreaIterator(void) {}
private:
// 	GUID GetAreaParentNodeGUID(GUID areaGUID);
// 	//获取区域的直系区域 下坐标越小区域越大
// 	std::vector<GUID> GetDirectLineArea(GUID areaGUID);

};

//以列表方式迭代出某个服务器下的全部设备节点（不含服务器本身）
class _EXPORT_NODEMANAGER_CLASS CServerDevIterator : public CIteratorBase
{
public:
	explicit CServerDevIterator(tuint32 smuNodeID);
	explicit CServerDevIterator(GUID smuNodeGUID);
	virtual ~CServerDevIterator(void) {}
private:
	CServerDevIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出某个服务器下的全部电视墙节点（不含服务器本身）
class _EXPORT_NODEMANAGER_CLASS CServerTVWallIterator : public CIteratorBase
{
public:
	explicit CServerTVWallIterator(tuint32 tvwNodeID);
	virtual ~CServerTVWallIterator(void) {}
private:
	CServerTVWallIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出某个设备下的全部或指定序号的通道节点
class _EXPORT_NODEMANAGER_CLASS CDevCHIterator : public CIteratorBase
{
public:
	explicit CDevCHIterator(tuint32 devNodeID);
	CDevCHIterator(tuint32 devNodeID, tuint32 nChannelNo);
	explicit CDevCHIterator(const GUID &DevNodeGUID);
	CDevCHIterator(const GUID &DevNodeGUID, tuint32 nChannelNo);
	CDevCHIterator(const char *szIP, tuint16 usPort, tuint32 nChannelNo);		//根据设备的IP和端口查找其下指定通道号的通道
	virtual ~CDevCHIterator(void) {}
private:
	CDevCHIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	template<class KEY>
	void GetDeviceChannel(const KEY &Key, tint32 nChannelNo);
};

//以列表方式迭代出某个设备下的全部或指定序号的Sensor节点
class _EXPORT_NODEMANAGER_CLASS CDevSensorIterator : public CIteratorBase
{
public:
	explicit CDevSensorIterator(tuint32 devNodeID);
	CDevSensorIterator(tuint32 devNodeID, tuint32 nSensorNO);
	explicit CDevSensorIterator(const GUID &DevNodeGUID);
	CDevSensorIterator(const GUID &DevNodeGUID, tuint32 nSensorNO);
	virtual ~CDevSensorIterator(void) {}
private:
	CDevSensorIterator(void) {}	//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	template<class KEY>
	void GetDeviceSensor(const KEY &Key, tint32 nSenNo);
};

//迭代设备下的Alarm Out
class _EXPORT_NODEMANAGER_CLASS CDevAlarmOutIterator : public CIteratorBase
{
public:
	explicit CDevAlarmOutIterator(tuint32 devNodeID);
	CDevAlarmOutIterator(tuint32 devNodeID, tuint32 nAlarmOutNO);
	explicit CDevAlarmOutIterator(const GUID &DevNodeGUID);
	CDevAlarmOutIterator(const GUID &DevNodeGUID, tuint32 nAlarmOutNO);
	virtual ~CDevAlarmOutIterator(void) {}
private:
	CDevAlarmOutIterator(void) {}	//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	template<class KEY>
	void GetDeviceAlarmOut(const KEY &Key, tint32 nAlarmOutNO);
};

//迭代本地设备下的Alarm Out/In
class _EXPORT_NODEMANAGER_CLASS CLocalAlarmInIterator : public CIteratorBase
{
public:
    CLocalAlarmInIterator();
    explicit CLocalAlarmInIterator(tuint32 dwNodeID);	//
    explicit CLocalAlarmInIterator(const GUID &NodeGUID);
    CLocalAlarmInIterator(tuint32 devNodeID, tuint32 nAlarmInNO);
    CLocalAlarmInIterator(const GUID &DevNodeGUID, tuint32 nAlarmInNO);
    virtual ~CLocalAlarmInIterator(void) {}

private:
    template<class KEY>
    void GetLocalAlarmIn(const KEY &Key, tint32 nAlarmInNO);
};
class _EXPORT_NODEMANAGER_CLASS CLocalAlarmOutIterator : public CIteratorBase
{
public:
    CLocalAlarmOutIterator();
    explicit CLocalAlarmOutIterator(tuint32 dwNodeID);	//
    explicit CLocalAlarmOutIterator(const GUID &NodeGUID);
    CLocalAlarmOutIterator(tuint32 devNodeID, tuint32 nAlarmOutNO);
    CLocalAlarmOutIterator(const GUID &DevNodeGUID, tuint32 nAlarmOutNO);
    virtual ~CLocalAlarmOutIterator(void) {}

private:
    template<class KEY>
    void GetLocalAlarmOut(const KEY &Key, tint32 nAlarmOutNO);
};


//以列表方式迭代出没有用到的Sensor节点
class _EXPORT_NODEMANAGER_CLASS CNoUsedSensorIteratorList : public CIteratorBase
{
public:
	CNoUsedSensorIteratorList(void);
	virtual ~CNoUsedSensorIteratorList(void) {}
};

//以列表方式迭代出系统中全部的节点资源
class _EXPORT_NODEMANAGER_CLASS CAllNodeIterator : public CIteratorBase
{
public:
	CAllNodeIterator(void);
	virtual ~CAllNodeIterator(void) {}
};

//以列表方式迭代出服务器下的所有节点,不包括服务器本身
class _EXPORT_NODEMANAGER_CLASS CServerChildIterator : public CIteratorBase
{
public:
	explicit CServerChildIterator(tuint32 smuNodeID);
	explicit CServerChildIterator(const GUID &SMUGUID);
	virtual ~CServerChildIterator(void) {}
};

//区域先根迭代器,可以由一个区域遍历到通道节点,也可以遍历所有的区域节点及其下到通道节点
class _EXPORT_NODEMANAGER_CLASS CAreaTreeNLRIterator : public CIteratorBase
{
public:
	CAreaTreeNLRIterator(void);
	explicit CAreaTreeNLRIterator(tuint32 dwRootID);	//此节点可以为区域,设备,通道节点
	explicit CAreaTreeNLRIterator(const GUID &NodeGUID);
	virtual ~CAreaTreeNLRIterator(void) {}
};

//CAreaTreeLRNIterator;//区域后根迭代器,遍历当前节点至通道节点
//区域后根迭代器,可以由一个区域遍历到通道节点,也可以遍历所有的区域节点及其下到通道节点
class _EXPORT_NODEMANAGER_CLASS CAreaTreeLRNIterator : public CIteratorBase
{
public:
	CAreaTreeLRNIterator(void);
	explicit CAreaTreeLRNIterator(tuint32 dwRootID);	//此节点可以为区域,设备,通道节点
	explicit CAreaTreeLRNIterator(const GUID &NodeGUID);
	virtual ~CAreaTreeLRNIterator(void) {}
};

//Sensor区域先根迭代器,可以由一个区域遍历到Sensor节点,也可以遍历所有的区域节点及其下到Sensor节点
class _EXPORT_NODEMANAGER_CLASS CSensorAreaTreeNLRIterator : public CIteratorBase
{
public:
	CSensorAreaTreeNLRIterator(void);
	explicit CSensorAreaTreeNLRIterator(tuint32 dwRootID);	//此节点可以为区域,设备,Sensor节点
	explicit CSensorAreaTreeNLRIterator(const GUID &NodeGUID);
	virtual ~CSensorAreaTreeNLRIterator(void) {}
};

//Sensor区域后根迭代器,可以由一个区域遍历到Sensor节点,也可以遍历所有的区域节点及其下到Sensor节点
class _EXPORT_NODEMANAGER_CLASS CSensorAreaTreeLRNIterator : public CIteratorBase
{
public:
	CSensorAreaTreeLRNIterator(void);
	explicit CSensorAreaTreeLRNIterator(tuint32 dwRootID);	//此节点可以为区域,设备,Sensor节点
	explicit CSensorAreaTreeLRNIterator(const GUID &NodeGUID);
	virtual ~CSensorAreaTreeLRNIterator(void) {}
};

class _EXPORT_NODEMANAGER_CLASS CAlarmOutAreaTreeNLRIterator : public CIteratorBase
{
public:
	CAlarmOutAreaTreeNLRIterator(void);
	explicit CAlarmOutAreaTreeNLRIterator(tuint32 dwRootID);	//此节点可以为区域,设备,Alarm Out节点
	explicit CAlarmOutAreaTreeNLRIterator(const GUID &NodeGUID);
	virtual ~CAlarmOutAreaTreeNLRIterator(void) {}
};

//Alarm Out区域后根迭代器,可以由一个区域遍历到Alarm Out节点,也可以遍历所有的区域节点及其下到Sensor节点
class _EXPORT_NODEMANAGER_CLASS CAlarmOutAreaTreeLRNIterator : public CIteratorBase
{
public:
	CAlarmOutAreaTreeLRNIterator(void);
	explicit CAlarmOutAreaTreeLRNIterator(tuint32 dwRootID);	//此节点可以为区域,设备,Alarm Out节点
	explicit CAlarmOutAreaTreeLRNIterator(const GUID &NodeGUID);
	virtual ~CAlarmOutAreaTreeLRNIterator(void) {}
};

//CAllAreaTreeNLRIterator;//同时含有通道和Sensor区域先根迭代器,遍历当前节点至Sensor节点和通道节点
class _EXPORT_NODEMANAGER_CLASS CAllAreaTreeNLRIterator : public CIteratorBase
{
public:
	CAllAreaTreeNLRIterator(void);
	explicit CAllAreaTreeNLRIterator(tuint32 dwRootID);	//此节点可以为区域,设备,Sensor节点
	explicit CAllAreaTreeNLRIterator(const GUID &NodeGUID);
	virtual ~CAllAreaTreeNLRIterator(void) {}
};

//区域后根迭代器,可以由一个区域遍历到通道节点,也可以遍历所有的区域节点及其下到通道节点
class _EXPORT_NODEMANAGER_CLASS CChannelGroupTreeLRNIterator : public CIteratorBase
{
public:
	CChannelGroupTreeLRNIterator(void);
	explicit CChannelGroupTreeLRNIterator(tuint32 dwRootID);	//此节点可以为区域,设备,通道节点
	explicit CChannelGroupTreeLRNIterator(const GUID &NodeGUID);
	virtual ~CChannelGroupTreeLRNIterator(void) {}
};

//节点所属服务器迭代器,迭代出此节点所在的所有服务器
class _EXPORT_NODEMANAGER_CLASS CParentServerIterator : public CIteratorBase
{
public:
	explicit CParentServerIterator(const GUID &NodeGUID);
	virtual ~CParentServerIterator(void) {}
};

//以列表方式迭代出全部的电视墙服务器节点
class _EXPORT_NODEMANAGER_CLASS CTVWServerIterator : public CIteratorBase
{
public:
	CTVWServerIterator(void);
	virtual ~CTVWServerIterator(void) {}
};


//以列表方式迭代出全部的电视墙节点
class _EXPORT_NODEMANAGER_CLASS CTVWallIterator : public CIteratorBase
{
public:
	CTVWallIterator(void);
	explicit CTVWallIterator(tuint32 dwNodeID);
	explicit CTVWallIterator(const GUID &NodeGUID);
	explicit CTVWallIterator(const char *szName);					//根据设备名称查找墙
	explicit CTVWallIterator(bool bByKeyboardIndex/*是否为根据键盘ID获取墙*/,tuint32 dwKeyboardIndex);
	virtual ~CTVWallIterator(void) {}
};

//以列表方式迭代出全部的通道组节点
class _EXPORT_NODEMANAGER_CLASS CChannelGroupIterator : public CIteratorBase
{
public:
	CChannelGroupIterator(void);
	explicit CChannelGroupIterator(tuint32 dwNodeID);
	explicit CChannelGroupIterator(const GUID &NodeGUID);
	explicit CChannelGroupIterator(const char *szName);			
	virtual ~CChannelGroupIterator(void) {}
};

//以列表方式迭代出全部的报警输出组节点
class _EXPORT_NODEMANAGER_CLASS CAlarmOutGroupIterator : public CIteratorBase
{
public:
	CAlarmOutGroupIterator(void);
	explicit CAlarmOutGroupIterator(tuint32 dwNodeID);
	explicit CAlarmOutGroupIterator(const GUID &NodeGUID);
	explicit CAlarmOutGroupIterator(const char *szName);			
	virtual ~CAlarmOutGroupIterator(void) {}
};

//以列表方式迭代出某一用户拥有的通道组节点
class _EXPORT_NODEMANAGER_CLASS CChannelGroupbyUserIterator : public CIteratorBase
{
public:
	CChannelGroupbyUserIterator(const GUID &UserGUID);		
	virtual ~CChannelGroupbyUserIterator(void) {}
};

//以列表方式迭代出某一用户拥有的报警输出组节点
class _EXPORT_NODEMANAGER_CLASS CAlarmOutGroupbyUserIterator : public CIteratorBase
{
public:
	CAlarmOutGroupbyUserIterator(const GUID &UserGUID);		
	virtual ~CAlarmOutGroupbyUserIterator(void) {}
};

//以列表方式迭代出全部的报警主机节点
class _EXPORT_NODEMANAGER_CLASS CAlarmHostIterator : public CIteratorBase
{
public:
	CAlarmHostIterator(void);
	explicit CAlarmHostIterator(tuint32 dwNodeID);
	explicit CAlarmHostIterator(const GUID &NodeGUID);
	explicit CAlarmHostIterator(const char *szName);			
	virtual ~CAlarmHostIterator(void) {}
};

//以列表方式迭代出全部的子系统节点
class _EXPORT_NODEMANAGER_CLASS CSubSystemIterator : public CIteratorBase
{
public:
	CSubSystemIterator(void);
	explicit CSubSystemIterator(tuint32 dwNodeID);
	explicit CSubSystemIterator(const GUID &NodeGUID);
	explicit CSubSystemIterator(const char *szName);			
	virtual ~CSubSystemIterator(void) {}
};

//以列表方式迭代出全部的防区节点
class _EXPORT_NODEMANAGER_CLASS CZoneIterator : public CIteratorBase
{
public:
	CZoneIterator(void);
	explicit CZoneIterator(tuint32 dwNodeID);
	explicit CZoneIterator(const GUID &NodeGUID);
	explicit CZoneIterator(const char *szName);			
	virtual ~CZoneIterator(void) {}
};

//以列表方式迭代出某个报警主机下的全部或指定序号的子系统节点
class _EXPORT_NODEMANAGER_CLASS CAlmHostSubSystemIterator : public CIteratorBase
{
public:
	explicit CAlmHostSubSystemIterator(tuint32 AlarmHostNodeID);
	CAlmHostSubSystemIterator(tuint32 AlarmHostNodeID, tuint32 nSubSystemNo);
	explicit CAlmHostSubSystemIterator(const GUID &AlarmHostNodeGUID);
	CAlmHostSubSystemIterator(const GUID &AlarmHostNodeGUID, tuint32 nSubSystemNo);
	CAlmHostSubSystemIterator(const char *szIP, tuint16 usPort, tuint32 nSubSystemNo);		//根据设备的IP和端口查找其下指定子系统
	CAlmHostSubSystemIterator(const char *szSerial, tuint32 nSubSystemNo);		//根据设备的序列号和子系统号查找其下指定子系统
	virtual ~CAlmHostSubSystemIterator(void) {}
private:
	CAlmHostSubSystemIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	template<class KEY>
	void GetAlarmHostSubSystem(const KEY &Key, tint32 nSubSystemNo);
};

//以列表方式迭代出某个报警主机下的全部或指定序号的防区节点
class _EXPORT_NODEMANAGER_CLASS CAlmHostZoneIterator : public CIteratorBase
{
public:
	explicit CAlmHostZoneIterator(tuint32 AlarmHostNodeID);
	CAlmHostZoneIterator(tuint32 AlarmHostNodeID, tuint32 nZoneNo);
	explicit CAlmHostZoneIterator(const GUID &AlarmHostNodeGUID);
	CAlmHostZoneIterator(const GUID &AlarmHostNodeGUID, tuint32 nZoneNo);
	CAlmHostZoneIterator(const char *szIP, tuint16 usPort, tuint32 nZoneNo);		//根据报警主机的IP和端口查找其下指定通道号的通道
	CAlmHostZoneIterator(const char *szSerial, tuint32 nZoneNo);		//根据报警主机的序列号和防区号查找其下指定防区
	virtual ~CAlmHostZoneIterator(void) {}
private:
	CAlmHostZoneIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	template<class KEY>
	void GetAlarmHostZone(const KEY &Key, tint32 nZoneNo);
};

//以列表方式迭代出某个子系统的全部或指定序号的防区节点
class _EXPORT_NODEMANAGER_CLASS CSubSystemZoneIterator : public CIteratorBase
{
public:
	explicit CSubSystemZoneIterator(tuint32 SubSystemNodeID);
	CSubSystemZoneIterator(tuint32 SubSystemNodeID, tuint32 nZoneNo);
	explicit CSubSystemZoneIterator(const GUID &SubSystemNodeGUID);
	CSubSystemZoneIterator(const GUID &SubSystemNodeGUID, tuint32 nZoneNo);
	virtual ~CSubSystemZoneIterator(void) {}
private:
	CSubSystemZoneIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
	template<class KEY>
	void GetSubSystemZone(const KEY &Key, tint32 nZoneNo);
};

//以列表方式迭代出某个服务器下全部的第三方门禁考勤系统节点
class _EXPORT_NODEMANAGER_CLASS CServerAcsSystemIterator : public CIteratorBase
{
public:
	explicit CServerAcsSystemIterator(tuint32 serverNodeID);
	explicit CServerAcsSystemIterator(const GUID &serverNodeGUID);
	virtual ~CServerAcsSystemIterator(void) {}
private:
	CServerAcsSystemIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出全部的第三方门禁考勤系统节点
class _EXPORT_NODEMANAGER_CLASS CAcsSystemIterator : public CIteratorBase
{
public:
	CAcsSystemIterator(void);
	explicit CAcsSystemIterator(tuint32 dwNodeID);
	explicit CAcsSystemIterator(const GUID &NodeGUID);		
	explicit CAcsSystemIterator(const char *szName);	
	virtual ~CAcsSystemIterator(void) {}
};

//以列表方式迭代出全部的门禁考勤设备节点(包含门禁设备和考勤设备)
class _EXPORT_NODEMANAGER_CLASS CAcsDeviceIterator : public CIteratorBase
{
public:
	CAcsDeviceIterator(void);
	explicit CAcsDeviceIterator(tuint32 dwNodeID);
	explicit CAcsDeviceIterator(const GUID &NodeGUID);		
	explicit CAcsDeviceIterator(const char *szName);		
	virtual ~CAcsDeviceIterator(void) {}
};

//以列表方式迭代出全部的门禁设备节点 仅门禁设备
class _EXPORT_NODEMANAGER_CLASS CAccDeviceIterator : public CIteratorBase
{
public:
	CAccDeviceIterator(void);
	explicit CAccDeviceIterator(tuint32 dwNodeID);
	explicit CAccDeviceIterator(const GUID &NodeGUID);		
	explicit CAccDeviceIterator(const char *szName);		
	virtual ~CAccDeviceIterator(void) {}
};

//以列表方式迭代出全部的考勤设备节点 仅考勤设备
class _EXPORT_NODEMANAGER_CLASS CAttDeviceIterator : public CIteratorBase
{
public:
	CAttDeviceIterator(void);
	explicit CAttDeviceIterator(tuint32 dwNodeID);
	explicit CAttDeviceIterator(const GUID &NodeGUID);		
	explicit CAttDeviceIterator(const char *szName);		
	virtual ~CAttDeviceIterator(void) {}
};

//以列表方式迭代出全部的门节点
class _EXPORT_NODEMANAGER_CLASS CAcsDoorIterator : public CIteratorBase
{
public:
	CAcsDoorIterator(void);
	explicit CAcsDoorIterator(tuint32 dwNodeID);
	explicit CAcsDoorIterator(const GUID &NodeGUID);	
	explicit CAcsDoorIterator(const char *szName);	
	virtual ~CAcsDoorIterator(void) {}
};

//以列表方式迭代出全部的读头节点
class _EXPORT_NODEMANAGER_CLASS CAcsReaderIterator : public CIteratorBase
{
public:
	CAcsReaderIterator(void);
	explicit CAcsReaderIterator(tuint32 dwNodeID);
	explicit CAcsReaderIterator(const GUID &NodeGUID);	
	explicit CAcsReaderIterator(const char *szName);	
	virtual ~CAcsReaderIterator(void) {}
};

//以列表方式迭代出某个门禁考勤系统下的全部门禁设备节点
class _EXPORT_NODEMANAGER_CLASS CAcsSystemAcsDeviceIterator : public CIteratorBase
{
public:
	explicit CAcsSystemAcsDeviceIterator(tuint32 AcsSystemNodeID);
	explicit CAcsSystemAcsDeviceIterator(const GUID &AcsSystemNodeGUID);
	virtual ~CAcsSystemAcsDeviceIterator(void) {}
private:
	CAcsSystemAcsDeviceIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出某个门禁考勤系统下的全部门节点
class _EXPORT_NODEMANAGER_CLASS CAcsSystemAcsDoorIterator : public CIteratorBase
{
public:
	explicit CAcsSystemAcsDoorIterator(tuint32 AcsSystemNodeID);
	explicit CAcsSystemAcsDoorIterator(const GUID &AcsSystemNodeGUID);
	virtual ~CAcsSystemAcsDoorIterator(void) {}
private:
	CAcsSystemAcsDoorIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出某个门禁考勤系统下的全部读头节点
class _EXPORT_NODEMANAGER_CLASS CAcsSystemAcsReaderIterator : public CIteratorBase
{
public:
	explicit CAcsSystemAcsReaderIterator(tuint32 AcsSystemNodeID);
	explicit CAcsSystemAcsReaderIterator(const GUID &AcsSystemNodeGUID);
	virtual ~CAcsSystemAcsReaderIterator(void) {}
private:
	CAcsSystemAcsReaderIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出某个门禁考勤设备下的全部门节点
class _EXPORT_NODEMANAGER_CLASS CAcsDeviceAcsDoorIterator : public CIteratorBase
{
public:
	explicit CAcsDeviceAcsDoorIterator(tuint32 AcsDeviceNodeID);
	explicit CAcsDeviceAcsDoorIterator(const GUID &AcsDeviceNodeID);
	virtual ~CAcsDeviceAcsDoorIterator(void) {}
private:
	CAcsDeviceAcsDoorIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出某个门禁考勤设备下的全部读头节点
class _EXPORT_NODEMANAGER_CLASS CAcsDeviceAcsReaderIterator : public CIteratorBase
{
public:
	explicit CAcsDeviceAcsReaderIterator(tuint32 AcsDeviceNodeID);
	explicit CAcsDeviceAcsReaderIterator(const GUID &AcsDeviceNodeID);
	virtual ~CAcsDeviceAcsReaderIterator(void) {}
private:
	CAcsDeviceAcsReaderIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出门下的全部读头节点
class _EXPORT_NODEMANAGER_CLASS CAcsDoorAcsReaderIterator : public CIteratorBase
{
public:
	explicit CAcsDoorAcsReaderIterator(tuint32 AcsDoorNodeID);
	explicit CAcsDoorAcsReaderIterator(const GUID &AcsDoorNodeID);
	virtual ~CAcsDoorAcsReaderIterator(void) {}
private:
	CAcsDoorAcsReaderIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//PMS相关
//以列表方式迭代出全部停车场
class _EXPORT_NODEMANAGER_CLASS CPmsParkIterator : public CIteratorBase
{
public:
	CPmsParkIterator(void);
	explicit CPmsParkIterator(tuint32 dwNodeID);
	explicit CPmsParkIterator(const GUID &NodeGUID);	
	virtual ~CPmsParkIterator(void) {}
};

//以列表方式迭代出全部出入口
class _EXPORT_NODEMANAGER_CLASS CPmsGatewayIterator : public CIteratorBase
{
public:
	CPmsGatewayIterator(void);
	explicit CPmsGatewayIterator(tuint32 dwNodeID);
	explicit CPmsGatewayIterator(const GUID &NodeGUID);	
	virtual ~CPmsGatewayIterator(void) {}
};

//以列表方式迭代出全部车道
class _EXPORT_NODEMANAGER_CLASS CPmsLaneIterator : public CIteratorBase
{
public:
	CPmsLaneIterator(void);
	explicit CPmsLaneIterator(tuint32 dwNodeID);
	explicit CPmsLaneIterator(const GUID &NodeGUID);	
	virtual ~CPmsLaneIterator(void) {}
};

//以列表方式迭代出全部显示屏
class _EXPORT_NODEMANAGER_CLASS CPmsLedIterator : public CIteratorBase
{
public:
	CPmsLedIterator(void);
	explicit CPmsLedIterator(tuint32 dwNodeID);
	explicit CPmsLedIterator(const GUID &NodeGUID);	
	explicit CPmsLedIterator(const char *szName);	
	virtual ~CPmsLedIterator(void) {}
};

//以列表方式迭代出全部收费规则
class _EXPORT_NODEMANAGER_CLASS CPmsChargeRuleIterator : public CIteratorBase
{
public:
	CPmsChargeRuleIterator(void);
	explicit CPmsChargeRuleIterator(tuint32 dwNodeID);
	explicit CPmsChargeRuleIterator(const GUID &NodeGUID);	
	virtual ~CPmsChargeRuleIterator(void) {}
};

//以列表方式迭代出全部固定车辆
class _EXPORT_NODEMANAGER_CLASS CPmsFixedVehicleIterator : public CIteratorBase
{
public:
	CPmsFixedVehicleIterator(void);
	explicit CPmsFixedVehicleIterator(tuint32 dwNodeID);
	explicit CPmsFixedVehicleIterator(const GUID &NodeGUID);	
	explicit CPmsFixedVehicleIterator(const char *szLicensePlate);	//根据车牌迭代
	virtual ~CPmsFixedVehicleIterator(void) {}
};

//以列表方式迭代出全部黑名单车辆----弃用，合入固定车辆
class _EXPORT_NODEMANAGER_CLASS CPmsBlacklistCarIterator : public CIteratorBase
{
public:
	CPmsBlacklistCarIterator(void);
	explicit CPmsBlacklistCarIterator(tuint32 dwNodeID);
	explicit CPmsBlacklistCarIterator(const GUID &NodeGUID);	
	explicit CPmsBlacklistCarIterator(const char *szLicensePlate);	//根据车牌迭代
	virtual ~CPmsBlacklistCarIterator(void) {}
};
//以列表方式迭代出某个车辆组下的全部车辆
class _EXPORT_NODEMANAGER_CLASS CPmsGroupChildCarIterator : public CIteratorBase
{
public:
	explicit CPmsGroupChildCarIterator(tuint32 groupNodeID);
	explicit CPmsGroupChildCarIterator(const GUID &groupNodeGUID);
	virtual ~CPmsGroupChildCarIterator(void) {}
private:
	CPmsGroupChildCarIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出全部车辆组
class _EXPORT_NODEMANAGER_CLASS CPmsVehicleGroupIterator : public CIteratorBase
{
public:
	CPmsVehicleGroupIterator(void);
	explicit CPmsVehicleGroupIterator(tuint32 dwNodeID);
	explicit CPmsVehicleGroupIterator(const GUID &NodeGUID);
	virtual ~CPmsVehicleGroupIterator(void) {}
};

//以列表方式迭代出全部自定义收费时间段
class _EXPORT_NODEMANAGER_CLASS CPmsTimeSectionChargeRuleIterator : public CIteratorBase
{
public:
	CPmsTimeSectionChargeRuleIterator(void);
	explicit CPmsTimeSectionChargeRuleIterator(tuint32 dwNodeID);
	explicit CPmsTimeSectionChargeRuleIterator(const GUID &NodeGUID);	
	virtual ~CPmsTimeSectionChargeRuleIterator(void) {}
};

//以列表方式迭代出全部月卡包月收费
class _EXPORT_NODEMANAGER_CLASS CPmsBagPeriodIterator : public CIteratorBase
{
public:
	CPmsBagPeriodIterator(void);
	explicit CPmsBagPeriodIterator(tuint32 dwNodeID);
	explicit CPmsBagPeriodIterator(const GUID &NodeGUID);	
	virtual ~CPmsBagPeriodIterator(void) {}
};
//以列表方式迭代出全部通行记录
class _EXPORT_NODEMANAGER_CLASS CPmsPassRecordsIterator : public CIteratorBase
{
public:
	CPmsPassRecordsIterator(void);
	explicit CPmsPassRecordsIterator(tuint32 dwNodeID);
	explicit CPmsPassRecordsIterator(const GUID &NodeGUID);	
	virtual ~CPmsPassRecordsIterator(void) {}
};

//以列表方式迭代出停车场下的全部出入口节点
class _EXPORT_NODEMANAGER_CLASS CPmsParkPmsGatewayIterator : public CIteratorBase
{
public:
	explicit CPmsParkPmsGatewayIterator(tuint32 PmsParkNodeID);
	explicit CPmsParkPmsGatewayIterator(const GUID &PmsParkNodeID);
	virtual ~CPmsParkPmsGatewayIterator(void) {}
private:
	CPmsParkPmsGatewayIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出出入口下的全部车道节点
class _EXPORT_NODEMANAGER_CLASS CPmsGatewayPmsLaneIterator : public CIteratorBase
{
public:
	explicit CPmsGatewayPmsLaneIterator(tuint32 PmsGatewayNodeID);
	explicit CPmsGatewayPmsLaneIterator(const GUID &PmsGatewayNodeID);
	virtual ~CPmsGatewayPmsLaneIterator(void) {}
private:
	CPmsGatewayPmsLaneIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出全部的级联平台节点
class _EXPORT_NODEMANAGER_CLASS CCascadePlatformIterator : public CIteratorBase
{
public:
	CCascadePlatformIterator(void);
	explicit CCascadePlatformIterator(tuint32 dwNodeID);
	explicit CCascadePlatformIterator(const GUID &NodeGUID);
	explicit CCascadePlatformIterator(const char *szName);
	explicit CCascadePlatformIterator(bool bGBID,const char *szGBID);//用于根据国标ID迭代
	virtual ~CCascadePlatformIterator(void) {}
};

//以列表方式迭代出级联平台下的全部设备节点
class _EXPORT_NODEMANAGER_CLASS CCascadePlatformDeviceIterator : public CIteratorBase
{
public:
	explicit CCascadePlatformDeviceIterator(tuint32 CascadePlatformID);
	explicit CCascadePlatformDeviceIterator(const GUID &CascadePlatformID);
	virtual ~CCascadePlatformDeviceIterator(void) {}
private:
	CCascadePlatformDeviceIterator(void) {}//把默认构造函数隐藏起来不让访问，因为此时没有任何意义
};

//以列表方式迭代出全部配置项
class _EXPORT_NODEMANAGER_CLASS CConfigIterator : public CIteratorBase
{
public:
	CConfigIterator(void);
	explicit CConfigIterator(const GUID &ConfigID);
	explicit CConfigIterator(const tuint32 ConfigKey);
	virtual ~CConfigIterator(void) {}
};

class _EXPORT_NODEMANAGER_CLASS CCustomIterator : public CIteratorBase
{
public:
	CCustomIterator(void);
	explicit CCustomIterator(const GUID &ConfigID);
	explicit CCustomIterator(const tuint32 CustomIdx);
	virtual ~CCustomIterator(void) {}
};


//以列表方式迭代出全部的处警标准操作指导
class _EXPORT_NODEMANAGER_CLASS CSopIterator : public CIteratorBase
{
public:
	CSopIterator(void);
	explicit CSopIterator(tuint32 dwNodeID);
	explicit CSopIterator(const GUID &NodeGUID);
	explicit CSopIterator(const char *szName);			
	virtual ~CSopIterator(void) {}
};

//以列表方式迭代出全部的告警任务组节点
class _EXPORT_NODEMANAGER_CLASS CAlarmTaskIterator : public CIteratorBase
{
public:
	CAlarmTaskIterator(void);
	explicit CAlarmTaskIterator(tuint32 dwNodeID);
	explicit CAlarmTaskIterator(const GUID &NodeGUID);
	explicit CAlarmTaskIterator(const char *szName);	
	explicit CAlarmTaskIterator(bool bType/*是否为根据类型获取报警任务 ECMS_TASK_TYPE*/,tuint32 nType);
	virtual ~CAlarmTaskIterator(void) {}
};

//以列表方式迭代出全部的邮件地址
class _EXPORT_NODEMANAGER_CLASS CEmailIterator : public CIteratorBase
{
public:
	CEmailIterator(void);
	explicit CEmailIterator(tuint32 dwNodeID);
	explicit CEmailIterator(const GUID &NodeGUID);
	explicit CEmailIterator(bool bType, bool type);	//0:收件，1 发件
	explicit CEmailIterator(bool type, const char *szAddr);	
	virtual ~CEmailIterator(void) {}
};

//以列表方式迭代出全部的管理数据库中的目标库节点 目标库
class _EXPORT_NODEMANAGER_CLASS CRsuAlbumIterator : public CIteratorBase
{
public:
	CRsuAlbumIterator(void);
	explicit CRsuAlbumIterator(tuint32 dwNodeID);
    explicit CRsuAlbumIterator(const GUID &NodeGUID);
    virtual ~CRsuAlbumIterator(void) {}
};

#endif //__ITERATOR_H__
