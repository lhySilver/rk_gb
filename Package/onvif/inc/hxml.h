#ifndef	__H_LT_XML_H__
#define	__H_LT_XML_H__

#ifdef __cplusplus
extern "C" {
#endif

#define LTXML_MAX_STACK_DEPTH	1024
#define LTXML_MAX_ATTR_NUM		128

typedef struct ltxd_xmlparser 
{
	char *	xmlstart;
	char *	xmlend;
	char *	ptr;		// pointer to current character
	int		xmlsize;

	char *	e_stack[LTXML_MAX_STACK_DEPTH];
	int		e_stack_index;					

	char *	attr[LTXML_MAX_ATTR_NUM];

	void *	userdata;
	void (*startElement)(void * userdata, const char * name, const char ** attr);
	void (*endElement)(void * userdata, const char * name);
	void (*charData)(void * userdata, const char * str, int len);
}LTXMLPRS;

int  hxml_parse_header(LTXMLPRS * parse);

int  hxml_parse_attr(LTXMLPRS * parse);
int  hxml_parse_element_end(LTXMLPRS * parse);
int  hxml_parse_element_start(LTXMLPRS * parse);

int  hxml_parse_element(LTXMLPRS * parse);

int  hxml_parse(LTXMLPRS * parse);

void xml_startElement(void * userdata, const char * name, const char ** attr);
void xml_endElement(void * userdata, const char * name);
void xml_charData(void * userdata, const char * str, int len);

#ifdef __cplusplus
}
#endif

#endif	//	__H_LT_XML_H__



