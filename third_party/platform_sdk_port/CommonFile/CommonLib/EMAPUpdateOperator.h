#ifndef __EMAP_UPDATE_OPERATOR__
#define __EMAP_UPDATE_OPERATOR__

class CConfigPack;
class CEMapNode;

class CEMapUpdateOperator
{
public:
	CEMapUpdateOperator(void) {}
	virtual ~CEMapUpdateOperator(void) {}

	virtual void AddItem(const CEMapNode *pEMapNode, CConfigPack &ConfigPack) {}		//添加节点
	virtual void DeleteItem(const CEMapNode *pEMapNode,  CConfigPack &ConfigPack) {}	//删除节点
	virtual void ModifyItem(const CEMapNode *pOldEMapNode, const CEMapNode *pNewEMapNode, CConfigPack &ConfigPack) {}	//修改节点

	//电子地图树关系, 没有修改
	virtual void AddEMapTree(const CEMapNode *pParentNode, const CEMapNode *pChildNode, CConfigPack &ConfigPack) {}
	virtual void DeleteEMapTree(const CEMapNode *pParentNode, const CEMapNode *pChildNode, CConfigPack &ConfigPack) {}

	//电子地图热区热点变化通知, 没有修改
	virtual void EMapAddHotSpotOrAreaNode(const CEMapNode *pEMapNode, const CEMapNode *pHotSpotOrAreaNode, CConfigPack &ConfigPack) {}
	virtual void EMapDelHotSpotOrAreaNode(const CEMapNode *pEMapNode, const CEMapNode *pHotSpotOrAreaNode, CConfigPack &ConfigPack) {}

	virtual void AddHotSpotEvent(const CEMapNode *pHotSpotNode, CConfigPack &ConfigPack) {}
	virtual void DeleteHotSpotEvent(const CEMapNode *pHotSpotNode, CConfigPack &ConfigPack) {}

	virtual void ClearAllInfo(void) {}

	virtual void UpdateCompleted(void) {}

	virtual void OnAddObserver(void) = 0;
};

#endif //__EMAP_UPDATE_OPERATOR__