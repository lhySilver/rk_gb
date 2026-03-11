
#ifndef __BLOCK_INFO_ITERATOR__
#define __BLOCK_INFO_ITERATOR__
#include "ConfigPack.h"
#include "ConfigPack_Data_Item_ID_Def.h"

class CBlockInfoIterator
{
public:
	CBlockInfoIterator(tuint32 ItemID, const CConfigPack &ConfigPack)
	{
		ConfigPack.GetItemBlockInfoList(ItemID, m_BlockInfoList);
		m_Iter = m_BlockInfoList.begin();
	}

	explicit CBlockInfoIterator(const CConfigPack &ConfigPack)
	{
		ConfigPack.GetConfigDataList(m_BlockInfoList);
		m_Iter = m_BlockInfoList.begin();
	}

	~CBlockInfoIterator(void) {}

	std::list<CConfigPack::DATA_BLOCK_INFO>::size_type GetBlockInfoNum(void)		//这个函数只用于断言,其他没有使用的必要
	{
		return m_BlockInfoList.size();
	}

	bool HasNext(void)
	{
		return m_Iter != m_BlockInfoList.end();
	}

	const CConfigPack::DATA_BLOCK_INFO &GetNext(void)
	{
		ASSERT(m_Iter != m_BlockInfoList.end());
		return *m_Iter++;
	}

private:
	std::list<CConfigPack::DATA_BLOCK_INFO> m_BlockInfoList;
	std::list<CConfigPack::DATA_BLOCK_INFO>::const_iterator m_Iter;
};

class CExplainBlock
{
public:
	explicit CExplainBlock(const CConfigPack::DATA_BLOCK_INFO &BlockInfo) : m_BlockInfo(BlockInfo), \
		m_ItemHead(*reinterpret_cast<const CFGPACK_DEFINE::NCFG_ITEM_HEAD *>(BlockInfo.pData))
	{
		ASSERT((int)(m_ItemHead.len + CFGPACK_DEFINE::NCFG_ITEM_HEAD_LENGTH) == m_BlockInfo.dataLen);		//块长度检查
		m_usIndex = 0;
	}

	~CExplainBlock(void) {}

	tuint32 GetItemID(void) const
	{
		return m_ItemHead.itemID;
	}

	tuint16 GetOperatorType(void) const
	{
		return m_ItemHead.operatorType;
	}

	GUID GetDestNodeID(void) const
	{
		return m_ItemHead.destNodeID;
	}
// 	tuint32 GetDestNodeID(void) const
// 	{
// 		return m_ItemHead.destNodeID;
// 	}

	tuint16 GetItemNum(void) const		//这个函数只用于断言,其他没有使用的必要
	{
		return m_ItemHead.num;
	}

	bool HasNext(void) const
	{
		return (m_usIndex < m_ItemHead.num);
	}

 	template<class ItemType>
 	const ItemType &GetNext(void) const
 	{
		ASSERT(m_usIndex < m_ItemHead.num);
 		ASSERT(m_ItemHead.subLen == sizeof(ItemType));					//项的长度与对应类型的长度一定相等
 		ASSERT(m_ItemHead.len == m_ItemHead.subLen * m_ItemHead.num);	//数据总长度检查
 
 		const ItemType &ItemInfo = *(reinterpret_cast<const ItemType *>(m_BlockInfo.pData + CFGPACK_DEFINE::NCFG_ITEM_HEAD_LENGTH + m_ItemHead.subLen*m_usIndex));
		(const_cast<CExplainBlock *>(this))->m_usIndex++;
 
 		return ItemInfo;
	}

	template<class ItemType>
	ItemType &GetNext(void)
	{
		ASSERT(m_usIndex < m_ItemHead.num);
		ASSERT(m_ItemHead.subLen == sizeof(ItemType));					//项的长度与对应类型的长度一定相等
		ASSERT(m_ItemHead.len == m_ItemHead.subLen * m_ItemHead.num);	//数据总长度检查

		ItemType &ItemInfo = *(reinterpret_cast<ItemType *>(m_BlockInfo.pData + CFGPACK_DEFINE::NCFG_ITEM_HEAD_LENGTH + m_ItemHead.subLen*m_usIndex));
		m_usIndex++;

		return ItemInfo;
	}

	template<class ItemType>
	const ItemType &GetNext(const char *&pBuf, tuint32 &ulBufLen) const
	{
		ASSERT(m_usIndex < m_ItemHead.num);
		ASSERT(m_ItemHead.num == 1);									//这个只能有一项
		ASSERT(m_ItemHead.subLen == sizeof(ItemType));					//项的长度与对应类型的长度一定相等
		ASSERT(m_ItemHead.len >= m_ItemHead.subLen);					//数据总长度检查

		const ItemType &ItemInfo = *(reinterpret_cast<const ItemType *>(m_BlockInfo.pData + CFGPACK_DEFINE::NCFG_ITEM_HEAD_LENGTH + m_ItemHead.subLen*m_usIndex));
		(const_cast<CExplainBlock *>(this))->m_usIndex++;

		ulBufLen = m_ItemHead.len - m_ItemHead.subLen;
		if (ulBufLen > 0)
		{
			pBuf = m_BlockInfo.pData + CFGPACK_DEFINE::NCFG_ITEM_HEAD_LENGTH + sizeof(ItemType);
		}
		else
		{
			pBuf = NULL;
		}

		return ItemInfo;
	}
	bool GetNext(const char *&pBuf, tuint32 &ulBufLen) const
	{
		ASSERT(m_usIndex < m_ItemHead.num);
		ASSERT(m_ItemHead.num == 1);									//这个只能有一项
		ASSERT(m_ItemHead.subLen == m_ItemHead.len);					//项的长度与对应类型的长度一定相等
		//ASSERT(m_ItemHead.len >= m_ItemHead.subLen);					//数据总长度检查

		(const_cast<CExplainBlock *>(this))->m_usIndex++;

		ulBufLen = m_ItemHead.len;
		if (ulBufLen > 0)
		{
			pBuf = m_BlockInfo.pData + CFGPACK_DEFINE::NCFG_ITEM_HEAD_LENGTH;
			return true;
		}
		else
		{
			pBuf = NULL;
			return false;
		}
	}
private:
	unsigned short m_usIndex;
	const CConfigPack::DATA_BLOCK_INFO &m_BlockInfo;
	const CFGPACK_DEFINE::NCFG_ITEM_HEAD &m_ItemHead;
};

/*************************************用例
//特定类型
CConfigPack ConfigPack;
CBlockInfoIterator MSUBlaockInfoIter(NCFG_ITEM_STORAGE_INFO, ConfigPack);
while (MSUBlaockInfoIter.HasNext())
{
	CExplainBlock ExplainBlock(MSUBlaockInfoIter.GetNext());
	while (ExplainBlock.HasNext())
	{
		const NCFG_STORAGE_INFO &MSUInfo = ExplainBlock.GetNext<NCFG_STORAGE_INFO>();
	}
}

//所有类型
CConfigPack ConfigPack;
CBlockInfoIterator AllBlaockInfoIter(ConfigPack);
while (AllBlaockInfoIter.HasNext())
{
	CExplainBlock ExplainBlock(AllBlaockInfoIter.GetNext());
	switch (ExplainBlock.GetItemID())
	{
	case NCFG_ITEM_STORAGE_INFO:
		switch (ExplainBlock.GetOperatorType())
		{
		case CREATE_ITEM:
			while (ExplainBlock.HasNext())
			{
				const NCFG_STORAGE_INFO &MSUInfo = ExplainBlock.GetNext<NCFG_STORAGE_INFO>();
			}
			break;

		default:
			ASSERT(FALSE);
			break;
		}

		break;

	default:
		ASSERT(FALSE);
		break;
	}	
}
*****************************************/

#endif //__BLOCK_INFO_ITERATOR__