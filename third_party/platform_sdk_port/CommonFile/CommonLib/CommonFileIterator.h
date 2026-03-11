
#ifndef __FILE_ITERATOR__
#define __FILE_ITERATOR__

#ifdef WIN32
#pragma warning (disable:4251)
#endif 

#include "CommonFileSDKAPI.h"
#include "ConfigPack_Struct_Def.h"
#include "ConfigPack_Data_Item_ID_Def.h"
#include <list>
#include "ConfigPack.h"


class _EXPORT_FILE_CLASS CCommonFileNode
{
public:
	virtual bool GetNodeInfo(CConfigPack &ConfigPack, CFGPACK_DEFINE::OPERATOR_TYPE OperatorType) const = 0;
	virtual tuint32 GetNodeID(void) const = 0;
	virtual const GUID &GetNodeGUID(void) const = 0;
	virtual bool GetNodeName(char *szName, int nSize) const = 0;

	virtual bool IsVoiceBroadcastFile(void) = 0;

	virtual bool SetFileInfo(const CFGPACK_DEFINE::RES_COMMON_FILE_INFO &FileInfo) = 0;
	virtual bool GetFileInfo(CFGPACK_DEFINE::RES_COMMON_FILE_INFO &FileInfo) = 0;

	virtual const char *GetFileData(tuint32 &nDataLen)  const = 0;

protected:
	CCommonFileNode(void) {}
	virtual ~CCommonFileNode(void) {}
};


class CCommonFileNodeEx;
class _EXPORT_FILE_CLASS CVoiceBroadcastItemIterator
{
public:
	CVoiceBroadcastItemIterator(void);
	explicit CVoiceBroadcastItemIterator(tuint32 dwFileID);
	explicit CVoiceBroadcastItemIterator(const GUID &FileGUID);
	explicit CVoiceBroadcastItemIterator(const char *szName);

	~CVoiceBroadcastItemIterator(void);

	bool HasNext(void);
	CCommonFileNode *GetNext(void);

private:
	std::list<CCommonFileNodeEx *> m_FileNodeList;
	std::list<CCommonFileNodeEx *>::iterator m_Iter;
};

#endif //__FILE_ITERATOR__