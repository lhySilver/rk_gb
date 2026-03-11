#include "TVTXMLParse.h"
#include <vector>
#include <sstream>
#include "SysHelper.h"
#ifdef __ENVIRONMENT_MAC__
#include <unistd.h>
#endif

CTVTXMLParse::CTVTXMLParse(void)
{
    m_XMLDocument.Clear();
	m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
}

CTVTXMLParse::CTVTXMLParse(const std::string &strXML)
{
	m_XMLDocument.Clear();
	m_XMLDocument.Parse(strXML.c_str(), strXML.length());

	XMLNode *pFirstNode = m_XMLDocument.FirstChild();
	if (!pFirstNode || !pFirstNode->ToDeclaration())
	{
		m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
	}
}

CTVTXMLParse::CTVTXMLParse(const char *pXML, tuint32 nLen)
{
	if (pXML == 0 && nLen <= 0)
	{
		ASSERT(0);
		return;
	}
	m_XMLDocument.Clear();
	m_XMLDocument.Parse(pXML, nLen);

	XMLNode *pFirstNode = m_XMLDocument.FirstChild();
	if (!pFirstNode || !pFirstNode->ToDeclaration())
	{
		m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
	}
}


CTVTXMLParse::~CTVTXMLParse(void)
{
}

void CTVTXMLParse::SaveXMLSrc(const std::string &strXMLSrc)
{
	m_strXml = strXMLSrc;
}

std::string CTVTXMLParse::GetXMLSrc()
{
	return m_strXml;
}

bool CTVTXMLParse::LoadXML(const std::string &strXML)
{
	ASSERT(strXML.length() != 0);
	m_XMLDocument.Clear();
	return (m_XMLDocument.Parse(strXML.c_str(), strXML.length()) == 0);
}

bool CTVTXMLParse::LoadFile(const std::string &strFile)
{
	ASSERT(strFile.length() != 0);
	m_strFile = strFile;
	bool bRet = (m_XMLDocument.LoadFile(strFile.c_str()) == XML_SUCCESS);
	if (bRet)
	{
		XMLNode *pFirstNode = m_XMLDocument.FirstChild();
		if (!pFirstNode || !pFirstNode->ToDeclaration())		// 如果加载的文件为空, 则加入xml头部声明信息
		{
			m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
		}
	}
// 	else
// 	{
// 		m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
// 	}

	return bRet;
}

bool CTVTXMLParse::SaveFile(const std::string &strFile/*=""*/)
{
	std::string strFilePath = m_strFile;
	if (strFile != "")
	{
		strFilePath = strFile;
	}

    ASSERT(strFilePath.length() > 0);
    XMLError Error = XML_NO_ERROR;
#ifdef __ENVIRONMENT_MAC__
    FILE* fp = fopen(strFilePath.c_str(), "w+");
    if (fp)
    {
        Error = m_XMLDocument.SaveFile(fp);
        int fd = ::fileno(fp); // 获取文件描述符
        ::fsync(fd); // 强制写磁盘
        fclose(fp);
    }
#else
    Error = m_XMLDocument.SaveFile(strFilePath.c_str());
#endif
	return (Error == XML_SUCCESS);
}

tuint_ptr CTVTXMLParse::GetRootItem(void) const
{
	return (tuint_ptr)m_XMLDocument.RootElement();
}

XMLElement *CTVTXMLParse::NewElement(const std::string &strName, const std::string &strData /* ="" */
	, XMLNode *pParentNode/*=0*/, bool bCData/* = false*/)
{
	XMLElement *pElement = 0, *pParentElement = 0;
	std::vector<std::string> strResultVec;
	split(strName, "/", strResultVec);

	// 创建节点
	if (strResultVec.size()>0)
	{
		pParentElement = m_XMLDocument.NewElement(strResultVec[0].c_str());
		pElement = pParentElement;

		for (tuint32 nIndex=1; nIndex<strResultVec.size(); nIndex++)
		{
			XMLElement *pNewElement = m_XMLDocument.NewElement(strResultVec[nIndex].c_str());
			if (pElement != 0)
			{
				pElement->InsertEndChild(pNewElement);
				pElement = pNewElement;
			}
		}

		if (pElement != 0)
		{
			XMLText * pText = m_XMLDocument.NewText(strData.c_str());
			if (bCData)
			{
				pText->SetCData(bCData);
			}

			pElement->InsertFirstChild(pText);
		}
	}

	if (pParentNode != 0)
	{
		pParentNode->InsertEndChild(pParentElement);
	}
	return pElement;
}

// 添加元素*****************************************************************************************
tuint_ptr  CTVTXMLParse::AddElement(const std::string &strName, const std::string &strData/* ="" */, bool bCData/* = false*/)
{
	XMLElement *pElement = 0;
	// 如果文档根节点为空，则创建，否则作为根节点的最后一个子节点
	XMLElement* pRoot = m_XMLDocument.RootElement();
	if (pRoot == 0)
	{
		pElement = NewElement(strName, strData, &m_XMLDocument, bCData);
	}
	else
	{
		pElement = NewElement(strName, strData, pRoot, bCData);
	}

	return (tuint_ptr)pElement;
}

tuint_ptr CTVTXMLParse::AddElement(const tuint_ptr ptrItem, const std::string &strName
	, const std::string &strData/*=""*/, bool bCData/* = false*/)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		XMLElement *pElement = NewElement(strName, strData, pXMLElement, bCData);
		return (tuint_ptr)pElement;
	}

	return 0;
}

tuint_ptr CTVTXMLParse::AddValue(const std::string &strName, const std::string &strData/*=""*/, bool bCData/* = false*/)
{
	tuint_ptr ptrNode = AddElement(strName, strData, bCData);
	return ptrNode;
}

tuint_ptr CTVTXMLParse::AddValue(const std::string &strName, const tuint32 dwData)
{
	stringstream strData;
	strData<<dwData;

	tuint_ptr ptrNode = AddElement(strName, strData.str());
	return ptrNode;
}

 tuint_ptr CTVTXMLParse::AddValue_i32(const std::string &strName, const tint32 dwData)
{
	stringstream strData;
	strData<<dwData;

	tuint_ptr ptrNode = AddElement(strName, strData.str());
	return ptrNode;
}


tuint_ptr CTVTXMLParse::AddValue_ui64(const std::string &strName, const tuint64 dwData)
{
	stringstream strData;
	strData<<dwData;

	tuint_ptr ptrNode = AddElement(strName, strData.str());
	return ptrNode;
}

 tuint_ptr CTVTXMLParse::AddValue(const std::string &strName, const GUID &GUIDData)
{
	char chGUID[48] = {0};
	CSysHelper::GuidToString(GUIDData, chGUID);
	tuint_ptr ptrNode = AddElement(strName, chGUID);
	return ptrNode;
}

tuint_ptr CTVTXMLParse::AddValue(const tuint_ptr ptrItem, const std::string &strName
	, const std::string &strData/*=""*/, bool bCData/* = false*/)
{
	tuint_ptr ptrNode = AddElement(ptrItem, strName, strData, bCData);
	return ptrNode;
}

tuint_ptr CTVTXMLParse::AddValue(const tuint_ptr ptrItem, const std::string &strName, const tuint32 dwData)
{
	stringstream strData;
	strData<<dwData;
	tuint_ptr ptrNode = AddElement(ptrItem, strName, strData.str());
	return ptrNode;
}

tuint_ptr CTVTXMLParse::AddValue_i32(const tuint_ptr ptrItem, const std::string &strName, const tint32 dwData)
{
	stringstream strData;
	strData<<dwData;
	tuint_ptr ptrNode = AddElement(ptrItem, strName, strData.str());
	return ptrNode;
}

tuint_ptr CTVTXMLParse::AddValue_ui64(const tuint_ptr ptrItem, const std::string &strName, const tuint64 dwData)
{
	stringstream strData;
	strData<<dwData;
	tuint_ptr ptrNode = AddElement(ptrItem, strName, strData.str());
	return ptrNode;
}

tuint_ptr CTVTXMLParse::AddValue(const tuint_ptr ptrItem, const std::string &strName, const GUID &GUIDData)
{
	char chGUID[48] = {0};
	CSysHelper::GuidToString(GUIDData, chGUID);
	tuint_ptr ptrNode = AddElement(ptrItem, strName, chGUID);
	return ptrNode;
}

// 设置元素数据*******************************************************************************
bool CTVTXMLParse::SetValue(const std::string &strName, const std::string &strData, bool bCData/* = false*/)
{
	XMLElement *pXMLElement = FindElement(strName);
	return SetValue((tuint_ptr)pXMLElement, strData, bCData);
}

bool CTVTXMLParse::SetValue(const std::string &strName, const tuint32 dwData)
{
	XMLElement *pXMLElement = FindElement(strName);
	return SetValue((tuint_ptr)pXMLElement, dwData);
}

bool CTVTXMLParse::SetValue_i32(const std::string &strName, const tint32 dwData)
{
	XMLElement *pXMLElement = FindElement(strName);
	return SetValue((tuint_ptr)pXMLElement, dwData);
}

bool CTVTXMLParse::SetValue_ui64(const std::string &strName, const tuint64 dwData)
{
	XMLElement *pXMLElement = FindElement(strName);
	return SetValue_ui64((tuint_ptr)pXMLElement, dwData);
}

bool CTVTXMLParse::SetValue(const std::string &strName, const GUID &GUIDData)
{
	XMLElement *pXMLElement = FindElement(strName);
	return SetValue((tuint_ptr)pXMLElement, GUIDData);
}

bool CTVTXMLParse::SetValue(const tuint_ptr ptrItem, const std::string &strData, bool bCData /*= false*/)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
   // ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		return SetText(pXMLElement, strData.c_str(), bCData);
	}

	return false;
}

bool CTVTXMLParse::SetValue(const tuint_ptr ptrItem, const std::string &strName, const std::string &strData, bool bCData /*= false*/)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	if (ptrPos == 0)
	{
		return false;
	}

	return SetValue(ptrPos, strData, bCData);
}

bool CTVTXMLParse::SetValue(const tuint_ptr ptrItem, const tuint32 dwData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		stringstream strData;
		strData<<dwData;
		return SetText(pXMLElement, strData.str().c_str(), false);
	}

	return false;
}

bool CTVTXMLParse::SetValue(const tuint_ptr ptrItem, const std::string &strName, const tuint32 dwData)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	if (ptrPos == 0)
	{
		return false;
	}

	return SetValue(ptrPos, dwData);
}

bool CTVTXMLParse::SetValue_i32(const tuint_ptr ptrItem, const tint32 dwData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		stringstream strData;
		strData<<dwData;
		return SetText(pXMLElement, strData.str().c_str(), false);
	}

	return false;
}

bool CTVTXMLParse::SetValue_i32(const tuint_ptr ptrItem, const std::string &strName, const tint32 dwData)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	if (ptrPos == 0)
	{
		return false;
	}

	return SetValue_i32(ptrPos, dwData);
}


bool CTVTXMLParse::SetValue_ui64(const tuint_ptr ptrItem, const tuint64 dwData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		stringstream strData;
		strData<<dwData;
		return SetText(pXMLElement, strData.str().c_str(), false);
	}

	return false;
}

bool CTVTXMLParse::SetValue_ui64(const tuint_ptr ptrItem, const std::string &strName, const tuint64 dwData)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	if (ptrPos == 0)
	{
		return false;
	}

	return SetValue_ui64(ptrPos, dwData);
}

bool CTVTXMLParse::SetValue(const tuint_ptr ptrItem, const GUID &GUIDData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		char chGUID[48] = {0};
		CSysHelper::GuidToString(GUIDData, chGUID);
		return SetText(pXMLElement, chGUID, false);
	}

	return false;
}

bool CTVTXMLParse::SetValue(const tuint_ptr ptrItem, const std::string &strName, const GUID &GUIDData)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	if (ptrPos == 0)
	{
		return false;
	}

	return SetValue(ptrPos, GUIDData);
}

bool CTVTXMLParse::SetText(XMLNode *pNode, const char *str, bool bCData)
{
	if (!(pNode != 0 && str != 0))
	{
		return false;
	}

	XMLNode *textNode = pNode->FirstChild();

	if (textNode)
	{
		XMLText *pText = textNode->ToText();
		pText->SetCData(bCData);
		textNode->ToText()->SetValue( str );
	}
	else
	{
		XMLText * pText = pNode->GetDocument()->NewText( str );
		pText->SetCData(bCData);
		pNode->InsertFirstChild(pText);	
	}

	return true;
}

// 删除节点*******************************************************************************************
void CTVTXMLParse::DeleteValue(const std::string &strNodeName)
{
	XMLElement *pXMLElement = FindElement(strNodeName);
	DeleteValue((tuint_ptr)pXMLElement);
}

void CTVTXMLParse::DeleteValue(const tuint_ptr ptrItem)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;

	ASSERT((XMLNode*)pXMLElement != (XMLNode*)&m_XMLDocument);
	if (pXMLElement != 0)
	{
		m_XMLDocument.DeleteNode(pXMLElement);
	}
}

void CTVTXMLParse::DeleteChildrenValue(const std::string &strNodeName)
{
	XMLElement *pXMLElement = FindElement(strNodeName);
	DeleteChildrenValue((tuint_ptr)pXMLElement);
}

void CTVTXMLParse::DeleteChildrenValue(const tuint_ptr ptrItem)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	if (pXMLElement != 0)
	{
		pXMLElement->DeleteChildren();
	}
}

// 设置属性********************************************************************************************
bool CTVTXMLParse::SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const std::string &strData/*=""*/)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), strData.c_str());
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const tuint32 dwData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), dwData);
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute_i32(const tuint_ptr ptrItem, const std::string &strAttrName, const tint32 dwData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), dwData);
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute_ui64(const tuint_ptr ptrItem, const std::string &strAttrName, const tuint64 dwData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		stringstream strData;
		strData<<dwData;
		pXMLElement->SetAttribute(strAttrName.c_str(), strData.str().c_str());
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const GUID &GUIDData)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		char chGUID[48] = {0};
		CSysHelper::GuidToString(GUIDData, chGUID);
		pXMLElement->SetAttribute(strAttrName.c_str(), chGUID);
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute(const std::string &strName, const std::string &strAttrName
	, const std::string &strData/*=""*/)
{
	XMLElement *pXMLElement = FindElement(strName);
	ASSERT(pXMLElement!= 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), strData.c_str());
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute(const std::string &strName, const std::string &strAttrName, const tuint32 dwData)
{
	XMLElement *pXMLElement = FindElement(strName);
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), dwData);
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute_i32(const std::string &strName, const std::string &strAttrName, const tint32 dwData)
{
	XMLElement *pXMLElement = FindElement(strName);
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), dwData);
		return true;
	}

	return false;
}

bool CTVTXMLParse::SetAttribute_ui64(const std::string &strName, const std::string &strAttrName, const tuint64 dwData)
{
	XMLElement *pXMLElement = FindElement(strName);
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		stringstream strData;
		strData<<dwData;
		pXMLElement->SetAttribute(strAttrName.c_str(), strData.str().c_str());
		return true;
	}

	return false;
}


bool CTVTXMLParse::SetAttribute(const std::string &strName, const std::string &strAttrName, const GUID &GUIDData)
{
	XMLElement *pXMLElement = FindElement(strName);
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		char chGUID[48] = {0};
		CSysHelper::GuidToString(GUIDData, chGUID);
		pXMLElement->SetAttribute(strAttrName.c_str(), chGUID);
		return true;
	}

	return false;
}

// 删除属性****************************************************************************************
void CTVTXMLParse::DeleteAttribute(const std::string &strNodeName, const std::string &strAttrName)
{
	XMLElement *pXMLElement = FindElement(strNodeName);
	DeleteAttribute((tuint_ptr)pXMLElement, strAttrName);
}

void CTVTXMLParse::DeleteAttribute(const tuint_ptr ptrItem, const std::string &strAttrName)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->DeleteAttribute(strAttrName.c_str());
	}
}

// 获取元素数据******************************************************************************************
bool CTVTXMLParse::GetValueStr(const std::string &strName, std::string &strValue)				
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetValueStr((tuint_ptr)pXMLElement, strValue);

}

bool CTVTXMLParse::GetValueDWORD(const std::string &strName, tuint32 &dwValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetValueDWORD((tuint_ptr)pXMLElement, dwValue);

}

bool CTVTXMLParse::GetValue_i32(const std::string &strName, tint32 &dwValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetValue_i32((tuint_ptr)pXMLElement, dwValue);

}

bool CTVTXMLParse::GetValue_ui64(const std::string &strName, tuint64 &dwValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetValue_ui64((tuint_ptr)pXMLElement, dwValue);

}

bool CTVTXMLParse::GetValueGUID(const std::string &strName, GUID &GUIDValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetValueGUID((tuint_ptr)pXMLElement, GUIDValue);
}

bool CTVTXMLParse::GetValueStr(const tuint_ptr ptrItem, std::string &strValue)
{
	strValue = "";
	if (ptrItem != 0)
	{ 	
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pText = pXMLElement->GetText();
		if(pText != 0)
		{
			strValue = pText;
		}

		return true;
	}

	return false;
}



bool CTVTXMLParse::GetValueStr(const tuint_ptr ptrItem, const std::string &strName, std::string &strValue)
{
	strValue = "";
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	if (ptrPos == 0)
	{
		return false;
	}

	return GetValueStr(ptrPos, strValue);
}

bool CTVTXMLParse::GetValueDWORD(const tuint_ptr ptrItem, tuint32 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pText = pXMLElement->GetText();
		if(pText != 0)
		{
			stringstream strData(pText);
			strData>>dwValue;
		}

		return true;
	}

	return false;
}

bool CTVTXMLParse::GetValueDWORD(const tuint_ptr ptrItem, const std::string &strName, tuint32 &dwValue)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	return GetValueDWORD(ptrPos, dwValue);
}

bool CTVTXMLParse::GetValue_i32(const tuint_ptr ptrItem, tint32 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		pXMLElement->QueryIntText(&dwValue);
		return true;
	}

	return false;
}

bool CTVTXMLParse::GetValue_i32(const tuint_ptr ptrItem, const std::string &strName, tint32 &dwValue)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	return GetValue_i32(ptrPos, dwValue);
}

bool CTVTXMLParse::GetValue_ui64(const tuint_ptr ptrItem, tuint64 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pText = pXMLElement->GetText();
		if(pText != 0)
		{
			stringstream strData(pText);
			strData>>dwValue;
		}

		return true;
	}

	return false;
}

bool CTVTXMLParse::GetValue_ui64(const tuint_ptr ptrItem, const std::string &strName, tuint64 &dwValue)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	return GetValue_ui64(ptrPos, dwValue);
}


bool CTVTXMLParse::GetValueGUID(const tuint_ptr ptrItem, GUID &GUIDValue)
{
	 GUIDValue = GUID_NULL;
	if (ptrItem != 0)
	{  
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);

		const char *pText = pXMLElement->GetText();
		if(pText != 0 && strlen(pText) > 0)
		{
			GUIDValue = CSysHelper::GetGuidFromString(pText);
		}

		return true;
	}

	return false;
}

bool CTVTXMLParse::GetValueGUID(const tuint_ptr ptrItem, const std::string &strName, GUID &GUIDValue)
{
	tuint_ptr ptrPos = GetFirstItem(ptrItem, strName);
	return GetValueGUID(ptrPos, GUIDValue);
}

// 获取属性值**********************************************************************************************
bool CTVTXMLParse::GetAttributeStr(const std::string &strName, const std::string &strAttrName, std::string &strValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetAttributeStr((tuint_ptr)pXMLElement, strAttrName, strValue);
}

bool CTVTXMLParse::GetAttributeDWORD(const std::string &strName, const std::string &strAttrName, tuint32 &dwValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetAttributeDWORD((tuint_ptr)pXMLElement, strAttrName, dwValue);
}

bool CTVTXMLParse::GetAttribute_i32(const std::string &strName, const std::string &strAttrName, tint32 &dwValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetAttribute_i32((tuint_ptr)pXMLElement, strAttrName, dwValue);
}

bool CTVTXMLParse::GetAttribute_ui64(const std::string &strName, const std::string &strAttrName, tuint64 &dwValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetAttribute_ui64((tuint_ptr)pXMLElement, strAttrName, dwValue);
}

bool CTVTXMLParse::GetAttributeGUID(const std::string &strName, const std::string &strAttrName, GUID &GUIDValue)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetAttributeGUID((tuint_ptr)pXMLElement, strAttrName, GUIDValue);
}

bool CTVTXMLParse::GetAttributeStr(const tuint_ptr ptrItem, const std::string &strAttrName, std::string &strValue)
{
	strValue = "";
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pValue = pXMLElement->Attribute(strAttrName.c_str());
		if (pValue != 0)
		{
			strValue = pValue;
			return true;
		}
	}

	return false;
} 

bool CTVTXMLParse::GetAttributeDWORD(const tuint_ptr ptrItem, const std::string &strAttrName, tuint32 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pValue = pXMLElement->Attribute(strAttrName.c_str());
		if (pValue != 0)
		{
			stringstream strData(pValue);
			strData>>dwValue;
			return true;
		}
	}

	return false;
}

bool CTVTXMLParse::GetAttribute_i32(const tuint_ptr ptrItem, const std::string &strAttrName, tint32 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pValue = pXMLElement->Attribute(strAttrName.c_str());
		if (pValue != 0)
		{
			dwValue = atoi(pValue);
			return true;
		}
	}

	return false;
}

bool CTVTXMLParse::GetAttribute_ui64(const tuint_ptr ptrItem, const std::string &strAttrName, tuint64 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		XMLElement *pXMLElement = (XMLElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		const char *pValue = pXMLElement->Attribute(strAttrName.c_str());
		if (pValue != 0)
		{
			stringstream strData(pValue);
			strData>>dwValue;
			return true;
		}
	}

	return false;
}

bool CTVTXMLParse::GetAttributeGUID(const tuint_ptr ptrItem, const std::string &strAttrName, GUID &GUIDValue)
{
	GUIDValue = GUID_NULL;
	if (ptrItem != 0)
	{ 	
		XMLElement *pXMLElement = (XMLElement*)ptrItem;	 
		ASSERT(pXMLElement != 0);
		const char *pValue = pXMLElement->Attribute(strAttrName.c_str());
		if (pValue == 0)
		{
			return false;
		}
		if (strlen(pValue) > 0)
		{
			GUIDValue = CSysHelper::GetGuidFromString(pValue);
		}
		return true;
	}

	return false;
}


bool CTVTXMLParse::GetAttributes(const std::string &strNodeName, map<std::string, std::string> &AttributeMap)
{
	XMLElement *pXMLElement = FindElement(strNodeName);
	return GetAttributes((tuint_ptr)pXMLElement, AttributeMap);
}

bool CTVTXMLParse::GetAttributes(tuint_ptr ptrItem, map<std::string, std::string> &AttributeMap)
{
	AttributeMap.clear();
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		const XMLAttribute *pAttr = pXMLElement->FirstAttribute();
		while(pAttr != 0)
		{
			AttributeMap.insert(pair<std::string, std::string>(pAttr->Name(), pAttr->Value()));
			pAttr = pAttr->Next();
		}

		return true;
	}

	return false;
}


// 遍历操作**********************************************************************************
tuint_ptr CTVTXMLParse::GetFirstItem(const std::string &strName)
{
	XMLElement *pXMLElement = FindElement(strName);
	return (tuint_ptr)pXMLElement;
}

bool CTVTXMLParse::GetItemName(const tuint_ptr ptrItem, std::string &strItemName)
{
	strItemName = "";
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		strItemName = pXMLElement->Name();
		return true;
	}

	return false;
}

tuint_ptr CTVTXMLParse::GetNextSiblingItem(const tuint_ptr ptrItem)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		XMLElement *pNextElement = pXMLElement->NextSiblingElement();
		return (tuint_ptr)pNextElement;
	}

	return 0;
}

tuint_ptr CTVTXMLParse::GetNextSiblingItemByName(const tuint_ptr ptrItem,const string &strName)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		XMLElement *pNextElement = pXMLElement->NextSiblingElement();
		while ( (pNextElement!=0) && (strName.compare( pNextElement->Name() )!= 0) )
		{
			pNextElement = pNextElement->NextSiblingElement();
		}
		return (tuint_ptr)pNextElement;
	}

	return 0;
}

tuint_ptr CTVTXMLParse::GetChildItem(const tuint_ptr ptrItem)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		XMLElement *pChildElement = pXMLElement->FirstChildElement();
		return (tuint_ptr)pChildElement;
	}

	return 0;
}

tuint_ptr CTVTXMLParse::GetFirstItem(tuint_ptr ptrItem, const std::string &strName)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	pXMLElement = FindElement(strName, pXMLElement);
	return (tuint_ptr)pXMLElement;
}


bool CTVTXMLParse::GetXMLData(const std::string &strName, std::string &strXML)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetXMLData((tuint_ptr)pXMLElement, strXML);
}

bool CTVTXMLParse::GetXMLData(const tuint_ptr ptrItem, std::string &strXML)
{
	strXML = "";
	XMLNode *pDestNode = (XMLNode*)ptrItem;

	if (pDestNode == 0)
	{
		return false;
	}

	XMLPrinter printer;
	tinyxml2::XMLDocument doc;
	XMLNode *dNode = pDestNode->ShallowClone( &doc );
	ASSERT(dNode!=0);

	XMLElement *pNewElement = (XMLElement*)doc.InsertEndChild(dNode);
	CloneNode(pNewElement, pDestNode);
	doc.Print( &printer );
	strXML = printer.CStr();

	return true;
}



bool CTVTXMLParse::GetChildrenXMLData(const std::string &strName, std::string &strXML)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetChildrenXMLData((tuint_ptr)pXMLElement, strXML);
}

bool CTVTXMLParse::GetChildrenXMLData(const tuint_ptr ptrItem, std::string &strXML)
{
	strXML = "";
	XMLNode *pDestNode = (XMLNode*)ptrItem;

	if (pDestNode == 0)
	{
		return false;
	}

	tinyxml2::XMLDocument doc;
	CloneNode(&doc, pDestNode);

	XMLPrinter printer;
	doc.Print( &printer );
	strXML = printer.CStr();

	return true;
}

tint32 CTVTXMLParse::GetChildrenLength(const std::string &strName, const std::string &strChildName/* ="" */)
{
	XMLElement *pXMLElement = FindElement(strName);
	return GetChildrenLength((tuint_ptr)pXMLElement, strChildName);
}

tint32 CTVTXMLParse::GetChildrenLength(const tuint_ptr ptrItem, const std::string &strChildName/* ="" */)
{
	XMLElement *pXMLElement = (XMLElement*)ptrItem;
	int nCount = 0;

	if (pXMLElement != 0)
	{
		XMLElement *pChild = pXMLElement->FirstChildElement();

		while(pChild)
		{
			if (!strChildName.empty())
			{
				if (std::string(pChild->Name()) == strChildName)
				{
					nCount++;
				}
			}
			else
			{
				nCount++;
			}
			
			pChild = pChild->NextSiblingElement();
		}
	}

	return nCount;
}

bool CTVTXMLParse::CloneValue(const std::string &strName, const std::string &strXML)
{
	XMLElement *pXMLElement = FindElement(strName);
	return CloneValue((tuint_ptr)pXMLElement, strXML);
}

bool CTVTXMLParse::CloneValue(const tuint_ptr ptrItem, const std::string &strXML)
{
	XMLNode *pDestNode = (XMLNode*)ptrItem;

	if (pDestNode == 0)
	{
		return false;
	}

	tinyxml2::XMLDocument doc;
	doc.Parse(strXML.c_str());

	if ( doc.Error()) // 解析粗错误
	{
		return false;
	}

	CloneNode(pDestNode, &doc);
	return true;
}

bool CTVTXMLParse::CloneNode(XMLNode *pDestNode, XMLNode *pSrcNode)
{
	if (!(pDestNode && pSrcNode))
	{
		return false;
	}

	//	XMLDocument * pSrcDoc = srcNode->GetDocument();
	tinyxml2::XMLDocument *pDstDoc = pDestNode->GetDocument();
	XMLNode * sNode, * dNode;

	for (sNode=pSrcNode->FirstChild(), dNode=pDestNode->FirstChild(); sNode != 0; 
		sNode=sNode->NextSibling(), dNode=dNode->NextSibling())
	{
		//if (!dNode)
		{
			dNode = sNode->ShallowClone( pDstDoc );
			pDestNode->InsertEndChild( dNode );
		}

		// 递归调用
		CloneNode(dNode, sNode);
	}

	return true;
}


// 查找元素*************************************************************
XMLElement *CTVTXMLParse::FindElement(const std::string &strElement, XMLElement *pXMLElement/*=0*/)
{
	if (pXMLElement == 0)
	{
		pXMLElement = m_XMLDocument.RootElement();
	}
	if (pXMLElement == 0)
	{
		return 0;
	}

	// 如果查找的节点是根节点
	if (pXMLElement->Name() == strElement)
	{
		return pXMLElement;
	}

	vector<string> strResultVec;
	split(strElement, "/", strResultVec);
	for(vector<string>::iterator strIter=strResultVec.begin(); strIter!=strResultVec.end(); ++strIter)
	{
		// 过滤路径中的根节点：pXMLElement->Name() != strIter.
		if (pXMLElement != 0 && pXMLElement->Name() != (*strIter))
		{
			pXMLElement = pXMLElement->FirstChildElement(strIter->c_str());
		}
	}

	return pXMLElement;
}

void CTVTXMLParse::split(const std::string str,const std::string strPattern, std::vector<std::string> &strResultVec)
{
	string substring;
	string::size_type start = 0, index = -1;

	do
	{
		index = str.find_first_of(strPattern,start);
		if (index != string::npos)
		{    
			substring = str.substr(start,index-start);
			strResultVec.push_back(substring);
			start = str.find_first_not_of(strPattern,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	substring = str.substr(start);
	strResultVec.push_back(substring);
}

const std::string &CTVTXMLParse::GetDoc(void)
{
	//////////////////////////////////////////////////////////////////////////
	XMLPrinter printer;
	m_XMLDocument.Print( &printer );
	m_strDoc = printer.CStr();
	return m_strDoc;
}
