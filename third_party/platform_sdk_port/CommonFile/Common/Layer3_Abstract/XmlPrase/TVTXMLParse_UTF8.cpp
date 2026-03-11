#include "TVTXMLParse_UTF8.h"
#include <vector>
#include <sstream>
#include "SysHelper.h"

CTVTXMLParseUTF8::CTVTXMLParseUTF8(void)
{
}

CTVTXMLParseUTF8::CTVTXMLParseUTF8(const char *pXML, tuint32 nLen)
{
	//这个是标准xml头，设备端发来的XML数据都带这个头
	std::string strFind = "<?xml"; //"<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	if (pXML == 0 && nLen <= 0)
	{
		ASSERT(0);
		return;
	}
	std::string strXML(pXML);

	size_t pos = strXML.find(strFind);
	if (pos != std::string::npos)
	{
//		const char* test2 = pXML + pos;
		m_XMLDocument.Parse(pXML + pos);
	}
	else
	{
		m_XMLDocument.Parse(pXML);
	}
}

CTVTXMLParseUTF8::~CTVTXMLParseUTF8(void)
{
}

// 遍历操作**********************************************************************************
tuint_ptr CTVTXMLParseUTF8::GetFirstItem(const std::string &strName)
{
	TiXmlElement *pXMLElement = FindElement(strName);
	return (tuint_ptr)pXMLElement;
}

// 查找元素*************************************************************
TiXmlElement *CTVTXMLParseUTF8::FindElement(const std::string &strElement, TiXmlElement *pXMLElement/*=0*/)
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
	if (pXMLElement->Value() == strElement)
	{
		return pXMLElement;
	}

	vector<string> strResultVec;
	split(strElement, "/", strResultVec);
	for(vector<string>::iterator strIter=strResultVec.begin(); strIter!=strResultVec.end(); ++strIter)
	{
		// 过滤路径中的根节点：pXMLElement->Name() != strIter.
		if (pXMLElement != 0 && pXMLElement->Value() != (*strIter))
		{
			pXMLElement = pXMLElement->FirstChildElement(strIter->c_str());
		}
	}

	return pXMLElement;
}

void CTVTXMLParseUTF8::split(const std::string str,const std::string strPattern, std::vector<std::string> &strResultVec)
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

bool CTVTXMLParseUTF8::GetAttributeStr(const tuint_ptr ptrItem, const std::string &strAttrName, std::string &strValue)
{
	strValue = "";
	if (ptrItem != 0)
	{ 
		TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
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

const tuint_ptr CTVTXMLParseUTF8::AddValue(const std::string &strName, const std::string &strData/*=""*/, bool bCData/* = false*/)
{
	tuint_ptr ptrNode = AddElement(strName, strData, bCData);
	return ptrNode;
}

const tuint_ptr CTVTXMLParseUTF8::AddValue(const tuint_ptr ptrItem, const std::string &strName
	, const std::string &strData/*=""*/, bool bCData/* = false*/)
{
	tuint_ptr ptrNode = AddElement(ptrItem, strName, strData, bCData);
	return ptrNode;
}

const tuint_ptr CTVTXMLParseUTF8::AddValue(const tuint_ptr ptrItem, const std::string &strName, const tuint32 dwData)
{
	stringstream strData;
	strData<<dwData;
	tuint_ptr ptrNode = AddElement(ptrItem, strName, strData.str());
	return ptrNode;
}

TiXmlElement *CTVTXMLParseUTF8::NewElement(const std::string &strName, const std::string &strData /* ="" */
	, TiXmlElement *pParentNode/*=0*/, bool bCData/* = false*/)
{
	TiXmlElement ParentElement = "";
	TiXmlElement *pXMLElement = NULL;
	std::vector<std::string> strResultVec;
	split(strName, "/", strResultVec);

	// 创建节点
	if (strResultVec.size()>0)
	{
		ParentElement.SetValue(strResultVec[0].c_str());

		for (tuint32 nIndex=1; nIndex<strResultVec.size(); nIndex++)
		{
			TiXmlElement itemElement = "";
			itemElement.Clear();
			itemElement.SetValue(strResultVec[nIndex].c_str());
			
			pXMLElement = ParentElement.FirstChildElement(strResultVec[nIndex-1].c_str());
			pXMLElement->InsertEndChild(itemElement);
			
		}

		if (pXMLElement != NULL)
		{
			pXMLElement = ParentElement.FirstChildElement(strResultVec[strResultVec.size()-1].c_str());
			TiXmlText textNode(strData.c_str());
			pXMLElement->InsertEndChild(textNode);
		}
	}

	if (pParentNode != 0)
	{
		pParentNode->InsertEndChild(ParentElement);
	}
	return pXMLElement;
}

// 添加元素*****************************************************************************************
tuint_ptr  CTVTXMLParseUTF8::AddElement(const std::string &strName, const std::string &strData/* ="" */, bool bCData/* = false*/)
{
	TiXmlElement *pElement = 0;
	// 如果文档根节点为空，则创建，否则作为根节点的最后一个子节点
	TiXmlElement* pRoot = m_XMLDocument.RootElement();
	if (pRoot == 0)
	{
		TiXmlElement ParentElement = "";
		ParentElement.SetValue(strName.c_str());
		m_XMLDocument.InsertEndChild(ParentElement); 
		pElement = m_XMLDocument.RootElement();
	}
	else
	{
		pElement = NewElement(strName, strData, pRoot, bCData);
	}

	return (tuint_ptr)pElement;
}

tuint_ptr CTVTXMLParseUTF8::AddElement(const tuint_ptr ptrItem, const std::string &strName
	, const std::string &strData/*=""*/, bool bCData/* = false*/)
{
	TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		TiXmlElement *pElement = NewElement(strName, strData, pXMLElement, bCData);
		return (tuint_ptr)pElement;
	}

	return 0;
}

// 获取元素数据******************************************************************************************
bool CTVTXMLParseUTF8::GetValueStr(const std::string &strName, std::string &strValue)				
{
	TiXmlElement *pXMLElement = FindElement(strName);
	return GetValueStr((tuint_ptr)pXMLElement, strValue);

}

bool CTVTXMLParseUTF8::GetValueDWORD(const std::string &strName, tuint32 &dwValue)
{
	TiXmlElement *pXMLElement = FindElement(strName);
	return GetValueDWORD((tuint_ptr)pXMLElement, dwValue);

}

bool CTVTXMLParseUTF8::GetValueStr(const tuint_ptr ptrItem, std::string &strValue)
{
	strValue = "";
	if (ptrItem != 0)
	{ 	
		TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		TiXmlNode *pValue = pXMLElement->FirstChild();
		if (pValue == NULL)
		{
			return true;
		}
		const char *pText = pValue->ToText()->Value();

		if(pText != 0)
		{
			strValue = pText;
		}
		return true;
	}

	return false;
}


bool CTVTXMLParseUTF8::GetValueDWORD(const tuint_ptr ptrItem, tuint32 &dwValue)
{
	dwValue = 0;
	if (ptrItem != 0)
	{ 
		TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
		ASSERT(pXMLElement != 0);
		TiXmlNode *pValue = pXMLElement->FirstChild();
		if (pValue == NULL)
		{
			return true;
		}
		const char *pText = pValue->ToText()->Value();
		if(pText != 0)
		{
			stringstream strData(pText);
			strData>>dwValue;
		}

		return true;
	}

	return false;
}


const std::string &CTVTXMLParseUTF8::GetDoc(void)
{
	//////////////////////////////////////////////////////////////////////////
	TiXmlPrinter printer;;
	m_XMLDocument.Accept(&printer);
	m_strDoc = printer.CStr();
	return m_strDoc;
}

bool CTVTXMLParseUTF8::LoadXML(const std::string &strXML)
{
	ASSERT(strXML.length() != 0);
	m_XMLDocument.Clear();
	m_XMLDocument.Parse(strXML.c_str());
	return true;
}

bool CTVTXMLParseUTF8::LoadFile(const std::string &strFile)
{
	ASSERT(strFile.length() != 0);
	m_strFile = strFile;
	bool bRet = (m_XMLDocument.LoadFile(strFile.c_str()) == 0);
	if (bRet)
	{
		TiXmlNode *pFirstNode = m_XMLDocument.FirstChild();
		if (!pFirstNode || !pFirstNode->ToDeclaration())		// 如果加载的文件为空, 则加入xml头部声明信息
		{
			m_XMLDocument.ToDeclaration();
	//		m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
		}
	}
	// 	else
	// 	{
	// 		m_XMLDocument.InsertFirstChild(m_XMLDocument.NewDeclaration());
	// 	}

	return bRet;
}

bool CTVTXMLParseUTF8::SaveFile(const std::string &strFile/*=""*/)
{
	std::string strFilePath = m_strFile;
	if (strFile != "")
	{
		strFilePath = strFile;
	}

	ASSERT(strFilePath.length() > 0);
	bool bRet = (m_XMLDocument.SaveFile(strFilePath.c_str()) == 0);
	return bRet;
}

tuint_ptr CTVTXMLParseUTF8::GetNextSiblingItem(const tuint_ptr ptrItem)
{
	TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		TiXmlElement *pNextElement = pXMLElement->NextSiblingElement();
		return (tuint_ptr)pNextElement;
	}

	return 0;
}

// 设置属性********************************************************************************************
bool CTVTXMLParseUTF8::SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const std::string &strData/*=""*/)
{
	TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->SetAttribute(strAttrName.c_str(), strData.c_str());
		return true;
	}

	return false;
}

tuint_ptr CTVTXMLParseUTF8::GetChildItem(const tuint_ptr ptrItem)
{
	TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		TiXmlElement *pChildElement = pXMLElement->FirstChildElement();
		return (tuint_ptr)pChildElement;
	}

	return 0;
}

// 删除属性****************************************************************************************
void CTVTXMLParseUTF8::DeleteAttribute(const std::string &strNodeName, const std::string &strAttrName)
{
	TiXmlElement *pXMLElement = FindElement(strNodeName);
	DeleteAttribute((tuint_ptr)pXMLElement, strAttrName);
}

void CTVTXMLParseUTF8::DeleteAttribute(const tuint_ptr ptrItem, const std::string &strAttrName)
{
	TiXmlElement *pXMLElement = (TiXmlElement*)ptrItem;
	ASSERT(pXMLElement != 0);
	if (pXMLElement != 0)
	{
		pXMLElement->RemoveAttribute(strAttrName.c_str());
	}
}

bool CTVTXMLParseUTF8::GetXMLData(const std::string &strName, std::string &strXML)
{
	TiXmlElement *pXMLElement = FindElement(strName);
	return GetXMLData((tuint_ptr)pXMLElement, strXML);
}

bool CTVTXMLParseUTF8::GetXMLData(const tuint_ptr ptrItem, std::string &strXML)
{
	strXML = "";
	TiXmlNode *pDestNode = (TiXmlNode*)ptrItem;

	if (pDestNode == 0)
	{
		return false;
	}

	TiXmlPrinter printer;
	TiXmlDocument doc;

	const TiXmlNode *dNode = pDestNode->Clone();
	ASSERT(dNode!=0);

	doc.InsertEndChild(*dNode);
	//CloneNode(pNewElement, pDestNode);

	doc.Accept(&printer);
	strXML = printer.CStr();

	return true;
}

bool CTVTXMLParseUTF8::CloneNode(TiXmlNode *pDestNode, TiXmlNode *pSrcNode)
{
	if (!(pDestNode && pSrcNode))
	{
		return false;
	}

	//	XMLDocument * pSrcDoc = srcNode->GetDocument();
	pDestNode->GetDocument();
	TiXmlNode * sNode, * dNode;

	for (sNode=pSrcNode->FirstChild(), dNode=pDestNode->FirstChild(); sNode != 0; 
		sNode=sNode->NextSibling(), dNode=dNode->NextSibling())
	{
		//if (!dNode)
		{
			dNode = sNode->Clone();
			pDestNode->InsertEndChild( *dNode );
		}

		// 递归调用
		CloneNode(dNode, sNode);
	}

	return true;
}

bool CTVTXMLParseUTF8::CloneValue(const std::string &strName, const std::string &strXML)
{
	TiXmlElement *pXMLElement = FindElement(strName);
	return CloneValue((tuint_ptr)pXMLElement, strXML);
}

bool CTVTXMLParseUTF8::CloneValue(const tuint_ptr ptrItem, const std::string &strXML)
{
	TiXmlNode *pDestNode = (TiXmlNode*)ptrItem;

	if (pDestNode == 0)
	{
		return false;
	}

	TiXmlDocument doc;
	doc.Parse(strXML.c_str(), 0, TIXML_ENCODING_UTF8);

	if ( doc.Error()) // 解析粗错误
	{
		return false;
	}

	CloneNode(pDestNode, &doc);
	return true;
}
