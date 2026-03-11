
#ifndef _TVTXMLPARSE_20131218_
#define _TVTXMLPARSE_20131218_

#include "tinyxml2.h"
#include "base_type_define.h"
#include <string>
#include <vector>
#include <map>
using namespace tinyxml2;
using namespace std;

class CTVTXMLParse
{
public:
	CTVTXMLParse(void);
	CTVTXMLParse(const std::string &strXML);
	CTVTXMLParse(const char *pXML, tuint32 nLen);
	~CTVTXMLParse(void);

	// 加载XML字符串
	bool LoadXML(const std::string &strXML);

	// 加载/保存xml文件
	bool LoadFile(const std::string &strFile);
	bool SaveFile(const std::string &strFile="");
	void SaveXMLSrc(const std::string &strXMLSrc);
	std::string GetXMLSrc();

	// 添加节点***********************************************************************************

	// (strName: 元素名称,支持路径名称,如：tvt/dvr; strData: 元素数据)
    tuint_ptr AddValue(const std::string &strName, const std::string &strData="", bool bCData = false);
    tuint_ptr AddValue(const std::string &strName, const tuint32 dwData);
    tuint_ptr AddValue_i32(const std::string &strName, const tint32 dwData);
    tuint_ptr AddValue_ui64(const std::string &strName, const tuint64 dwData);
    tuint_ptr AddValue(const std::string &strName, const GUID &GUIDData);

	// (ptrItem: 节点位置; strData: 元素数据)
    tuint_ptr AddValue(const tuint_ptr ptrItem, const std::string &strName, const std::string &strData="", bool bCData = false);
    tuint_ptr AddValue(const tuint_ptr ptrItem, const std::string &strName, const tuint32 dwData);
    tuint_ptr AddValue_i32(const tuint_ptr ptrItem, const std::string &strName, const tint32 dwData);
    tuint_ptr AddValue_ui64(const tuint_ptr ptrItem, const std::string &strName, const tuint64 dwData);
    tuint_ptr AddValue(const tuint_ptr ptrItem, const std::string &strName, const GUID &GUIDData);
	//////////////////////////////////////////////////////////////////////////
	
	// 设置节点数据***********************************************************************************
	// (strName: 元素名称,支持路径名称,如：tvt/dvr; strData: 元素数据)
	bool SetValue(const std::string &strName, const std::string &strData, bool bCData = false);
	bool SetValue(const std::string &strName, const tuint32 dwData);
	bool SetValue_i32(const std::string &strName, const tint32 dwData);
	bool SetValue_ui64(const std::string &strName, const tuint64 dwData);
	bool SetValue(const std::string &strName, const GUID &GUIDData);

	// (ptrItem: 节点位置; strName: 子节点名称 strData: 节点数据)
	bool SetValue(const tuint_ptr ptrItem, const std::string &strData, bool bCData = false);
	bool SetValue(const tuint_ptr ptrItem, const std::string &strName, const std::string &strData, bool bCData = false);

	bool SetValue(const tuint_ptr ptrItem, const tuint32 dwData);
	bool SetValue(const tuint_ptr ptrItem, const std::string &strName, const tuint32 dwData);

	bool SetValue_i32(const tuint_ptr ptrItem, const tint32 dwData);
	bool SetValue_i32(const tuint_ptr ptrItem, const std::string &strName, const tint32 dwData);

	bool SetValue_ui64(const tuint_ptr ptrItem, const tuint64 dwData);
	bool SetValue_ui64(const tuint_ptr ptrItem, const std::string &strName, const tuint64 dwData);

	bool SetValue(const tuint_ptr ptrItem, const GUID &GUIDData);
	bool SetValue(const tuint_ptr ptrItem, const std::string &strName, const GUID &GUIDData);
	//////////////////////////////////////////////////////////////////////////

	// 设置元素属性***************************************************************************************
	// (strName: 元素名称, strAttrName: 属性名称; strData: 属性值)
	bool SetAttribute(const std::string &strName, const std::string &strAttrName, const std::string &strData="");
	bool SetAttribute(const std::string &strName, const std::string &strAttrName, const tuint32 dwData);
	bool SetAttribute_i32(const std::string &strName, const std::string &strAttrName, const tint32 dwData);
	bool SetAttribute_ui64(const std::string &strName, const std::string &strAttrName, const tuint64 dwData);
	bool SetAttribute(const std::string &strName, const std::string &strAttrName, const GUID &GUIDData);

	// (ptrItem: 节点位置, strAttrName: 属性名称; strData: 属性值)
	bool SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const std::string &strData="");
	bool SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const tuint32 dwData);
	bool SetAttribute_i32(const tuint_ptr ptrItem, const std::string &strAttrName, const tint32 dwData);
	bool SetAttribute_ui64(const tuint_ptr ptrItem, const std::string &strAttrName, const tuint64 dwData);
	bool SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const GUID &GUIDData);
	//////////////////////////////////////////////////////////////////////////

	// 获取节点值(返回值用来判断获取的节点是否存在)************************************************************
    bool GetValueStr(const std::string &strName, std::string &strValue);
    bool GetValueDWORD(const std::string &strName, tuint32 &dwValue);
    bool GetValue_i32(const std::string &strName, tint32 &dwValue);
    bool GetValue_ui64(const std::string &strName, tuint64 &dwValue);
    bool GetValueGUID(const std::string &strName, GUID &GUIDValue);

    bool GetValueStr(const tuint_ptr ptrItem, std::string &strValue);
	bool GetValueStr(const tuint_ptr ptrItem, const std::string &strName, std::string &strValue);	

	bool GetValueDWORD(const tuint_ptr ptrItem, tuint32 &dwValue);	
	bool GetValueDWORD(const tuint_ptr ptrItem, const std::string &strName, tuint32 &dwValue);	

    bool GetValue_i32(const tuint_ptr ptrItem, tint32 &dwValue);
	bool GetValue_i32(const tuint_ptr ptrItem, const std::string &strName, tint32 &dwValue);	

    bool GetValue_ui64(const tuint_ptr ptrItem, tuint64 &dwValue);
	bool GetValue_ui64(const tuint_ptr ptrItem, const std::string &strName, tuint64 &dwValue);	

    bool GetValueGUID(const tuint_ptr ptrItem, GUID &GUIDValue);
    bool GetValueGUID(const tuint_ptr ptrItem, const std::string &strName, GUID &GUIDValue);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	// 获取属性值************************************************************************************************
    bool GetAttributeStr(const std::string &strName, const std::string &strAttrName, std::string &strValue);
    bool GetAttributeDWORD(const std::string &strName, const std::string &strAttrName, tuint32 &dwValue);
    bool GetAttribute_i32(const std::string &strName, const std::string &strAttrName, tint32 &dwValue);
    bool GetAttribute_ui64(const std::string &strName, const std::string &strAttrName, tuint64 &dwValue);
    bool GetAttributeGUID(const std::string &strName, const std::string &strAttrName, GUID &GUIDValue);

    bool GetAttributeStr(const tuint_ptr ptrItem, const std::string &strAttrName, std::string &strValue);
    bool GetAttributeDWORD(const tuint_ptr ptrItem, const std::string &strAttrName, tuint32 &dwValue);
    bool GetAttribute_i32(const tuint_ptr ptrItem, const std::string &strAttrName, tint32 &dwValue);
    bool GetAttribute_ui64(const tuint_ptr ptrItem, const std::string &strAttrName, tuint64 &dwValue);
    bool GetAttributeGUID(const tuint_ptr ptrItem, const std::string &strAttrName, GUID &GUIDValue);

	// 获取节点下的所有属性<属性名称, 属性值>, 获取失败返回false
	bool GetAttributes(const std::string &strNodeName, std::map<std::string, std::string> &AttributeMap);
	bool GetAttributes(tuint_ptr ptrItem, std::map<std::string, std::string> &AttributeMap);
	////////////////////////////////////////////////////////////////////////////////////////////////

	// 删除节点*****************************************************************************************
	
	// 删除节点(包括本节点)
	void DeleteValue(const std::string &strNodeName);
	void DeleteValue(const tuint_ptr ptrItem);

	// 删除节点下的所有子节点(保留本节点)
	void DeleteChildrenValue(const std::string &strNodeName);
	void DeleteChildrenValue(const tuint_ptr ptrItem);

	// 删除属性*****************************************************************************************
	void DeleteAttribute(const std::string &strNodeName, const std::string &strAttrName);
	void DeleteAttribute(const tuint_ptr ptrItem, const std::string &strAttrName);

	// 克隆节点****************************************************************************************
	bool CloneValue(const std::string &strName, const std::string &strXML);
	bool CloneValue(const tuint_ptr ptrItem, const std::string &strXML);

	// 以xml形式获取节点信息(包括所有子节点)***********************************************************
	bool GetXMLData(const std::string &strName, std::string &strXML);
	bool GetXMLData(const tuint_ptr ptrItem, std::string &strXML);

	// 以xml形式获取所有子节点信息**********************************************************************
	bool GetChildrenXMLData(const std::string &strName, std::string &strXML);
	bool GetChildrenXMLData(const tuint_ptr ptrItem, std::string &strXML);

	// 获取孩子节点的数量
	tint32 GetChildrenLength(const std::string &strName, const std::string &strChildName="");
	tint32 GetChildrenLength(const tuint_ptr ptrItem, const std::string &strChildName="");

	bool GetItemName(const tuint_ptr ptrItem, std::string &strItemName);

	// 获取根节点************************************************************************************************
	tuint_ptr GetRootItem(void)const;

	// 遍历操作****************************************************************************************
	tuint_ptr GetFirstItem(const std::string &strName);					// 根据名称获取第一个节点位置
	tuint_ptr GetFirstItem(tuint_ptr ptrItem, const std::string &strName);	// 获取节点ptrItem下名称为strName的节点
	tuint_ptr GetChildItem(const tuint_ptr ptrItem);						// 根据节点位置获取第一个子节点位置
	tuint_ptr GetNextSiblingItem(const tuint_ptr ptrItem);					// 根据节点位置获取下一个兄弟节点位置
	tuint_ptr GetNextSiblingItemByName(const tuint_ptr ptrItem,const string &strName); 
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// 获取xml文档内容
	const std::string &GetDoc(void);

private:
    CTVTXMLParse(const CTVTXMLParse  &/*xmlParse*/){;}
    const CTVTXMLParse &operator=(const CTVTXMLParse  &/*xmlParse*/){return *this;}

private:

	// 创建新元素
	XMLElement *NewElement(const std::string &strName, const std::string &strData ="", XMLNode *pParentNode=0, bool bCData = false);

	// 添加元素
	tuint_ptr AddElement(const std::string &strName, const std::string &strData="", bool bCData = false);
	tuint_ptr AddElement(const tuint_ptr ptrItem, const std::string &strName, const std::string &strData="", bool bCData = false);
	
	// 查找元素
	XMLElement *FindElement(const std::string &strElement, XMLElement *pXMLElement=0);

	bool SetText(XMLNode *pNode, const char* str, bool bCData);

	// 截取路径中的字符串
    void split(const std::string str, const std::string strPattern, std::vector<std::string> &strResultVec);

	// 克隆节点
	bool CloneNode(XMLNode *pDestNode, XMLNode *pSrcNode);

private:
	tinyxml2::XMLDocument m_XMLDocument;
	std::string m_strFile;
	std::string m_strDoc;
	std::string m_strXml;
};

#endif
