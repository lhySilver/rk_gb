#include <algorithm>
#include "ConfigPack.h"
#include "TypeCheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace CFGPACK_DEFINE;

CConfigPack::CConfigPack(void)
{
	m_ConfigPackAttrib=CONFIG_PACK_UPDATE;//在没有严格要求的情况下可以认为全部是配置更新包
}

CConfigPack::~CConfigPack(void)
{
	//////////////销毁内存
	//清除索引的项
    std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator iter = m_ConfigDataMap.begin();
    while (iter != m_ConfigDataMap.end())
    {
		if(NULL != iter->second)
		{
			while (!iter->second->empty())
			{
				std::list<DATA_BLOCK_INFO>::iterator iterlist = iter->second->begin();
                delete []iterlist->pData;
                iterlist->pData = NULL;
				iter->second->erase(iterlist);
			}

            delete iter->second;
            iter->second = NULL;
		}

        iter++;
	}

    m_ConfigDataMap.clear();
	////////////清除顺序列表的项
    m_ConfigDataList.clear();
}

//得到自己的一份拷贝，外部自己释放此对象
CConfigPack* CConfigPack::GetSelfCopy()
{
	CConfigPack *pConfigPack=new CConfigPack;
	char *pDataBuf=NULL;
	int len=GetBlockBuf(pDataBuf);
	if (pDataBuf!=NULL)
	{
		pConfigPack->SetConfigBlockByID(pDataBuf,len);	
	}	
	ReleaseConfigData(pDataBuf);
	pConfigPack->SetConfigPackAttrib(m_ConfigPackAttrib);
	return pConfigPack;
}

void CConfigPack::SetConfigPackAttrib(CONFIG_PACK_ATTRIB configAttrib)
{
	m_ConfigPackAttrib=configAttrib;
}

///////////////组合配置数据信息,
int CConfigPack::SetConfigBlockByID(const char *pDataBlock,int len)
{
	const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD *>(pDataBlock);
	
	const NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD *>(pDataBlock + sizeof(NCFG_BLOCK_HEAD));
	
	int headNum = pBlockHead->ItemNum;

	/////配置数据Buf
	const char *pBuf = pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*headNum;

	return SetConfigBlock(headNum, pItemHead, pBuf);
}

///////////////组合配置数据信息,
int CConfigPack::SetConfigBlockByID_Lite(const char *pDataBlock,int len)
{
    const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD *>(pDataBlock);

    const NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD *>(pDataBlock + sizeof(NCFG_BLOCK_HEAD));

    int headNum = pBlockHead->ItemNum;

    /////配置数据Buf
    const char *pBuf = pDataBlock + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*headNum;

    return SetConfigBlock_Lite(headNum, pItemHead, pBuf);
}

int CConfigPack::SetConfigBlock(int headNum, const NCFG_ITEM_HEAD *pItemHead, const char *pBuf)
{
	ASSERT(headNum > 0);
	ASSERT(NULL != pItemHead);
	ASSERT(NULL != pBuf);

	/////配置数据Buf
	tuint32 *pCfgOffset = new tuint32[headNum];
	memset(pCfgOffset,0, sizeof(tuint32)*headNum);

	//计算每个配置项，相对于配置数据起始地址的偏移位置
	int  itemcount = 0;
	for(itemcount = 0; itemcount < headNum; ++itemcount)
	{
		if(itemcount > 0)
		{
			pCfgOffset[itemcount] = pCfgOffset[itemcount-1] + pItemHead[itemcount-1].len;
		}
		else
		{
			pCfgOffset[itemcount] = 0;
		}
	}

	for(itemcount = 0; itemcount < headNum; ++itemcount)
	{
		std::list<DATA_BLOCK_INFO>   *pItemList = NULL;	
		std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator iter=m_ConfigDataMap.find(pItemHead[itemcount].itemID);
		if( iter ==  m_ConfigDataMap.end())
		{
			//////////如果节点的相关内容不存在
			pItemList = new std::list<DATA_BLOCK_INFO>;
			m_ConfigDataMap.insert(std::pair<tuint32, std::list<DATA_BLOCK_INFO> *>(pItemHead[itemcount].itemID, pItemList));			
		}
		else
		{
			pItemList=iter->second;
		}	

		ASSERT(NULL != pItemList);

		DATA_BLOCK_INFO   DataInfo;
		memset(&DataInfo,0,sizeof(DataInfo));
		DataInfo.dataLen = sizeof(NCFG_ITEM_HEAD) + pItemHead[itemcount].len;
		DataInfo.pData = new char[DataInfo.dataLen];
		memcpy(DataInfo.pData, &pItemHead[itemcount], sizeof(NCFG_ITEM_HEAD));
		memcpy(DataInfo.pData + sizeof(NCFG_ITEM_HEAD), pBuf + pCfgOffset[itemcount], pItemHead[itemcount].len);

		pItemList->push_back(DataInfo);

        m_ConfigDataList.push_back(DataInfo);

	}


	delete [] pCfgOffset;

	pCfgOffset = NULL;

	return 0;
}

bool IsLiteNodeManagerType(tuint32 dwType)
{
    CTypeCheck TypeCheck(dwType);

    return  TypeCheck.IsChannel() ||
            TypeCheck.IsArea() ||
        TypeCheck.IsPU() || \
        TypeCheck.IsADU() || \
        TypeCheck.IsEMU() || \
        TypeCheck.IsGPSServer() || \
        TypeCheck.IsGSU() || \
        TypeCheck.IsMDU() || \
        TypeCheck.IsMSU() || \
        TypeCheck.IsNVMS1000() || \
        TypeCheck.IsPCNVR9000() || \
        TypeCheck.IsMediaServer() || \
        TypeCheck.IsWall() || \
        TypeCheck.IsChannelGroup() || \
		TypeCheck.IsAlarmTaskGroup() || \
		TypeCheck.IsAlarmOutGroup() || \
        TypeCheck.IsRSU();
}

int CConfigPack::SetConfigBlock_Lite(int headNum, const CFGPACK_DEFINE::NCFG_ITEM_HEAD *pItemHead, const char *pBuf)
{
    ASSERT(headNum > 0);
    ASSERT(NULL != pItemHead);
    ASSERT(NULL != pBuf);

    /////配置数据Buf
    tuint32 *pCfgOffset = new tuint32[headNum];
    memset(pCfgOffset,0, sizeof(tuint32)*headNum);

    //计算每个配置项，相对于配置数据起始地址的偏移位置
    int  itemcount = 0;
    for(itemcount = 0; itemcount < headNum; ++itemcount)
    {
        if(itemcount > 0)
        {
            pCfgOffset[itemcount] = pCfgOffset[itemcount-1] + pItemHead[itemcount-1].len;
        }
        else
        {
            pCfgOffset[itemcount] = 0;
        }
    }

    for(itemcount = 0; itemcount < headNum; ++itemcount)
    {

        std::list<DATA_BLOCK_INFO>   *pItemList = NULL;

        tuint32 itemID =  pItemHead[itemcount].itemID;

        if( (itemID == CFGPACK_DEFINE::DATA_ITEM_DEVICE_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_CHANNELGROUP_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_WALL_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_CH_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_SERVER_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_BINARY_RELATION_LOGIC)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_BINARY_RELATION_PHYSICS)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_USER_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_USER_RIGHT)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_CHANNEL_GROUP_RELATION_INFO)||\
			(itemID == CFGPACK_DEFINE::DATA_ITEM_ALARMOUTGROUP_INFO)||\
			 (itemID == CFGPACK_DEFINE::DATA_ITEM_ALARMOUT_GROUP_RELATION_INFO)||\
            (itemID == CFGPACK_DEFINE::DATA_ITEM_AREA_INFO))
        {
            std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator iter=m_ConfigDataMap.find(pItemHead[itemcount].itemID);
            if( iter ==  m_ConfigDataMap.end())
            {
                //////////如果节点的相关内容不存在
                pItemList = new std::list<DATA_BLOCK_INFO>;
                m_ConfigDataMap.insert(std::pair<tuint32, std::list<DATA_BLOCK_INFO> *>(pItemHead[itemcount].itemID, pItemList));
            }
            else
            {
                pItemList=iter->second;
            }

            ASSERT(NULL != pItemList);

            if( (itemID == CFGPACK_DEFINE::DATA_ITEM_BINARY_RELATION_LOGIC)||(itemID == CFGPACK_DEFINE::DATA_ITEM_BINARY_RELATION_PHYSICS) )
            {
                tuint32 nNeedCount = 0;
                tuint32 nNum = pItemHead[itemcount].num;
                if(nNum*sizeof(CFGPACK_DEFINE::BINARY_RELATION) == pItemHead[itemcount].len )
                {
                    CFGPACK_DEFINE::BINARY_RELATION tempST1;
                    for(tuint32 i = 0 ;i<nNum;i++)
                    {
                        memset(&tempST1,0,sizeof(CFGPACK_DEFINE::BINARY_RELATION));
                        memcpy(&tempST1,pBuf + pCfgOffset[itemcount]+i*sizeof(CFGPACK_DEFINE::BINARY_RELATION),sizeof(CFGPACK_DEFINE::BINARY_RELATION));
                        if(IsLiteNodeManagerType(tempST1.childNodeType) == true)
                        {
                            nNeedCount++;
                        }
                    }
                    DATA_BLOCK_INFO   DataInfo;
                    memset(&DataInfo,0,sizeof(DataInfo));
                    DataInfo.dataLen = sizeof(NCFG_ITEM_HEAD) + nNeedCount*sizeof(CFGPACK_DEFINE::BINARY_RELATION);
                    DataInfo.pData = new char[DataInfo.dataLen];
                    NCFG_ITEM_HEAD headST;
                    memcpy(&headST,&pItemHead[itemcount],sizeof(NCFG_ITEM_HEAD));
                    headST.num = nNeedCount;
                    headST.len = nNeedCount*sizeof(CFGPACK_DEFINE::BINARY_RELATION);

                    memcpy(DataInfo.pData, &headST, sizeof(NCFG_ITEM_HEAD));

                    tuint32 nAlreadyAdd = 0;//已经添加了的数量
                    CFGPACK_DEFINE::BINARY_RELATION tempST2;
                    for(tuint32 i = 0 ;i<nNum;i++)
                    {
                        memset(&tempST2,0,sizeof(CFGPACK_DEFINE::BINARY_RELATION));
                        memcpy(&tempST2,pBuf + pCfgOffset[itemcount]+i*sizeof(CFGPACK_DEFINE::BINARY_RELATION),sizeof(CFGPACK_DEFINE::BINARY_RELATION));
                        if(IsLiteNodeManagerType(tempST2.childNodeType) == true)
                        {
                            memcpy(DataInfo.pData + sizeof(NCFG_ITEM_HEAD) +nAlreadyAdd*sizeof(CFGPACK_DEFINE::BINARY_RELATION) ,\
                                   &tempST2, sizeof(CFGPACK_DEFINE::BINARY_RELATION));
                            nAlreadyAdd++;
                        }
                    }

                    pItemList->push_back(DataInfo);

                    m_ConfigDataList.push_back(DataInfo);

                    continue;//下一个for循环
                }

            }
            else if( (itemID == CFGPACK_DEFINE::DATA_ITEM_DEVICE_INFO) )
            {
                tuint32 nNum = pItemHead[itemcount].num;

                if(nNum*sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO) == pItemHead[itemcount].len)
                {
                    DATA_BLOCK_INFO   DataInfo;
                    memset(&DataInfo,0,sizeof(DataInfo));
                    DataInfo.dataLen = sizeof(NCFG_ITEM_HEAD) + nNum*sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO_Lite);
                    DataInfo.pData = new char[DataInfo.dataLen];
                    NCFG_ITEM_HEAD headST;
                    memcpy(&headST,&pItemHead[itemcount],sizeof(NCFG_ITEM_HEAD));
                    headST.len = nNum*sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO_Lite);
                    headST.subLen = sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO_Lite);
                    memcpy(DataInfo.pData, &headST, sizeof(NCFG_ITEM_HEAD));

                    tuint32 nAlreadyAdd = 0;//已经添加了的数量
                    CFGPACK_DEFINE::RES_DEVICE_INFO srcST;
                    CFGPACK_DEFINE::RES_DEVICE_INFO_Lite dstST;
                    for(tuint32 i = 0 ;i<nNum;i++)
                    {
                        memset(&srcST,0,sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO));
                        memset(&dstST,0,sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO_Lite));

                        memcpy(&srcST,pBuf + pCfgOffset[itemcount]+i*sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO),sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO));

                        dstST.nodeID = srcST.nodeID;
                        dstST.nodeType = srcST.nodeType;
                        dstST.UpdateMask = srcST.UpdateMask;

                        memcpy(DataInfo.pData + sizeof(NCFG_ITEM_HEAD) +nAlreadyAdd*sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO_Lite) ,\
                                   &dstST, sizeof(CFGPACK_DEFINE::RES_DEVICE_INFO_Lite));
                        nAlreadyAdd++;
                    }

                    pItemList->push_back(DataInfo);

                    m_ConfigDataList.push_back(DataInfo);

                    continue;//下一个for循环
                }
            }
            else if( (itemID == CFGPACK_DEFINE::DATA_ITEM_CH_INFO) )
            {
                tuint32 nNum = pItemHead[itemcount].num;

                if(nNum*sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO) == pItemHead[itemcount].len)
                {
                    DATA_BLOCK_INFO   DataInfo;
                    memset(&DataInfo,0,sizeof(DataInfo));
                    DataInfo.dataLen = sizeof(NCFG_ITEM_HEAD) + nNum*sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite);
                    DataInfo.pData = new char[DataInfo.dataLen];
                    NCFG_ITEM_HEAD headST;
                    memcpy(&headST,&pItemHead[itemcount],sizeof(NCFG_ITEM_HEAD));
                    headST.len = nNum*sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite);
                    headST.subLen = sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite);
                    memcpy(DataInfo.pData, &headST, sizeof(NCFG_ITEM_HEAD));

                    tuint32 nAlreadyAdd = 0;//已经添加了的数量
                    CFGPACK_DEFINE::RES_CHANNEL_INFO srcST;
                    CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite dstST;
                    for(tuint32 i = 0 ;i<nNum;i++)
                    {
                        memset(&srcST,0,sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO));
                        memset(&dstST,0,sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite));

                        memcpy(&srcST,pBuf + pCfgOffset[itemcount]+i*sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO),sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO));

                        dstST.nodeID = srcST.nodeID;
                        dstST.deviceNodeID = srcST.deviceNodeID;
                        dstST.Index = srcST.Index;
                        dstST.KeyboardIndex = srcST.KeyboardIndex;
                        dstST.byStreamCount = srcST.byStreamCount;
                        memcpy(dstST.byReserve,srcST.byReserve,sizeof(dstST.byReserve));
                        dstST.UpdateMask = srcST.UpdateMask;

                        memcpy(DataInfo.pData + sizeof(NCFG_ITEM_HEAD) +nAlreadyAdd*sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite) ,\
                                   &dstST, sizeof(CFGPACK_DEFINE::RES_CHANNEL_INFO_Lite));
                        nAlreadyAdd++;
                    }

                    pItemList->push_back(DataInfo);

                    m_ConfigDataList.push_back(DataInfo);

                    continue;//下一个for循环
                }
            }

            //常规
            DATA_BLOCK_INFO   DataInfo;
            memset(&DataInfo,0,sizeof(DataInfo));
            DataInfo.dataLen = sizeof(NCFG_ITEM_HEAD) + pItemHead[itemcount].len;
            DataInfo.pData = new char[DataInfo.dataLen];
            memcpy(DataInfo.pData, &pItemHead[itemcount], sizeof(NCFG_ITEM_HEAD));
            memcpy(DataInfo.pData + sizeof(NCFG_ITEM_HEAD), pBuf + pCfgOffset[itemcount], pItemHead[itemcount].len);

            pItemList->push_back(DataInfo);

            m_ConfigDataList.push_back(DataInfo);

        }
    }


    delete [] pCfgOffset;

    pCfgOffset = NULL;

    return 0;
}

int CConfigPack::SetConfigBlockByItem(const NCFG_ITEM_HEAD &itemHead, const char *pData, int len)
{
	return SetConfigBlock(1, &itemHead, pData);
}

int CConfigPack::SetConfigBlockByItem(const NCFG_ITEM_HEAD &itemHead, const char *pData)
{
	return SetConfigBlock(1, &itemHead, pData);
}

int CConfigPack::SetConfigBlockByItem(CConfigPack &configPack, const int nItemID, const int dwItemSize, char *pBuff)
{
	if (pBuff != NULL)
	{
		// 写入配置文件
		CFGPACK_DEFINE::NCFG_ITEM_HEAD ItemHead = {0};
		ItemHead.itemID = nItemID;					//项ID
		ItemHead.num = 1;							//元素数目
		ItemHead.subLen = dwItemSize;				//每个元素的长度
		ItemHead.operatorType = CFGPACK_DEFINE::CREATE_ITEM;		//操作类型,OPERATOR_TYPE操作类型包括添加、删除、修改
		ItemHead.len = ItemHead.subLen;				//数据总长度,因为只有一个,所以与单项长度相等
		ItemHead.destNodeID = GUID_NULL;			//发送的目标节点
		configPack.SetConfigBlockByItem(ItemHead, pBuff);
	}

	return 1;
}

int CConfigPack::GetConfigBlockByID(const NCFG_ITEM_HEAD * pHead, int headNum, char **ppDataOut, int *pLen)
{
	////认证服务器的配置数据打包
	ASSERT(FALSE);
	return -1;
}

void CConfigPack::ReleaseConfigData(char *&pData) const
{
	if (pData != NULL)
	{
		delete [] pData;
		pData = NULL;
	}
}

///////////私有成员  Copyitem内容
void CConfigPack::ItemListCopy(int itemID, std::list<DATA_BLOCK_INFO> &destList) const
{
	std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::const_iterator iter=m_ConfigDataMap.find(itemID);
	if(iter !=  m_ConfigDataMap.end())
	{
		ASSERT(iter->second != NULL);
		ASSERT(!(iter->second->empty()));
		destList.insert(destList.end(),iter->second->begin(),iter->second->end());
	}
}

int CConfigPack::GetBlockBuf(char *&pDataBuf) const
{
	int BufLen = 0;
	int ItemCount = 0;
	
	GetBlockInfo(BufLen, ItemCount);

	if (!BufLen)
	{
		ASSERT(!ItemCount);
		return 0;
	}
	
	ASSERT(BufLen > 0);
	ASSERT(ItemCount > 0);
	
	BufLen += sizeof(NCFG_BLOCK_HEAD);
	pDataBuf = new char[BufLen];
	ASSERT(NULL != pDataBuf);

	NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pDataBuf);
	pBlockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
	pBlockHead->netcfgver = CFGPACK_DEFINE::NCFG_VERSION;
	pBlockHead->ItemNum = ItemCount;

	NCFG_ITEM_HEAD  *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pDataBuf + sizeof(NCFG_BLOCK_HEAD));
	int  ItemIndex = 0;
	char  *pConfigBuf = (pDataBuf + sizeof(NCFG_BLOCK_HEAD) + ItemCount*sizeof(NCFG_ITEM_HEAD));
	int   Offset = 0;

	std::list<DATA_BLOCK_INFO>::const_iterator iter = m_ConfigDataList.begin();
	for (; iter != m_ConfigDataList.end(); iter++, ItemIndex++)
	{
		memcpy(&pItemHead[ItemIndex], iter->pData, sizeof(NCFG_ITEM_HEAD));
		memcpy(pConfigBuf + Offset, iter->pData + sizeof(NCFG_ITEM_HEAD), pItemHead[ItemIndex].len);

		Offset += pItemHead[ItemIndex].len; 				
	}

	return BufLen;
}

int CConfigPack::GetBlockBufLen() const
{
	int BufLen = 0;
	int ItemCount = 0;

	GetBlockInfo(BufLen, ItemCount);
	return BufLen;
}

int CConfigPack::GetBlockInfo(int &bufLen, int &itemCount) const
{
	bufLen = 0;

	////////////获取块的数量  和  总的Buf的长度
	itemCount = static_cast<int>(m_ConfigDataList.size());

	std::list<DATA_BLOCK_INFO>::const_iterator iter = m_ConfigDataList.begin();
	for (; iter != m_ConfigDataList.end(); iter++)
	{
		bufLen += iter->dataLen;
	}

	return 0;
}

bool CConfigPack::ClearConfigInfo(const std::set<tuint16/*项的ID*/> &itemIDSet)
{
	std::set<tuint16>::const_iterator IDIter= itemIDSet.begin();
	for (; IDIter != itemIDSet.end(); IDIter++)
	{
		std::list<DATA_BLOCK_INFO>::iterator iter = m_ConfigDataList.begin();
		while (iter != m_ConfigDataList.end())
		{
			ASSERT(iter->dataLen > (int)sizeof(NCFG_ITEM_HEAD));
			if(reinterpret_cast<NCFG_ITEM_HEAD *>(iter->pData)->itemID == *IDIter)
			{
				delete [] iter->pData;
				iter = m_ConfigDataList.erase(iter);
			}
			else
			{
				iter++;
			}
		}

		std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator mapiter=m_ConfigDataMap.find(*IDIter);
		if( mapiter !=  m_ConfigDataMap.end())
		{
			std::list<DATA_BLOCK_INFO> *   pMyList = mapiter->second;
			ASSERT(NULL != pMyList);
			ASSERT(!pMyList->empty());
			pMyList->clear();
			delete pMyList;
			pMyList = NULL;
			m_ConfigDataMap.erase(mapiter);
		}
	}

	return true;
}

bool CConfigPack::ClearHasDestInfo()
{
	std::list<DATA_BLOCK_INFO>::iterator iter = m_ConfigDataList.begin();
	while (iter != m_ConfigDataList.end())
	{
		ASSERT(iter->dataLen >= (int)sizeof(NCFG_ITEM_HEAD));
		if(reinterpret_cast<NCFG_ITEM_HEAD *>(iter->pData)->destNodeID != GUID_NULL/* NVMS_DEFINE::INVALID_NODE_ID*/)
		{
			{
				tuint32 ItemID=reinterpret_cast<NCFG_ITEM_HEAD *>(iter->pData)->itemID;
				std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator mapiter=m_ConfigDataMap.find(ItemID);
				if( mapiter != m_ConfigDataMap.end() )
				{
					std::list<DATA_BLOCK_INFO> *   pMyList = mapiter->second;
					ASSERT(NULL != pMyList);
					std::list<DATA_BLOCK_INFO>::iterator listiter=pMyList->begin();
					while(listiter !=  pMyList->end())
					{
						DATA_BLOCK_INFO blocktmp=*listiter;
						if(blocktmp.pData == iter->pData)
						{
							pMyList->erase(listiter);
							break;
						}
						listiter++;
					}
					if(pMyList->empty())
					{
						delete pMyList;
						pMyList = NULL;
						m_ConfigDataMap.erase(mapiter);
					}
				}
			}

			delete [] iter->pData;
			iter=m_ConfigDataList.erase(iter);

			continue;
		}
		else
		{
			iter++;
		}
	}
	return true;
}

int	 CConfigPack::GetConfigInfo(const std::set<tuint16/*项的ID*/> &itemIDSet,std::list<DATA_BLOCK_INFO> &ConfigInfoList)
{
	std::set<tuint16>::const_iterator iter = itemIDSet.begin();
	while (iter != itemIDSet.end())
	{
		tuint16   ItemID =*iter;
		iter++;
		ItemListCopy(ItemID, ConfigInfoList);
	}
	return 0;
}

bool CConfigPack::DeleteBlockInfo(CConfigPack::DATA_BLOCK_INFO &BlockInfo)
{
	const NCFG_ITEM_HEAD &ItemHead = *reinterpret_cast<const NCFG_ITEM_HEAD *>(BlockInfo.pData);

	std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator MapIter = m_ConfigDataMap.find(ItemHead.itemID);
	ASSERT(MapIter != m_ConfigDataMap.end());

	std::list<DATA_BLOCK_INFO>::iterator ListIter = MapIter->second->begin();
	for (; ListIter != MapIter->second->end(); ListIter++)
	{
		if (ListIter->pData == BlockInfo.pData)		//同一块内存
		{
			ASSERT(ListIter->dataLen == BlockInfo.dataLen);

			MapIter->second->erase(ListIter);
			if (MapIter->second->empty())
			{
				delete MapIter->second;
				m_ConfigDataMap.erase(MapIter);
			}

			break;
		}
	}

	std::list<CConfigPack::DATA_BLOCK_INFO>::iterator CfgDataIter = m_ConfigDataList.begin();
	for(; CfgDataIter != m_ConfigDataList.end(); CfgDataIter++)
	{
		if(CfgDataIter->pData == BlockInfo.pData)
		{
			ASSERT(CfgDataIter->dataLen == BlockInfo.dataLen);
			m_ConfigDataList.erase(CfgDataIter);
			break;
		}
	}

	delete []BlockInfo.pData;
	BlockInfo.pData = NULL;
	BlockInfo.dataLen = 0;

	return true;
}

int  CConfigPack::GetConfigDataList(std::list<CConfigPack::DATA_BLOCK_INFO> &configDataList) const
{
	ASSERT(configDataList.empty());
	configDataList=m_ConfigDataList;
	return static_cast<int>(configDataList.size());
}

void CConfigPack::AddBlockInfo(int ItemID, const DATA_BLOCK_INFO &BlockInfo)
{
	std::list<DATA_BLOCK_INFO> *pItemList = NULL;
	std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator Iter = m_ConfigDataMap.find(ItemID);
	if (Iter == m_ConfigDataMap.end())			//如果节点的相关内容不存在
	{
		pItemList = new std::list<DATA_BLOCK_INFO>;
		m_ConfigDataMap.insert(std::pair<tuint32, std::list<DATA_BLOCK_INFO> *>(ItemID, pItemList));
	}
	else
	{
		pItemList = Iter->second;
	}

	ASSERT(NULL != pItemList);
	pItemList->push_back(BlockInfo);

	m_ConfigDataList.push_back(BlockInfo);
}

void CConfigPack::AddBlockInfo(CFGPACK_DEFINE::DATA_ITEM_ID ItemID, const DATA_BLOCK_INFO &BlockInfo)
{
	std::list<DATA_BLOCK_INFO> *pItemList = NULL;
	std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::iterator Iter = m_ConfigDataMap.find(ItemID);
	if (Iter == m_ConfigDataMap.end())			//如果节点的相关内容不存在
	{
		pItemList = new std::list<DATA_BLOCK_INFO>;
		m_ConfigDataMap.insert(std::pair<tuint32, std::list<DATA_BLOCK_INFO> *>(ItemID, pItemList));
	}
	else
	{
		pItemList = Iter->second;
	}

	ASSERT(NULL != pItemList);
	pItemList->push_back(BlockInfo);

	m_ConfigDataList.push_back(BlockInfo);
}

void CConfigPack::GetItemBlockInfoList(tuint32 ItemID, std::list<DATA_BLOCK_INFO> &BlockInfoList) const
{
	std::map<tuint32, std::list<DATA_BLOCK_INFO> *>::const_iterator Iter = m_ConfigDataMap.find(ItemID);
	if (Iter !=  m_ConfigDataMap.end())
	{
		ASSERT(Iter->second != NULL);
		ASSERT(!(Iter->second->empty()));
		BlockInfoList.insert(BlockInfoList.end(), Iter->second->begin(), Iter->second->end());
	}
}
