
#ifndef __CONFIG__PACK__
#define __CONFIG__PACK__
/////////把配置数据打包给到网络上,不同的节点此类会不一样
#include "ConfigPack_Struct_Def.h"
#include "ConfigPack_Data_Item_ID_Def.h"
#include "NVMS_Define.h"
#include <set>
#include <map>
#include <list>


enum CONFIG_PACK_ATTRIB
{
	CONFIG_PACK_LOGIN,//登陆传输的数据包
	CONFIG_PACK_UPDATE,//配置更新数据包
}
;

class CConfigPack
{
public:
	typedef struct _data_block_info 
	{
		char *pData;/////////配置块数据信息
		int   dataLen;//////信息的长度
	}DATA_BLOCK_INFO;
	typedef std::list<CConfigPack::DATA_BLOCK_INFO> DATA_BLOCK_LIST;
	////////////通过构造函数去构造 配置数据包
	CConfigPack(void);
public:
	virtual ~CConfigPack(void);
	CConfigPack* GetSelfCopy(); //得到自己的一份拷贝，外部自己释放此对象

	void SetConfigPackAttrib(CONFIG_PACK_ATTRIB configAttrib);
	CONFIG_PACK_ATTRIB GetConfigPackAttrib(){return m_ConfigPackAttrib;};
	//////////通过函数去修改配置数据包
	int SetConfigBlockByID(const char *pDataBlock,int len);
    int SetConfigBlockByID_Lite(const char *pDataBlock,int len);//键盘用到

	int SetConfigBlockByItem(const CFGPACK_DEFINE::NCFG_ITEM_HEAD &itemHead, const char *pData, int len);

	int SetConfigBlockByItem(const CFGPACK_DEFINE::NCFG_ITEM_HEAD &itemHead, const char *pData);				//以后用这个函数

	int SetConfigBlockByItem(CConfigPack &configPack, const int  nItemID, const int dwItemSize, char *pBuff);

	/////////获取对应的ID的配置数据,需要与应用程序相关
	virtual int GetConfigBlockByID(const CFGPACK_DEFINE::NCFG_ITEM_HEAD * pHead, int headNum, char **ppDataOut, int *pLen);

	/////////////释放通过GetConfigBlockByID  和 GetBlockBuf返回的数据缓冲区
	virtual void ReleaseConfigData(char *&pData) const;

	////////获取整个配置块的Buf
	int GetBlockBuf(char *&pDataBuf) const;
	int GetBlockBufLen() const;

	//清除一些配置ID信息
	bool ClearConfigInfo(const std::set<tuint16/*项的ID*/> &itemIDSet);
	//清除有目标的信息
	bool ClearHasDestInfo();
	//得到一些ID的配置信息
	int	 GetConfigInfo(const std::set<tuint16/*项的ID*/> &itemIDSet,std::list<DATA_BLOCK_INFO> &ConfigInfoList);

	bool DeleteBlockInfo(CConfigPack::DATA_BLOCK_INFO &BlockInfo);

	int  GetConfigDataList(std::list<CConfigPack::DATA_BLOCK_INFO> &configDataList) const;

	
	template<class ItemType>	//单项内容打成数据包
	void AddOneItemInfo(const ItemType			&ItemData,								//要打包的数据
						tuint32 ItemID,								//项ID
						CFGPACK_DEFINE::OPERATOR_TYPE	OperatorType,				//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
						GUID dwDestNode = GUID_NULL/*tuint32			dwDestNode = NVMS_DEFINE::INVALID_NODE_ID*/)	//发送的目标节点
	{
		tuint32 ulItemLen = static_cast<tuint32>(sizeof(ItemType));

		DATA_BLOCK_INFO BlockInfo;
		memset(&BlockInfo,0,sizeof(BlockInfo));
		BlockInfo.dataLen = sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD) + ulItemLen;

		BlockInfo.pData = new char[BlockInfo.dataLen];
		memset(BlockInfo.pData, 0, BlockInfo.dataLen);

		CFGPACK_DEFINE::NCFG_ITEM_HEAD &ItemHead = *(reinterpret_cast<CFGPACK_DEFINE::NCFG_ITEM_HEAD *>(BlockInfo.pData));
		ItemHead.itemID = ItemID;					//项ID
		ItemHead.num = 1;							//元素数目
		ItemHead.subLen = ulItemLen;				//每个元素的长度
		ItemHead.operatorType = OperatorType;		//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
		ItemHead.len = ItemHead.subLen;				//数据总长度,因为只有一个,所以与单项长度相等
		ItemHead.destNodeID = dwDestNode;			//发送的目标节点

		memcpy(BlockInfo.pData + sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD), &ItemData, ItemHead.subLen);

		AddBlockInfo(ItemID, BlockInfo);
	}

	template<class ItemType>	//多项内容打成数据包
	void AddListItemInfo(const std::list<ItemType>	&ItemList,									//要打包的数据
						 tuint32 ItemID,									//项ID
						 CFGPACK_DEFINE::OPERATOR_TYPE	OperatorType,						//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
						GUID dwDestNode = GUID_NULL/* tuint32				dwDestNode = NVMS_DEFINE::INVALID_NODE_ID*/)	//发送的目标节点
	{
		if (!ItemList.empty())
		{
			CFGPACK_DEFINE::NCFG_ITEM_HEAD ItemHead;
			memset(&ItemHead,0,sizeof(ItemHead));

			ItemHead.itemID = ItemID;						//项ID
			if (ItemList.size() >= MAXWORD)					//超出部分直接丢掉
			{
				ItemHead.num = MAXWORD;
			}
			else
			{
				ItemHead.num = static_cast<unsigned short>(ItemList.size());		//元素数目
			}
			
			ItemHead.subLen = static_cast<tuint32>(sizeof(ItemType));	//每个元素的长度
			ItemHead.operatorType = OperatorType;			//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
			ItemHead.len = ItemHead.subLen * ItemHead.num;	//数据总长度
			ItemHead.destNodeID = dwDestNode;				//发送的目标节点

			DATA_BLOCK_INFO BlockInfo;
			memset(&BlockInfo,0,sizeof(BlockInfo));
			BlockInfo.dataLen = sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD) + ItemHead.len;

			BlockInfo.pData = new char[BlockInfo.dataLen];
			memset(BlockInfo.pData, 0, BlockInfo.dataLen);

			memcpy(BlockInfo.pData, &ItemHead, sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD));

			ItemType *pItemData = reinterpret_cast<ItemType *>(BlockInfo.pData + sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD));

			typename std::list<ItemType>::const_iterator Iter = ItemList.begin();
			for (int i=0; (i<ItemHead.num) && (Iter != ItemList.end()); ++Iter, ++i)
			{
				*pItemData = *Iter;
				pItemData++;
			}

			AddBlockInfo(ItemID, BlockInfo);
		}	
	}

	template<class ItemType>	//单项内容+附加数据块打成数据包
	void AddOneItemAndAdditionalBufInfo(const ItemType &ItemData,							//要打包的数据
										tuint32 ItemID,								//项ID
										CFGPACK_DEFINE::OPERATOR_TYPE OperatorType,			//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
										const char *pBuf, tuint32 ulBufLen,			//附加的数据块
										GUID dwDestNode = GUID_NULL/*tuint32 dwDestNode = NVMS_DEFINE::INVALID_NODE_ID*/)	//发送的目标节点
	{
		tuint32 ulItemLen = static_cast<tuint32>(sizeof(ItemType));

		DATA_BLOCK_INFO BlockInfo;
		memset(&BlockInfo,0,sizeof(BlockInfo));
		BlockInfo.dataLen = sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD) + ulItemLen + ulBufLen;

		BlockInfo.pData = new char[BlockInfo.dataLen];
		memset(BlockInfo.pData, 0, BlockInfo.dataLen);

		CFGPACK_DEFINE::NCFG_ITEM_HEAD &ItemHead = *(reinterpret_cast<CFGPACK_DEFINE::NCFG_ITEM_HEAD *>(BlockInfo.pData));
		ItemHead.itemID = ItemID;					//项ID
		ItemHead.num = 1;							//元素数目
		ItemHead.subLen = ulItemLen;				//项的长度
		ItemHead.operatorType = OperatorType;		//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
		ItemHead.len = ulItemLen + ulBufLen;		//数据总长度,项长度+Buf长度
		ItemHead.destNodeID = dwDestNode;			//发送的目标节点

		memcpy(BlockInfo.pData + sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD), &ItemData, ItemHead.subLen);

		if (pBuf != NULL && ulBufLen > 0)
		{
			memcpy(BlockInfo.pData+sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD)+ulItemLen, pBuf, ulBufLen);
		}

		AddBlockInfo(ItemID, BlockInfo);
	}

	//数据块打成数据包
	void AddOneItemAndAdditionalBufInfo(tuint32 ItemID,								//项ID
		CFGPACK_DEFINE::OPERATOR_TYPE OperatorType,			//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
		const char *pBuf, tuint32 ulBufLen,			//附加的数据块
		GUID destNode = GUID_NULL/*tuint32 dwDestNode = NVMS_DEFINE::INVALID_NODE_ID*/)	//发送的目标节点
	{
		DATA_BLOCK_INFO BlockInfo;
		memset(&BlockInfo,0,sizeof(BlockInfo));
		BlockInfo.dataLen = sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD) + ulBufLen;

		BlockInfo.pData = new char[BlockInfo.dataLen];
		memset(BlockInfo.pData, 0, BlockInfo.dataLen);

		CFGPACK_DEFINE::NCFG_ITEM_HEAD &ItemHead = *(reinterpret_cast<CFGPACK_DEFINE::NCFG_ITEM_HEAD *>(BlockInfo.pData));
		ItemHead.itemID = ItemID;					//项ID
		ItemHead.num = 1;							//元素数目
		ItemHead.subLen = ulBufLen;					//项的长度
		ItemHead.operatorType = OperatorType;		//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
		ItemHead.len = ulBufLen;					//数据总长度,项长度+Buf长度
		ItemHead.destNodeID = destNode/*dwDestNode*/;			//发送的目标节点

		if (pBuf != NULL && ulBufLen > 0)
		{
			memcpy(BlockInfo.pData+sizeof(CFGPACK_DEFINE::NCFG_ITEM_HEAD), pBuf, ulBufLen);
		}

		AddBlockInfo(ItemID, BlockInfo);
	}
	//获取指定项的所有数据块
	void GetItemBlockInfoList(tuint32 ItemID, std::list<DATA_BLOCK_INFO> &BlockInfoList) const;

private:
	////////////项的Copy
	void ItemListCopy(int itemID, std::list<DATA_BLOCK_INFO> &destList) const;

	///////////修改项的块数据
	int SetConfigBlock(int headNum, const CFGPACK_DEFINE::NCFG_ITEM_HEAD *pItemHead, const char *pBuf);

    ///////////修改项的块数据小内存 键盘用到
    int SetConfigBlock_Lite(int headNum, const CFGPACK_DEFINE::NCFG_ITEM_HEAD *pItemHead, const char *pBuf);

	////////获取块信息
	int GetBlockInfo(int &bufLen, int &itemCount) const;

	void AddBlockInfo(CFGPACK_DEFINE::DATA_ITEM_ID ItemID, const DATA_BLOCK_INFO &BlockInfo);
	void AddBlockInfo(int ItemID, const DATA_BLOCK_INFO &BlockInfo);
	//////////数据块的列表
	std::list<CConfigPack::DATA_BLOCK_INFO>	m_ConfigDataList;
	//////////保存的配置信息，每个ID对应一个List，List的每个元素对应具有一个头的多个元素的内存
	std::map<tuint32, std::list<DATA_BLOCK_INFO> *>   m_ConfigDataMap;


	CONFIG_PACK_ATTRIB m_ConfigPackAttrib;
};

#endif //__CONFIG__PACK__