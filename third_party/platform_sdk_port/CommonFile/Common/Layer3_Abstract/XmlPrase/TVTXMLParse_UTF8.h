#include "tinyxml.h"
#include "base_type_define.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

class CTVTXMLParseUTF8
{
public:
	CTVTXMLParseUTF8(void);
	CTVTXMLParseUTF8(const char *pXML, tuint32 nLen);
	~CTVTXMLParseUTF8(void);

	// 加载XML字符串
	bool LoadXML(const std::string &strXML);

	// 遍历操作****************************************************************************************
	tuint_ptr GetFirstItem(const std::string &strName);					// 根据名称获取第一个节点位置
	tuint_ptr GetNextSiblingItem(const tuint_ptr ptrItem);					// 根据节点位置获取下一个兄弟节点位置
	tuint_ptr GetChildItem(const tuint_ptr ptrItem);						// 根据节点位置获取第一个子节点位置

	// 查找元素
	TiXmlElement *FindElement(const std::string &strElement, TiXmlElement *pXMLElement=0);
	// 截取路径中的字符串
	void split(const std::string str,const std::string strPattern, std::vector<std::string> &strResultVec);

	// 获取属性值************************************************************************************************
	bool GetAttributeStr(const tuint_ptr ptrItem, const std::string &strAttrName, std::string &strValue);	

	// 添加节点***********************************************************************************
	// (strName: 元素名称,支持路径名称,如：tvt/dvr; strData: 元素数据)
	const tuint_ptr AddValue(const std::string &strName, const std::string &strData="", bool bCData = false);
	// (ptrItem: 节点位置; strData: 元素数据)
	const tuint_ptr AddValue(const tuint_ptr ptrItem, const std::string &strName, const std::string &strData="", bool bCData = false);
	const tuint_ptr AddValue(const tuint_ptr ptrItem, const std::string &strName, const tuint32 dwData);

	// 创建新元素
	TiXmlElement *NewElement(const std::string &strName, const std::string &strData ="", TiXmlElement *pParentNode=0, bool bCData = false);
	// 添加元素
	tuint_ptr AddElement(const std::string &strName, const std::string &strData="", bool bCData = false);
	tuint_ptr AddElement(const tuint_ptr ptrItem, const std::string &strName, const std::string &strData="", bool bCData = false);

	// 获取节点值(返回值用来判断获取的节点是否存在)************************************************************
	bool GetValueStr(const std::string &strName, std::string &strValue);									 
	bool GetValueDWORD(const std::string &strName, tuint32 &dwValue);	

	bool GetValueStr(const tuint_ptr ptrItem, std::string &strValue);	
	bool GetValueDWORD(const tuint_ptr ptrItem, tuint32 &dwValue);	

	// 设置元素属性***************************************************************************************
	// (ptrItem: 节点位置, strAttrName: 属性名称; strData: 属性值)
	bool SetAttribute(const tuint_ptr ptrItem, const std::string &strAttrName, const std::string &strData="");

	// 删除属性*****************************************************************************************
	void DeleteAttribute(const std::string &strNodeName, const std::string &strAttrName);
	void DeleteAttribute(const tuint_ptr ptrItem, const std::string &strAttrName);


	// 以xml形式获取节点信息(包括所有子节点)***********************************************************
	bool GetXMLData(const std::string &strName, std::string &strXML);
	bool GetXMLData(const tuint_ptr ptrItem, std::string &strXML);

	// 获取xml文档内容
	const std::string &GetDoc(void);

	// 加载/保存xml文件
	bool LoadFile(const std::string &strFile);
	bool SaveFile(const std::string &strFile="");

	// 克隆节点
	bool CloneNode(TiXmlNode *pDestNode, TiXmlNode *pSrcNode);

	// 克隆节点****************************************************************************************
	bool CloneValue(const std::string &strName, const std::string &strXML);
	bool CloneValue(const tuint_ptr ptrItem, const std::string &strXML);
private:
	TiXmlDocument m_XMLDocument;
	std::string m_strFile;
	std::string m_strDoc;
};

