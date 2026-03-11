#include "SdpMessage.h"
#include "osipparser2/osip_port.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char * const STRING_EMPTY         = "";
const char * const STRING_SIP_SP        = " ";

/// 检查指针是否为空，为空返回
#define CHECK_RETURN(pointer) \
    if (pointer==0)\
    {\
    return;\
    }
/// 检查指针是否为空，为空返回指定值retv
#define CHECK_RETURN_VAL(pointer,retv)\
    if (pointer==0)\
    {\
    return retv;\
    }

/// 检查标示位flag，为3.71版本返回指定值retv
#define CHECK_RETURN_FLAG_VAL(flag,retv)\
    if (flag==PLATFORM_FLAG)\
    {\
    return retv;\
    }

/// 检查两个指针是否有空指针，如果存在空指针返回
#define CHECK_RETURN_2(pointer1,pointer2)\
    if (pointer1==0 || pointer2==0)\
    {\
    return;\
    }
/// 检查两个指针是否有空指针，如果存在空指针返回指定值retv
#define CHECK_RETURN_VAL_2(pointer1,pointer2,retv)\
    if (pointer1==0 || pointer2==0)\
    {\
    return retv;\
    }



std::string Int2String(int n)
{
	std::stringstream stream;
	stream << n;
	return stream.str();
}

int String2Int(std::string& str)
{
	std::istringstream is(str);
	int i;
	is >> i;
	return i;
}

//////////////////////////////////////////////////////////////////////////
CSipUri::CSipUri()
{
	osip_uri_init(&m_uri);
}

CSipUri::CSipUri(const char *uri)
{
	osip_uri_init(&m_uri);
	osip_uri_parse(m_uri,uri);
}

CSipUri::CSipUri(osip_uri *uri)
{
	if (m_uri)
		osip_uri_free(m_uri);
	m_uri = NULL;
	osip_uri_init(&m_uri);
	m_uri->host = osip_strdup(uri->host);
	m_uri->scheme = osip_strdup(uri->scheme);
	m_uri->username = osip_strdup(uri->username);
	m_uri->password = osip_strdup(uri->password);
	m_uri->port = osip_strdup(uri->port);
	m_uri->string = osip_strdup(uri->string);

	//int pos = 0;
	//while (!osip_list_eol(&uri->url_headers, pos))
	//{
	//	char* header = (char *) osip_list_get(&uri->url_headers, pos);
	//	if (header)
	//	{
	//		osip_list_add(&m_uri->url_headers, 
	//			(void *) osip_strdup(header), -1);
	//	}
	//	++pos;
	//}

	//pos = 0;
	//while (!osip_list_eol(&uri->url_params, pos))
	//{
	//	char* param = (char *) osip_list_get(&uri->url_params, pos);
	//	if (param)
	//	{
	//		osip_list_add(&m_uri->url_params, 
	//			(void *) osip_strdup(param), -1);
	//	}
	//	++pos;
	//}
}

CSipUri::CSipUri(const CSipUri &uri)
{
	if (this!=&uri)
	{
		osip_uri_clone(uri.m_uri,&m_uri);
	}
}

CSipUri& CSipUri::operator = (const CSipUri &uri)
{
	if (this != &uri)
	{
		if (m_uri)
			osip_uri_free(m_uri);
		m_uri = NULL;
		osip_uri_init(&m_uri);
		m_uri->host = osip_strdup(uri.m_uri->host);
		m_uri->scheme = osip_strdup(uri.m_uri->scheme);
		m_uri->username = osip_strdup(uri.m_uri->username);
		m_uri->password = osip_strdup(uri.m_uri->password);
		m_uri->port = osip_strdup(uri.m_uri->port);
		m_uri->string = osip_strdup(uri.m_uri->string);

		//int pos = 0;
		//while (!osip_list_eol(&uri.m_uri->url_headers, pos))
		//{
		//	char* header = (char *) osip_list_get(&uri.m_uri->url_headers, pos);
		//	if (header)
		//	{
		//		osip_list_add(&m_uri->url_headers, 
		//			(void *) osip_strdup(header), -1);
		//	}
		//	++pos;
		//}

		//pos = 0;
		//while (!osip_list_eol(&uri.m_uri->url_params, pos))
		//{
		//	char* param = (char *) osip_list_get(&uri.m_uri->url_params, pos);
		//	if (param)
		//	{
		//		osip_list_add(&m_uri->url_params, 
		//			(void *) osip_strdup(param), -1);
		//	}
		//	++pos;
		//}
	}
	return *this;
}

CSipUri& CSipUri::operator = (const osip_uri *uri)
{
	if (this->m_uri != uri)
	{
		if (m_uri)
			osip_uri_free(m_uri);
		m_uri = NULL;
		osip_uri_init(&m_uri);
		m_uri->host = osip_strdup(uri->host);
		m_uri->scheme = osip_strdup(uri->scheme);
		m_uri->username = osip_strdup(uri->username);
		m_uri->password = osip_strdup(uri->password);
		m_uri->port = osip_strdup(uri->port);
		m_uri->string = osip_strdup(uri->string);

		//int pos = 0;
		//while (!osip_list_eol(&uri->url_headers, pos))
		//{
		//	char* header = (char *) osip_list_get(&uri->url_headers, pos);
		//	if (header)
		//	{
		//		osip_list_add(&m_uri->url_headers, 
		//			(void *) osip_strdup(header), -1);
		//	}
		//	++pos;
		//}

		//pos = 0;
		//while (!osip_list_eol(&uri->url_params, pos))
		//{
		//	char* param = (char *) osip_list_get(&uri->url_params, pos);
		//	if (param)
		//	{
		//		osip_list_add(&m_uri->url_params, 
		//			(void *) osip_strdup(param), -1);
		//	}
		//	++pos;
		//}
	}
	return *this;
}

CSipUri& CSipUri::operator = (const char *uri)
{
	if (m_uri!=NULL)
		osip_uri_free(m_uri);
	m_uri = NULL;
	osip_uri_init(&m_uri);
	osip_uri_parse(m_uri,uri);
	return *this;
}


CSipUri::~CSipUri()
{
	osip_uri_free(m_uri);
	m_uri = NULL;
}

char* CSipUri::getUsername()
{
	return m_uri->username;
}

char* CSipUri::getHost()
{
	return m_uri->host;
}

char* CSipUri::getPort()
{
	return m_uri->port;
}

char* CSipUri::getString()
{
	if (m_uri->string==NULL)
	{
		char szTmp[256] = {0};
		sprintf(szTmp,"<%s:%s@%s:%s>", m_uri->scheme,
			m_uri->username, 
			m_uri->host,
			m_uri->port?m_uri->port:"5060");
		m_uri->string = osip_strdup(szTmp);
	}
	return m_uri->string;
}

//////////////////////////////////////////////////////////////////////////
CSdpBandwidth::CSdpBandwidth()
{
    sdp_bandwidth_init(&m_pBandwidth);
}

CSdpBandwidth::CSdpBandwidth(const char *type, const char *width)
{
    sdp_bandwidth_init(&m_pBandwidth);
    m_pBandwidth->b_bwtype = osip_strdup(type);
    m_pBandwidth->b_bandwidth = osip_strdup(width);
}

CSdpBandwidth::CSdpBandwidth(const CSdpBandwidth &bandwidth)
{
    if (this != &bandwidth)
    {
        m_pBandwidth = NULL;
        CHECK_RETURN(bandwidth.m_pBandwidth);

        sdp_bandwidth_init(&m_pBandwidth);
        m_pBandwidth->b_bwtype = 
            osip_strdup(bandwidth.m_pBandwidth->b_bwtype);
        m_pBandwidth->b_bandwidth = 
            osip_strdup(bandwidth.m_pBandwidth->b_bandwidth);
    }
}

CSdpBandwidth::~CSdpBandwidth()
{
    sdp_bandwidth_free(m_pBandwidth);
    m_pBandwidth = NULL;
}

CSdpBandwidth& CSdpBandwidth::operator = (const CSdpBandwidth &bandwidth)
{
    if (this != &bandwidth)
    {
        if (m_pBandwidth)
            sdp_bandwidth_free(m_pBandwidth);
        m_pBandwidth = NULL;
        CHECK_RETURN_VAL(bandwidth.m_pBandwidth, *this);

        sdp_bandwidth_init(&m_pBandwidth);
        m_pBandwidth->b_bwtype = 
            osip_strdup(bandwidth.m_pBandwidth->b_bwtype);
        m_pBandwidth->b_bandwidth = 
            osip_strdup(bandwidth.m_pBandwidth->b_bandwidth);
    }
    return *this;
}

int CSdpBandwidth::Attach(sdp_bandwidth_t *bandwidth)
{
    if (m_pBandwidth != bandwidth)
    {
        sdp_bandwidth_free(m_pBandwidth);
        m_pBandwidth = bandwidth;
    }
    return 0;
}

sdp_bandwidth_t* CSdpBandwidth::Detach()
{
    sdp_bandwidth_t *bandwidth = m_pBandwidth;
    m_pBandwidth = NULL;
    return bandwidth;
}

int CSdpBandwidth::SetBandwidth(sdp_bandwidth_t *bandwidth)
{
    if (NULL == bandwidth)
    {
        return Attach(bandwidth);
    }

    if (m_pBandwidth)
    {
        osip_free(m_pBandwidth->b_bandwidth);
        osip_free(m_pBandwidth->b_bwtype);
    }
    else
    {
        sdp_bandwidth_init(&m_pBandwidth);
    }

    m_pBandwidth->b_bandwidth = osip_strdup(bandwidth->b_bandwidth);
    m_pBandwidth->b_bwtype = osip_strdup(bandwidth->b_bwtype);
    return 0;
}

int CSdpBandwidth::SetType(const char *type)
{
    if (m_pBandwidth)
    {
        osip_free(m_pBandwidth->b_bwtype);
    }
    else
    {
        sdp_bandwidth_init(&m_pBandwidth);
    }

    osip_free(m_pBandwidth->b_bwtype);
    m_pBandwidth->b_bwtype = osip_strdup(type);
    return 0;
}

const char* CSdpBandwidth::GetType()
{
    CHECK_RETURN_VAL(m_pBandwidth, NULL);
    return m_pBandwidth->b_bwtype;
}

int CSdpBandwidth::SetWidth(const char *width)
{
    if (m_pBandwidth)
    {
        osip_free(m_pBandwidth->b_bandwidth);
    }
    else
    {
        sdp_bandwidth_init(&m_pBandwidth);
    }
    m_pBandwidth->b_bandwidth = osip_strdup(width);
    return 0;
}

const char* CSdpBandwidth::GetWidth()
{
    CHECK_RETURN_VAL(m_pBandwidth, NULL);
    return m_pBandwidth->b_bandwidth;
}

bool CSdpBandwidth::Invalid()
{
    return (NULL == m_pBandwidth);
}

//////////////////////////////////////////////////////////////////////////
CSdpTimeDescr::CSdpTimeDescr()
{
    sdp_time_descr_init(&m_pTimeDescr);
    m_pTimeDescr->t_start_time = osip_strdup("0");
    m_pTimeDescr->t_stop_time  = osip_strdup("0");
}

CSdpTimeDescr::CSdpTimeDescr(const char *start, const char *stop)
{
    sdp_time_descr_init(&m_pTimeDescr);
    m_pTimeDescr->t_start_time = osip_strdup(start);
    m_pTimeDescr->t_stop_time = osip_strdup(stop);
}

CSdpTimeDescr::CSdpTimeDescr(const CSdpTimeDescr &timeDescr)
{
	m_pTimeDescr = NULL;
    *this = timeDescr;
}

CSdpTimeDescr::~CSdpTimeDescr()
{
    sdp_time_descr_free(m_pTimeDescr);
    m_pTimeDescr = NULL;
}

CSdpTimeDescr& CSdpTimeDescr::operator = (const CSdpTimeDescr &timeDescr)
{
    if (this != &timeDescr)
    {
        if (m_pTimeDescr)
            sdp_time_descr_free(m_pTimeDescr);
        m_pTimeDescr = NULL;
        CHECK_RETURN_VAL(timeDescr.m_pTimeDescr, *this);

        sdp_time_descr_init(&m_pTimeDescr);
        m_pTimeDescr->t_start_time = osip_strdup(timeDescr.m_pTimeDescr->t_start_time);
        m_pTimeDescr->t_stop_time  = osip_strdup(timeDescr.m_pTimeDescr->t_stop_time);
        int pos = 0;
        while (!osip_list_eol(&timeDescr.m_pTimeDescr->r_repeats, pos))
        {
            char *repeat = (char *) osip_list_get(&timeDescr.m_pTimeDescr->r_repeats, pos);
            if (repeat)
            {
                osip_list_add(&m_pTimeDescr->r_repeats, 
                    (void *) osip_strdup(repeat), -1);
            }
            ++pos;
        }
    }

    return *this;
}

int CSdpTimeDescr::Attach(sdp_time_descr_t *timeDescr)
{
    if (m_pTimeDescr != timeDescr)
    {
        sdp_time_descr_free(m_pTimeDescr);
        m_pTimeDescr = timeDescr;
    }
    return 0;
}

sdp_time_descr_t* CSdpTimeDescr::Detach()
{
    sdp_time_descr_t *timeDescr = m_pTimeDescr;
    m_pTimeDescr = NULL;
    return timeDescr;
}

int CSdpTimeDescr::SetTimeDescr(sdp_time_descr_t *timeDescr)
{
    if (NULL == timeDescr)
    {
        return Attach(timeDescr);
    }

    if (m_pTimeDescr)
    {
        sdp_time_descr_free(m_pTimeDescr);
    }
    sdp_time_descr_init(&m_pTimeDescr);
    m_pTimeDescr->t_start_time = osip_strdup(timeDescr->t_start_time);
    m_pTimeDescr->t_stop_time  = osip_strdup(timeDescr->t_stop_time);
    int pos = 0;
    while (!osip_list_eol(&m_pTimeDescr->r_repeats, pos))
    {
        char* repeat = (char *) osip_list_get(&m_pTimeDescr->r_repeats, pos);
        if (repeat)
        {
            osip_list_add(&m_pTimeDescr->r_repeats, 
                          (void *) osip_strdup(repeat), -1);
        }
        ++pos;
    }
    return 0;
}

int CSdpTimeDescr::SetTime(const char *start, const char *stop)
{
    if (m_pTimeDescr)
    {
        osip_free(m_pTimeDescr->t_start_time);
        osip_free(m_pTimeDescr->t_stop_time);
    }
    else
    {
        sdp_time_descr_init(&m_pTimeDescr);
    }

    m_pTimeDescr->t_start_time = osip_strdup(start);
    m_pTimeDescr->t_stop_time = osip_strdup(stop);
    return 0;
}

int CSdpTimeDescr::SetStartTime(const char *startTime)
{
    if (m_pTimeDescr)
    {
        osip_free(m_pTimeDescr->t_start_time);
    }
    else
    {
        sdp_time_descr_init(&m_pTimeDescr);
    }

    m_pTimeDescr->t_start_time = osip_strdup(startTime);
    return 0;
}

const char* CSdpTimeDescr::GetStartTime()
{
    CHECK_RETURN_VAL(m_pTimeDescr, NULL);
    return m_pTimeDescr->t_start_time;
}

int CSdpTimeDescr::SetStopTime(const char *stopTime)
{
    if (m_pTimeDescr)
    {
        osip_free(m_pTimeDescr->t_stop_time);
    }
    else
    {
        sdp_time_descr_init(&m_pTimeDescr);
    }

    m_pTimeDescr->t_stop_time = osip_strdup(stopTime);
    return 0;
}

const char* CSdpTimeDescr::GetStopTime()
{
    CHECK_RETURN_VAL(m_pTimeDescr, NULL);
    return m_pTimeDescr->t_stop_time;
}

int CSdpTimeDescr::AddRepeat(const char *repeat)
{
    CHECK_RETURN_VAL_2(m_pTimeDescr, repeat, -1);
    osip_list_add(&m_pTimeDescr->r_repeats, osip_strdup(repeat), -1);
    return 0;
}

const char* CSdpTimeDescr::GetRepeat(int pos)
{
    CHECK_RETURN_VAL(m_pTimeDescr, NULL);
    return (const char *) osip_list_get(&m_pTimeDescr->r_repeats, pos);
}

bool CSdpTimeDescr::Invalid()
{
    return (NULL == m_pTimeDescr);
}

//////////////////////////////////////////////////////////////////////////
CSdpKey::CSdpKey()
{
    sdp_key_init(&m_pKey);
}

CSdpKey::CSdpKey(const CSdpKey &key)
{
    if (this != &key)
    {
        m_pKey = NULL;
        CHECK_RETURN(key.m_pKey);

        sdp_key_init(&m_pKey);
        m_pKey->k_keydata = osip_strdup(key.m_pKey->k_keydata);
        m_pKey->k_keytype = osip_strdup(key.m_pKey->k_keytype);
    }
}

CSdpKey::~CSdpKey()
{
    sdp_key_free(m_pKey);
}

int CSdpKey::Attach(sdp_key_t *key)
{
    if (m_pKey != key)
    {
        sdp_key_free(m_pKey);
        m_pKey = key;
    }
    return 0;
}

sdp_key_t* CSdpKey::Detach()
{
    sdp_key_t *key = m_pKey;
    m_pKey = NULL;
    return key;
}

int CSdpKey::SetKey(sdp_key_t *key)
{
    if (NULL == key)
    {
        return Attach(key);
    }

    if (m_pKey)
    {
        osip_free(m_pKey->k_keydata);
        osip_free(m_pKey->k_keytype);
    }
    else
    {
        sdp_key_init(&m_pKey);
    }

    m_pKey->k_keydata = osip_strdup(key->k_keydata);
    m_pKey->k_keytype = osip_strdup(key->k_keytype);
    return 0;
}

int CSdpKey::SetType(const char *type)
{
    if (m_pKey)
    {
        osip_free(m_pKey->k_keytype);
    }
    else
    {
        sdp_key_init(&m_pKey);
    }

    m_pKey->k_keytype = osip_strdup(type);
    return 0;
}

const char* CSdpKey::GetType()
{
    CHECK_RETURN_VAL(m_pKey, NULL);
    return m_pKey->k_keytype;
}

int CSdpKey::SetData(const char *data)
{
    if (m_pKey)
    {
        osip_free(m_pKey->k_keydata);
    }
    else
    {
        sdp_key_init(&m_pKey);
    }

    m_pKey->k_keydata = osip_strdup(data);
    return 0;
}

const char* CSdpKey::GetData()
{
    CHECK_RETURN_VAL(m_pKey, NULL);
    return m_pKey->k_keydata;
}

bool CSdpKey::Invalid()
{
    return (NULL == m_pKey);
}

//////////////////////////////////////////////////////////////////////////
CSdpAttribute::CSdpAttribute()
{
    sdp_attribute_init(&m_pAttribute);
}

CSdpAttribute::CSdpAttribute(const char *field)
{
    sdp_attribute_init(&m_pAttribute);
    m_pAttribute->a_att_field = osip_strdup(field);
}

CSdpAttribute::CSdpAttribute(const char *field, const char *value)
{
    sdp_attribute_init(&m_pAttribute);
    m_pAttribute->a_att_field = osip_strdup(field);
    m_pAttribute->a_att_value = osip_strdup(value);
}

CSdpAttribute::CSdpAttribute(const CSdpAttribute &attr)
{
    if (this != &attr)
    {
        m_pAttribute = NULL;
        CHECK_RETURN(attr.m_pAttribute);

        sdp_attribute_init(&m_pAttribute);
        m_pAttribute->a_att_field = osip_strdup(attr.m_pAttribute->a_att_field);
        m_pAttribute->a_att_value = osip_strdup(attr.m_pAttribute->a_att_value);
    }
}

CSdpAttribute::~CSdpAttribute()
{
    sdp_attribute_free(m_pAttribute);
    m_pAttribute = NULL;
}

int CSdpAttribute::Attach(sdp_attribute_t *attr)
{
    if (m_pAttribute != attr)
    {
        sdp_attribute_free(m_pAttribute);
        m_pAttribute = attr;
    }
    return 0;
}

sdp_attribute_t* CSdpAttribute::Detach()
{
    sdp_attribute_t *pAttr = m_pAttribute;
    m_pAttribute = NULL;
    return pAttr;
}

CSdpAttribute& CSdpAttribute::operator = (const CSdpAttribute &attr)
{
    if (this != &attr)
    {
        if (m_pAttribute)
            sdp_attribute_free(m_pAttribute);
        m_pAttribute = NULL;
        CHECK_RETURN_VAL(attr.m_pAttribute, *this);

        sdp_attribute_init(&m_pAttribute);
        m_pAttribute->a_att_field = osip_strdup(attr.m_pAttribute->a_att_field);
        m_pAttribute->a_att_value = osip_strdup(attr.m_pAttribute->a_att_value);
    }
    return *this;
}

int CSdpAttribute::SetAttribute(sdp_attribute_t *attr)
{
    if (NULL == attr)
    {
        return Attach(attr);
    }

    return SetAttribute(attr->a_att_field, attr->a_att_value);
}

int CSdpAttribute::SetAttribute(const char *field)
{
    if (m_pAttribute)
    {
        osip_free(m_pAttribute->a_att_field);
        osip_free(m_pAttribute->a_att_value);
    }
    else
    {
        sdp_attribute_init(&m_pAttribute);
    }

    m_pAttribute->a_att_field = osip_strdup(field);
    m_pAttribute->a_att_value = NULL;
    return 0;
}

int CSdpAttribute::SetAttribute(const char *field, const char *value)
{
    if (m_pAttribute)
    {
        osip_free(m_pAttribute->a_att_field);
        osip_free(m_pAttribute->a_att_value);
    }
    else
    {
        sdp_attribute_init(&m_pAttribute);
    }

    m_pAttribute->a_att_field = osip_strdup(field);
    m_pAttribute->a_att_value = osip_strdup(value);
    return 0;
}

std::string CSdpAttribute::GetField()
{
    CHECK_RETURN_VAL(m_pAttribute, NULL);
    return m_pAttribute->a_att_field ? m_pAttribute->a_att_field : "";
}

std::string CSdpAttribute::GetValue()
{
    CHECK_RETURN_VAL(m_pAttribute, NULL);
    return m_pAttribute->a_att_value ? m_pAttribute->a_att_value : "";
}

bool CSdpAttribute::Invalid()
{
    return (NULL == m_pAttribute);
}

//////////////////////////////////////////////////////////////////////////
CSdpConnection::CSdpConnection()
{
    sdp_connection_init(&m_pConn);
    m_pConn->c_nettype = osip_strdup("IN");
    m_pConn->c_addrtype = osip_strdup("IP4");
}

CSdpConnection::CSdpConnection(const char *address)
{
    sdp_connection_init(&m_pConn);
    m_pConn->c_addr = osip_strdup(address);
    m_pConn->c_nettype = osip_strdup("IN");
    m_pConn->c_addrtype = osip_strdup("IP4");
}

CSdpConnection::CSdpConnection(const CSdpConnection &conn)
{
	m_pConn = NULL;
	*this = conn;
}

CSdpConnection& CSdpConnection::operator = (const CSdpConnection &conn)
{
	if (this != &conn)
	{
		if (m_pConn)
            sdp_connection_free(m_pConn);
        m_pConn = NULL;
        CHECK_RETURN_VAL(conn.m_pConn, *this);

        sdp_connection_init(&m_pConn);
        m_pConn->c_nettype = osip_strdup(conn.m_pConn->c_nettype);
        m_pConn->c_addrtype = osip_strdup(conn.m_pConn->c_addrtype);
        m_pConn->c_addr = osip_strdup(conn.m_pConn->c_addr);
        m_pConn->c_addr_multicast_ttl = osip_strdup(conn.m_pConn->c_addr_multicast_ttl);
        m_pConn->c_addr_multicast_int = osip_strdup(conn.m_pConn->c_addr_multicast_int);
	}

	return *this;
}

CSdpConnection::~CSdpConnection()
{
    sdp_connection_free(m_pConn);
}

int CSdpConnection::Attach(sdp_connection_t *conn)
{
    if (m_pConn != conn)
    {
        sdp_connection_free(m_pConn);
        m_pConn = conn;
    }
    return 0;
}

sdp_connection_t* CSdpConnection::Detach()
{
    sdp_connection_t *conn = m_pConn;
    m_pConn = NULL;
    return conn;
}

int CSdpConnection::SetConnection(sdp_connection_t *conn)
{
    if (NULL == conn)
    {
        return Attach(conn);
    }

    if (m_pConn)
    {
        osip_free(m_pConn->c_nettype);
        osip_free(m_pConn->c_addrtype);
        osip_free(m_pConn->c_addr);
        osip_free(m_pConn->c_addr_multicast_ttl);
        osip_free(m_pConn->c_addr_multicast_int);
    }
    else
    {
        // After detach operation, m_pConn maybe NULL.
        sdp_connection_init(&m_pConn);
    }

    m_pConn->c_nettype = osip_strdup(conn->c_nettype);
    m_pConn->c_addrtype = osip_strdup(conn->c_addrtype);
    m_pConn->c_addr = osip_strdup(conn->c_addr);
    m_pConn->c_addr_multicast_ttl = osip_strdup(conn->c_addr_multicast_ttl);
    m_pConn->c_addr_multicast_int = osip_strdup(conn->c_addr_multicast_int);
    return 0;
}

int CSdpConnection::SetNetType(const char *netType)
{
    if (m_pConn)
    {
        osip_free(m_pConn->c_nettype);
    }
    else
    {
        sdp_connection_init(&m_pConn);
    }
    m_pConn->c_nettype = osip_strdup(netType);
    return 0;
}

const char* CSdpConnection::GetNetType()
{
    CHECK_RETURN_VAL(m_pConn, NULL);
    return m_pConn->c_nettype;
}

int CSdpConnection::SetAddrType(const char *addrType)
{
    if (m_pConn)
    {
        osip_free(m_pConn->c_addrtype);
    }
    else
    {
        sdp_connection_init(&m_pConn);
    }
    m_pConn->c_addrtype = osip_strdup(addrType);
    return 0;
}

const char* CSdpConnection::GetAddrType()
{
    CHECK_RETURN_VAL(m_pConn, NULL);
    return m_pConn->c_addrtype;
}

int CSdpConnection::SetAddress(const char *address)
{
    if (m_pConn)
    {
        osip_free(m_pConn->c_addr);
    }
    else
    {
        sdp_connection_init(&m_pConn);
    }
    m_pConn->c_addr = osip_strdup(address);
    return 0;
}

const char* CSdpConnection::GetAddress()
{
    CHECK_RETURN_VAL(m_pConn, NULL);
    return m_pConn->c_addr;
}

int CSdpConnection::SetAddrMulticastTTL(const char *ttl)
{
    if (m_pConn)
    {
        osip_free(m_pConn->c_addr_multicast_ttl);
    }
    else
    {
        sdp_connection_init(&m_pConn);
    }
    m_pConn->c_addr_multicast_ttl = osip_strdup(ttl);
    return 0;
}

const char* CSdpConnection::GetAddrMulticastTTL()
{
    CHECK_RETURN_VAL(m_pConn, NULL);
    return m_pConn->c_addr_multicast_ttl;
}

int CSdpConnection::SetAddrMulticastNumber(const char *number)
{
    if (m_pConn)
    {
        osip_free(m_pConn->c_addr_multicast_int);
    }
    else
    {
        sdp_connection_init(&m_pConn);
    }
    m_pConn->c_addr_multicast_int = osip_strdup(number);
    return 0;
}

const char* CSdpConnection::GetAddrMulticastNumber()
{
    CHECK_RETURN_VAL(m_pConn, NULL);
    return m_pConn->c_addr_multicast_int;
}

bool CSdpConnection::Invalid()
{
    return (NULL == m_pConn);
}

//////////////////////////////////////////////////////////////////////////
CSdpMedia::CSdpMedia()
{
    sdp_media_init(&m_pMedia);
    m_pMedia->m_port = osip_strdup("0");
    __InitVisitor();
}

CSdpMedia::CSdpMedia(const CSdpMedia &media)
{
    if (this != &media)
    {
        m_pMedia = NULL;
	    *this = media;
        __InitVisitor();
    }
}

CSdpMedia::~CSdpMedia()
{
    sdp_media_free(m_pMedia);
    __FreeVisitor();
}

CSdpMedia& CSdpMedia::operator = (const CSdpMedia &media)
{
    if (this != &media)
    {
		if (m_pMedia)
            sdp_media_free(m_pMedia);
        m_pMedia = NULL;
        CHECK_RETURN_VAL(media.m_pMedia, *this);

		sdp_media_init(&m_pMedia);
        m_pMedia->m_media = osip_strdup(media.m_pMedia->m_media);
        m_pMedia->m_port  = osip_strdup(media.m_pMedia->m_port);
        m_pMedia->m_number_of_port = 
            osip_strdup(media.m_pMedia->m_number_of_port);
        m_pMedia->m_proto = osip_strdup(media.m_pMedia->m_proto);
        m_pMedia->i_info  = osip_strdup(media.m_pMedia->i_info);

        // clone payloads
        ClonePayloadList(&media.m_pMedia->m_payloads);
        // clone connections
        CloneConnectionList(&media.m_pMedia->c_connections);
        // clone attributes
        CloneAttributeList(&media.m_pMedia->a_attributes);
        // clone bandwidths
        CloneBandwidthList(&media.m_pMedia->b_bandwidths);
        // clone key
        CloneKey(media.m_pMedia->k_key);
    }

	return *this;
}

void CSdpMedia::__InitVisitor()
{
    m_key.SetKey(static_cast<sdp_key_t *>(NULL));
    m_bandwidth.SetBandwidth(static_cast<sdp_bandwidth_t *>(NULL));
    m_connection.SetConnection(static_cast<sdp_connection_t *>(NULL));
    m_attribute.SetAttribute(static_cast<sdp_attribute_t *>(NULL));
}

void CSdpMedia::__FreeVisitor()
{
    m_key.Detach();
    m_bandwidth.Detach();
    m_connection.Detach();
    m_attribute.Detach();
}

int CSdpMedia::Attach(sdp_media_t *media)
{
    if (m_pMedia != media)
    {
        sdp_media_free(m_pMedia);
        m_pMedia = media;
    }
    return 0;
}

sdp_media_t* CSdpMedia::Detach()
{
    sdp_media_t *media = m_pMedia;
    m_pMedia = NULL;
    return media;
}

int CSdpMedia::SetMedia(sdp_media_t *media)
{
    if (NULL == media)
    {
        return Attach(media);
    }

    if (m_pMedia)
    {
        sdp_media_free(m_pMedia);
    }
    sdp_media_init(&m_pMedia);
    m_pMedia->m_media = osip_strdup(media->m_media);
    m_pMedia->m_port  = osip_strdup(media->m_port);
    m_pMedia->m_number_of_port = 
        osip_strdup(media->m_number_of_port);
    m_pMedia->m_proto = osip_strdup(media->m_proto);
    m_pMedia->i_info  = osip_strdup(media->i_info);
    // clone payloads
    ClonePayloadList(&media->m_payloads);
    // clone connections
    CloneConnectionList(&media->c_connections);
    // clone attributes
    CloneAttributeList(&media->a_attributes);
    // clone bandwidths
    CloneBandwidthList(&media->b_bandwidths);
    // clone key
    CloneKey(media->k_key);
    return 0;
}

int CSdpMedia::SetType(const char *type)
{
    if (m_pMedia)
    {
        osip_free(m_pMedia->m_media);
    }
    else
    {
        sdp_media_init(&m_pMedia);
    }
    m_pMedia->m_media = osip_strdup(type);
    return 0;
}

std::string CSdpMedia::GetType()
{
    CHECK_RETURN_VAL_2(m_pMedia, m_pMedia->m_media, "");
    return m_pMedia->m_media;
}

int CSdpMedia::SetPort11(int port)
{
    if (m_pMedia)
    {
        osip_free(m_pMedia->m_port);
    }
    else
    {
        sdp_media_init(&m_pMedia);
    }

    m_pMedia->m_port = osip_strdup(Int2String(port).c_str());

    return 0;
}

int CSdpMedia::GetPort()
{
    CHECK_RETURN_VAL_2(m_pMedia, m_pMedia->m_port, -1);
    std::string port(m_pMedia->m_port);
    return String2Int(port);
}

int CSdpMedia::GetPortCount()
{
    CHECK_RETURN_VAL(m_pMedia, 0);
    if (m_pMedia->m_number_of_port != NULL)
    {
        std::string portCount(m_pMedia->m_number_of_port);
        return String2Int(portCount);
    }
    else if (m_pMedia->m_port != NULL)
    {
        // 默认端口数量为1
        return 1;
    }

    return 0;
}

int CSdpMedia::SetProtocol(const char *protocol)
{
    if (m_pMedia)
    {
        osip_free(m_pMedia->m_proto);
    }
    else
    {
        sdp_media_init(&m_pMedia);
    }

    m_pMedia->m_proto = osip_strdup(protocol);
    return 0;
}

std::string CSdpMedia::GetProtocol()
{
    CHECK_RETURN_VAL_2(m_pMedia, m_pMedia->m_proto, "");
    return m_pMedia->m_proto;
}

/************************************************************************/
/* payload                                                              */
/************************************************************************/
int CSdpMedia::SetPayload(const char *payload, int pos /* = -1 */)
{
    CHECK_RETURN_VAL_2(m_pMedia, payload, -1);
    if (pos < 0)
    {
        ClearPayload();
    }
    else
    {
        RemovePayload(pos);
    }
    return AddPayload(payload);
}

int CSdpMedia::AddPayload(const char *payload)
{
    CHECK_RETURN_VAL_2(m_pMedia, payload, -1);
    osip_list_add(&m_pMedia->m_payloads, (void *) osip_strdup(payload), -1);
    return 0;
}

std::string CSdpMedia::GetPayload(int pos)
{
    CHECK_RETURN_VAL(m_pMedia, "");
    const char *payload = (const char *) osip_list_get(&m_pMedia->m_payloads, pos);
    return (payload) ? payload : "";
}

int CSdpMedia::GetPayloadCount()
{
    CHECK_RETURN_VAL(m_pMedia, 0);
    return osip_list_size(&m_pMedia->m_payloads);
}

int CSdpMedia::RemovePayload(int pos)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    char *payload = (char *) osip_list_get(&m_pMedia->m_payloads, pos);
    if (payload)
    {
        osip_free(payload);
        return 0;
    }
    return -1;
}

int CSdpMedia::ClearPayload()
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    osip_list_ofchar_free(&m_pMedia->m_payloads);
    return 0;
}

/**
 * @brief 判断CSdpMedia对象是否为无效的
 * 当m_pMedia成员为空时则CSdpMedia为无效对象
 * @return = RET_SUCCESS success
 * @return = RET_FAILED  failed
 **/
bool CSdpMedia::Invalid()
{
    return m_pMedia == NULL || m_pMedia->m_media == NULL;
}

/************************************************************************/
/* i                                                                    */
/************************************************************************/
int CSdpMedia::SetInfo(const char *info)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    osip_free(m_pMedia->i_info);
    m_pMedia->i_info = osip_strdup(info);
    return 0;
}

std::string CSdpMedia::GetInfo()
{
    CHECK_RETURN_VAL_2(m_pMedia, m_pMedia->i_info, "");
    return m_pMedia->i_info;
}

/************************************************************************/
/* c                                                                    */
/************************************************************************/
int CSdpMedia::AddConnection(CSdpConnection conn)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    sdp_connection_t *connection = conn.Detach();
    CHECK_RETURN_VAL(connection, -1);
    osip_list_add(&m_pMedia->c_connections, (void *) connection, -1);
    return 0;
}

CSdpConnection& CSdpMedia::GetConnection(int pos)
{
    m_connection.Detach();
    CHECK_RETURN_VAL(m_pMedia, m_connection);
    sdp_connection_t* connection = 
        (sdp_connection_t *) osip_list_get(&m_pMedia->c_connections, pos);
    m_connection.Attach(connection);
    return m_connection;
}

/************************************************************************/
/* b                                                                    */
/************************************************************************/
int CSdpMedia::AddBandwidth(CSdpBandwidth bandwidth)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    sdp_bandwidth_t *bw = bandwidth.Detach();
    CHECK_RETURN_VAL(bw, -1);
    osip_list_add(&m_pMedia->b_bandwidths, (void *) bw, -1);
    return 0;
}

CSdpBandwidth& CSdpMedia::GetBandwidth(int pos)
{
    m_bandwidth.Detach();
    CHECK_RETURN_VAL(m_pMedia, m_bandwidth);
    sdp_bandwidth_t* bw = 
        (sdp_bandwidth_t *) osip_list_get(&m_pMedia->b_bandwidths, pos);
    m_bandwidth.Attach(bw);
    return m_bandwidth;
}

/************************************************************************/
/* k                                                                    */
/************************************************************************/
int CSdpMedia::SetKey(CSdpKey key)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    m_pMedia->k_key = key.Detach();
    return 0;
}

CSdpKey& CSdpMedia::GetKey()
{
    m_key.Detach();
    CHECK_RETURN_VAL(m_pMedia, m_key);
    m_key.Attach(m_pMedia->k_key);
    return m_key;
}

/************************************************************************/
/* a                                                                    */
/************************************************************************/
int CSdpMedia::AddAttribute(const char *field)
{
    CHECK_RETURN_VAL(field, -1);
    CSdpAttribute attr(field);
    return AddAttribute(attr);
}

int CSdpMedia::AddAttribute(const char *field, const char *value)
{
    CHECK_RETURN_VAL_2(field, value, -1);
    CSdpAttribute attr(field, value);
    return AddAttribute(attr);
}

int CSdpMedia::AddAttribute(CSdpAttribute attribute)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    sdp_attribute_t *attr = attribute.Detach();
    CHECK_RETURN_VAL(attr, -1);
    osip_list_add(&m_pMedia->a_attributes, (void *) attr, -1);
    return 0;
}

CSdpAttribute& CSdpMedia::GetAttribute(int pos)
{
    m_attribute.Detach();
    CHECK_RETURN_VAL(m_pMedia, m_attribute);
    sdp_attribute_t *attr = 
        (sdp_attribute_t *) osip_list_get(&m_pMedia->a_attributes, pos);
    m_attribute.Attach(attr);
    return m_attribute;
}

CSdpAttribute& CSdpMedia::GetAttribute(const char *field)
{
    m_attribute.Detach();
    CHECK_RETURN_VAL_2(m_pMedia, field, m_attribute);
    osip_list_iterator_t it;
    osip_list_get_first(&m_pMedia->a_attributes, &it);
    while (osip_list_iterator_has_elem(it))
    {
        sdp_attribute_t *attr = 
            (sdp_attribute_t *) (it.actual->element);
        if (osip_strcasecmp(field, attr->a_att_field) == 0)
        {
            m_attribute.Attach(attr);
            break;
        }
        osip_list_get_next(&it);
    }
    return m_attribute;
}

std::string CSdpMedia::GetAttrValue(const char *field)
{
    std::string value("");
    CHECK_RETURN_VAL_2(field, m_pMedia, value);
    osip_list_iterator_t it;
    osip_list_get_first(&m_pMedia->a_attributes, &it);
    while (osip_list_iterator_has_elem(it))
    {
        sdp_attribute_t *attr = 
            (sdp_attribute_t *) (it.actual->element);
        if (osip_strcasecmp(field, attr->a_att_field) == 0)
        {
            value = attr->a_att_value ? attr->a_att_value : "";
            break;
        }
        osip_list_get_next(&it);
    }
    return value;
}

int CSdpMedia::RemoveAttribute(int pos)
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    sdp_attribute_t *attr = (sdp_attribute_t *) osip_list_get(&m_pMedia->a_attributes, pos);
    if (attr)
    {
        osip_list_remove(&m_pMedia->a_attributes, pos);
        sdp_attribute_free(attr);
        return 0;
    }
    return -1;
}

int CSdpMedia::RemoveAttribute(const char *field)
{
    CHECK_RETURN_VAL_2(m_pMedia, field, -1);
    int pos = 0;
    while (!osip_list_eol(&m_pMedia->a_attributes, pos))
    {
        sdp_attribute_t *attr = (sdp_attribute_t *) osip_list_get(&m_pMedia->a_attributes, pos);
        if (attr && 0 == osip_strcasecmp(attr->a_att_field, field))
        {
            // 注意删除节点后，pos不移动
            osip_list_remove(&m_pMedia->a_attributes, pos);
            sdp_attribute_free(attr);
        }
        else
        {
            ++pos;
        }
    }
    return 0;
}

int CSdpMedia::GetAttrCount()
{
    return osip_list_size(&m_pMedia->a_attributes);
}

int CSdpMedia::ClearAttribute()
{
    CHECK_RETURN_VAL(m_pMedia, -1);
    osip_list_special_free(&m_pMedia->a_attributes, 
        (void (*)(void *)) &sdp_attribute_free);
    return 0;
}

/**
 * @brief m的fmt与proto相关，在RFC4566中描述为：
 * 第四个字段开始到后面所有的内容，在oSIP中没有
 * fmt直接操作的方法，所以需要增加逻辑处理
 * 详细参见RFC4566.
 * @return fmt 字符串
 **/
std::string CSdpMedia::GetFmt()
{
    CHECK_RETURN_VAL_2(m_pMedia, m_pMedia->m_proto, "");
    std::string fmt("");
    std::string proto = m_pMedia->m_proto;

    // RTP/AVP
    int pos = 0;
    while (!osip_list_eol(&m_pMedia->m_payloads, pos))
    {
        const char *payload = (const char *) osip_list_get(&m_pMedia->m_payloads, pos);
        if (payload)
        {
            fmt += payload;
            fmt += STRING_SIP_SP;
        }
        ++pos;
    }

    return fmt;
}

/************************************************************************/
/* 列表拷贝方法                                                         */
/************************************************************************/
int CSdpMedia::ClonePayloadList(osip_list_t *li)
{
    int pos = 0;
    while (!osip_list_eol(li, pos))
    {
        char *payload = osip_strdup((char *) osip_list_get(li, pos));
        if (payload != NULL)
        {
            osip_list_add(&m_pMedia->m_payloads, (void *) payload, -1);
        }
        ++pos;
    }

    return 0;
}

int CSdpMedia::CloneConnectionList(osip_list_t *li)
{
    CHECK_RETURN_VAL(li, -1);
    int pos = 0;
    CSdpConnection connection;
    while (!osip_list_eol(li, pos))
    {
        sdp_connection_t *conn = 
            (sdp_connection_t *) osip_list_get(li, pos);
        if (conn != NULL)
        {
            connection.SetConnection(conn);
            osip_list_add(&m_pMedia->c_connections, (void *) connection.Detach(), -1);
        }
        ++pos;
    }

    return 0;
}

int CSdpMedia::CloneBandwidthList(osip_list_t *li)
{
    CHECK_RETURN_VAL(li, -1);
    int pos = 0;
    CSdpBandwidth bandwidth;
    while (!osip_list_eol(li, pos))
    {
        sdp_bandwidth_t *bw = (sdp_bandwidth_t *) osip_list_get(li, pos);
        if (bw != NULL)
        {
            bandwidth.SetBandwidth(bw);
            osip_list_add(&m_pMedia->b_bandwidths, (void *) bandwidth.Detach(), -1);
        }
        ++pos;
    }

    return 0;
}

int CSdpMedia::CloneAttributeList(osip_list_t *li)
{
    CHECK_RETURN_VAL(li, -1);
    int pos = 0;
    CSdpAttribute attribute;
    while (!osip_list_eol(li, pos))
    {
        sdp_attribute_t *attr = (sdp_attribute_t *) osip_list_get(li, pos);
        if (attr != NULL)
        {
            attribute.SetAttribute(attr);
            osip_list_add(&m_pMedia->a_attributes, (void *) attribute.Detach(), -1);
        }
        ++pos;
    }

    return 0;
}

int CSdpMedia::CloneKey(sdp_key_t *key)
{
    CHECK_RETURN_VAL_2(m_pMedia, key, -1);
    sdp_key_free(m_pMedia->k_key);
    m_pMedia->k_key->k_keytype = osip_strdup(key->k_keytype);
    m_pMedia->k_key->k_keydata = osip_strdup(key->k_keydata);
    return 0;
}

int CSdpMedia::GetRecvSendFlag()
{
    // 默认返回 eUnknown
    int nFlag = eUnknown;
    osip_list_iterator_t it;
    osip_list_get_first(&m_pMedia->a_attributes, &it);
    while (osip_list_iterator_has_elem(it))
    {
        sdp_attribute_t *attr = 
            (sdp_attribute_t *) (it.actual->element);
        const char *field = attr->a_att_field;
        if (field)
        {
            if (strncmp(field, "recvonly", 8) == 0)
            {
                nFlag = eRecvOnly;
                break;
            }
            else if (strncmp(field, "sendonly", 8) == 0)
            {
                nFlag = eSendOnly;
                break;
            }
            else if (strncmp(field, "sendrecv", 8) == 0)
            {
                nFlag = eSendRecv;
                break;
            }
        }
        osip_list_get_next(&it);
    }
    return nFlag;
}
//////////////////////////////////////////////////////////////////////////
CSdpOrigin::CSdpOrigin()
{
    m_Username = "TVT";
    m_SessionID = CSdpUtility::GenerateSessionID();
    m_SessionVersion = CSdpUtility::GenerateSessionID();
    m_NetType = "IN";
    m_AddrType = "IP4";
}

CSdpOrigin::CSdpOrigin(const CSdpOrigin &origin)
{
    if (this != &origin)
    {
        m_Username = origin.m_Username;
        m_SessionID = origin.m_SessionID;
        m_SessionVersion = origin.m_SessionVersion;
        m_NetType = origin.m_NetType;
        m_AddrType = origin.m_AddrType;
        m_Address = origin.m_Address;
    }
}

CSdpOrigin& CSdpOrigin::operator = (const CSdpOrigin &origin)
{
    if (this != &origin)
    {
        m_Username = origin.m_Username;
        m_SessionID = origin.m_SessionID;
        m_SessionVersion = origin.m_SessionVersion;
        m_NetType = origin.m_NetType;
        m_AddrType = origin.m_AddrType;
        m_Address = origin.m_Address;
    }
    return *this;
}

CSdpOrigin::~CSdpOrigin()
{
}

void CSdpOrigin::SetUsername(const char *username)
{
    CHECK_RETURN(username);
    m_Username = username;
}

void CSdpOrigin::SetSessionID(const char *sessionId)
{
    CHECK_RETURN(sessionId);
    m_SessionID = sessionId;
}

void CSdpOrigin::SetSessionVersion(const char *sessionVersion)
{
    CHECK_RETURN(sessionVersion);
    m_SessionVersion = sessionVersion;
}

void CSdpOrigin::SetNetType(const char *netType)
{
    CHECK_RETURN(netType);
    m_NetType = netType;
}

void CSdpOrigin::SetAddrType(const char *addrType)
{
    CHECK_RETURN(addrType);
    m_AddrType = addrType;
}

void CSdpOrigin::SetAddress(const char *address)
{
    CHECK_RETURN(address);
    m_Address = address;
}

std::string& CSdpOrigin::GetUsername()
{
    return m_Username;
}

std::string& CSdpOrigin::GetSessionID()
{
    return m_SessionID;
}

std::string& CSdpOrigin::GetSessionVersion()
{
    return m_SessionVersion;
}

std::string& CSdpOrigin::GetNetType()
{
    return m_NetType;
}

std::string& CSdpOrigin::GetAddrType()
{
    return m_AddrType;
}

std::string& CSdpOrigin::GetAddress()
{
    return m_Address;
}

//////////////////////////////////////////////////////////////////////////

CSdpMessage::CSdpMessage()
{
    sdp_message_init(&m_pSdp);
    m_pBuf = NULL;

    // v
    m_pSdp->v_version = osip_strdup("0");

    // o
    CSdpOrigin o;
    SetOrigin(o);

    // s
    m_pSdp->s_name = osip_strdup("TVT");

    // t
    CSdpTimeDescr t("0", "0");
    AddTimeDescr(t);

    __InitVisitor();
}

CSdpMessage::CSdpMessage(const char *buf)
{
    m_pSdp = NULL;
    m_pBuf = NULL;
    Parse(buf);
    __InitVisitor();
}

CSdpMessage::CSdpMessage(sdp_message_t *sdp)
{
    m_pSdp = sdp;
    m_pBuf = NULL;
    __InitVisitor();
}

CSdpMessage::CSdpMessage(const CSdpMessage &SdpMsg)
{
    if (this != &SdpMsg)
    {
        m_pSdp = NULL;
        m_pBuf = NULL;
        sdp_message_t *sdp = NULL;
        int ret = sdp_message_clone(SdpMsg.m_pSdp, &sdp);
        if (ret != 0)
        {
            //sdp_message_free(sdp);// sdp 消息clone失败osip自动释放
        }
        else
        {
            m_pSdp = sdp;
        }
        __InitVisitor();
    }
}

CSdpMessage& CSdpMessage::operator = (const CSdpMessage &SdpMsg)
{
    if (this != &SdpMsg)
    {
        m_pSdp = NULL;
        m_pBuf = NULL;
        sdp_message_t *sdp = NULL;
        int ret = sdp_message_clone(SdpMsg.m_pSdp, &sdp);
        if (ret != 0)
        {
            //sdp_message_free(sdp);// sdp 消息clone失败osip自动释放
        }
        else
        {
            m_pSdp = sdp;
        }

        // 赋值函数不需要调用__InitVisitor()
    }
    return *this;
}

CSdpMessage::~CSdpMessage()
{
	if(m_pBuf){
		osip_free(m_pBuf);
		m_pBuf = NULL;
	}

	if(m_pSdp){
		sdp_message_free(m_pSdp);
		m_pSdp = NULL;
	}

    __FreeVisitor();
}

void CSdpMessage::__InitVisitor()
{
    m_key.SetKey(static_cast<sdp_key_t *>(NULL));
    m_bandwidth.SetBandwidth(static_cast<sdp_bandwidth_t *>(NULL));
    m_timeDescr.SetTimeDescr(static_cast<sdp_time_descr_t *>(NULL));
    m_connection.SetConnection(static_cast<sdp_connection_t *>(NULL));
    m_attribute.SetAttribute(static_cast<sdp_attribute_t *>(NULL));
    m_media.SetMedia(static_cast<sdp_media_t *>(NULL));
}

void CSdpMessage::__FreeVisitor()
{
    m_key.Detach();
    m_bandwidth.Detach();
    m_timeDescr.Detach();
    m_connection.Detach();
    m_attribute.Detach();
    m_media.Detach();
}

int CSdpMessage::Parse(const char *buf)
{
    if (m_pSdp != NULL)
    {
        sdp_message_free(m_pSdp);
        m_pSdp = NULL;
    }
    sdp_message_init(&m_pSdp);
    CHECK_RETURN_VAL(m_pSdp, -1);

    return sdp_message_parse(m_pSdp, buf);
}

void CSdpMessage::Release()
{
    delete this;
}


const char* CSdpMessage::ToString()
{
    osip_free(m_pBuf);
    int ret = sdp_message_to_str(m_pSdp, &m_pBuf);
    if (ret != 0 || m_pBuf == NULL)
    {
        m_nLength = 0;
        return NULL;
    }
    m_nLength = strlen(m_pBuf);
    return m_pBuf;
}

int CSdpMessage::GetLength()
{
    return m_nLength;
}

CSdpMessage* CSdpMessage::Clone()
{
    sdp_message_t *sdp = NULL;
    int ret = sdp_message_clone(m_pSdp, &sdp);
    if (ret != 0)
    {
        // sdp 消息clone失败osip自动释放
        // sdp_message_free(sdp);
        return NULL;
    }
    return new CSdpMessage(sdp);
}

/************************************************************************/
/* v                                                                    */
/************************************************************************/
int CSdpMessage::SetVersion(const char *version)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->v_version);
    m_pSdp->v_version = osip_strdup(version);
    return 0;
}

const char* CSdpMessage::GetVersion()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->v_version;
}

/************************************************************************/
/* o                                                                    */
/************************************************************************/
int CSdpMessage::SetOrigin(CSdpOrigin &origin)
{
    return SetOrigin(origin.GetUsername().c_str(), 
                     origin.GetSessionID().c_str(), 
                     origin.GetSessionVersion().c_str(), 
                     origin.GetNetType().c_str(), 
                     origin.GetAddrType().c_str(), 
                     origin.GetAddress().c_str());
}

int CSdpMessage::SetOrigin(const char *username, 
                           const char *sessionId, 
                           const char *sessionVersion, 
                           const char *netType, 
                           const char *addrType, 
                           const char *addr)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_username);
    osip_free(m_pSdp->o_sess_id);
    osip_free(m_pSdp->o_sess_version);
    osip_free(m_pSdp->o_nettype);
    osip_free(m_pSdp->o_addrtype);
    osip_free(m_pSdp->o_addr);
    m_pSdp->o_username = osip_strdup(username);
    m_pSdp->o_sess_id = osip_strdup(sessionId);
    m_pSdp->o_sess_version = osip_strdup(sessionVersion);
    m_pSdp->o_nettype = osip_strdup(netType);
    m_pSdp->o_addrtype = osip_strdup(addrType);
    m_pSdp->o_addr = osip_strdup(addr);
    return 0;
}

int CSdpMessage::SetUsername(const char *username)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_username);
    m_pSdp->o_username = osip_strdup(username);
    return 0;
}

int CSdpMessage::SetSessionID(const char *sessId)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_sess_id);
    m_pSdp->o_sess_id = osip_strdup(sessId);
    return 0;
}

int CSdpMessage::SetSessionVersion(const char *sessVersion)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_sess_version);
    if (sessVersion)
    {
        m_pSdp->o_sess_version = osip_strdup(sessVersion);
    }
    else
    {
        m_pSdp->o_sess_version = 
            osip_strdup(CSdpUtility::GenerateSessionID().c_str());
    }
    return 0;
}

int CSdpMessage::SetNetType(const char *netType)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_nettype);
    m_pSdp->o_nettype = osip_strdup(netType);
    return 0;
}

int CSdpMessage::SetAddrType(const char *addrType)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_addrtype);
    m_pSdp->o_addrtype = osip_strdup(addrType);
    return 0;
}

int CSdpMessage::SetAddress(const char *addr)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->o_addr);
    m_pSdp->o_addr = osip_strdup(addr);
    return 0;
}

CSdpOrigin& CSdpMessage::GetOrigin()
{
    m_origin.SetUsername(m_pSdp->o_username);
    m_origin.SetSessionID(m_pSdp->o_sess_id);
    m_origin.SetSessionVersion(m_pSdp->o_sess_version);
    m_origin.SetNetType(m_pSdp->o_nettype);
    m_origin.SetAddrType(m_pSdp->o_addrtype);
    m_origin.SetAddress(m_pSdp->o_addr);
    return m_origin;
}

const char* CSdpMessage::GetUsername()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->o_username;
}

const char* CSdpMessage::GetSessionID()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->o_sess_id;
}

const char* CSdpMessage::GetSessionVersion()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->o_sess_version;
}

const char* CSdpMessage::GetNetType()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->o_nettype;
}

const char* CSdpMessage::GetAddrType()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->o_addrtype;
}

const char* CSdpMessage::GetAddress()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->o_addr;
}

/************************************************************************/
/* s                                                                    */
/************************************************************************/
int CSdpMessage::SetSessionName(const char *sessionName)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->s_name);
    m_pSdp->s_name = osip_strdup(sessionName);
    return 0;
}

const char* CSdpMessage::GetSessionName()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->s_name;
}

/************************************************************************/
/* i                                                                    */
/************************************************************************/
int CSdpMessage::SetSessionInfo(const char *sessionInfo)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->i_info);
    m_pSdp->i_info = osip_strdup(sessionInfo);
    return 0;
}

const char* CSdpMessage::GetSessionInfo()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->i_info;
}

/************************************************************************/
/* u                                                                    */
/************************************************************************/
int CSdpMessage::SetUri(const char *uri)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->u_uri);
    m_pSdp->u_uri = osip_strdup(uri);
    return 0;
}

const char* CSdpMessage::GetUri()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->u_uri;
}

/************************************************************************/
/* e                                                                    */
/************************************************************************/
int CSdpMessage::AddEmail(const char *email)
{
    CHECK_RETURN_VAL_2(m_pSdp, email, -1);
    osip_list_add(&m_pSdp->e_emails, osip_strdup(email), -1);
    return 0;
}

const char* CSdpMessage::GetEmail(int pos)
{
    return sdp_message_e_email_get(m_pSdp, pos);
}

/************************************************************************/
/* p                                                                    */
/************************************************************************/
int CSdpMessage::AddPhone(const char *phone)
{
    CHECK_RETURN_VAL_2(m_pSdp, phone, -1);
    osip_list_add(&m_pSdp->p_phones, osip_strdup(phone), -1);
    return 0;
}

const char* CSdpMessage::GetPhone(int pos)
{
    return sdp_message_p_phone_get(m_pSdp, pos);
}

/************************************************************************/
/* c                                                                    */
/************************************************************************/
int CSdpMessage::SetConnection(CSdpConnection connection)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_connection_free(m_pSdp->c_connection);
    m_pSdp->c_connection = connection.Detach();
    return 0;
}

CSdpConnection& CSdpMessage::GetConnection()
{
    m_connection.Detach();
    CHECK_RETURN_VAL(m_pSdp, m_connection);
    m_connection.Attach(m_pSdp->c_connection);
    return m_connection;
}

/************************************************************************/
/* b                                                                    */
/************************************************************************/
int CSdpMessage::AddBandwidth(const char *type, const char *bandwidth)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    CHECK_RETURN_VAL_2(type, bandwidth, -1);
    sdp_bandwidth_t *bw = NULL;
    sdp_bandwidth_init(&bw);
    bw->b_bandwidth = osip_strdup(bandwidth);
    bw->b_bwtype = osip_strdup(type);
    osip_list_add(&m_pSdp->b_bandwidths, (void *) bw, -1);
    return 0;
}

CSdpBandwidth& CSdpMessage::GetBandwidth(int pos)
{
    m_bandwidth.Detach();
    CHECK_RETURN_VAL(m_pSdp, m_bandwidth);
    sdp_bandwidth_t *bw = 
        (sdp_bandwidth_t *) osip_list_get(&m_pSdp->b_bandwidths, pos);
    m_bandwidth.Attach(bw);
    return m_bandwidth;
}

/************************************************************************/
/* t                                                                    */
/************************************************************************/
int CSdpMessage::SetTimeDescr(CSdpTimeDescr timeDescr, int pos /* = -1 */)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    if (pos < 0)
    {
        ClearTimeDescr();
    }
    else
    {
        RemoveTimeDescr(pos);
    }

    return AddTimeDescr(timeDescr);
}

CSdpTimeDescr& CSdpMessage::GetTimeDescr(int pos)
{
    m_timeDescr.Detach();
    CHECK_RETURN_VAL(m_pSdp, m_timeDescr);
    sdp_time_descr_t *td = 
        (sdp_time_descr_t *) osip_list_get(&m_pSdp->t_descrs, pos);
    m_timeDescr.Attach(td);
    return m_timeDescr;
}

int CSdpMessage::AddTimeDescr(CSdpTimeDescr timeDescr)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_time_descr_t *td = timeDescr.Detach();
    CHECK_RETURN_VAL(td, -1);
    osip_list_add(&m_pSdp->t_descrs, (void *) td, -1);
    return 0;
}

int CSdpMessage::RemoveTimeDescr(int pos)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_time_descr_t *t = 
        (sdp_time_descr_t *) osip_list_get(&m_pSdp->t_descrs, pos);
    if (t)
    {
        osip_list_remove(&m_pSdp->t_descrs, pos);
        sdp_time_descr_free(t);
        return 0;
    }
    return -1;
}

int CSdpMessage::ClearTimeDescr()
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_list_special_free(&m_pSdp->t_descrs, 
        (void (*)(void *)) &sdp_time_descr_free);
    return 0;
}

/************************************************************************/
/* z                                                                    */
/************************************************************************/
int CSdpMessage::SetZAdjustment(const char *zAdjustment)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_free(m_pSdp->z_adjustments);
    m_pSdp->z_adjustments = osip_strdup(zAdjustment);
    return 0;
}

const char* CSdpMessage::GetZAdjustment()
{
    CHECK_RETURN_VAL(m_pSdp, NULL);
    return m_pSdp->z_adjustments;
}

/************************************************************************/
/* k                                                                    */
/************************************************************************/
int CSdpMessage::SetKey(CSdpKey key)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_key_free(m_pSdp->k_key);
    m_pSdp->k_key = key.Detach();
    return 0;
}

CSdpKey& CSdpMessage::GetKey()
{
    m_key.Detach();
    CHECK_RETURN_VAL(m_pSdp, m_key);
    m_key.Attach(m_pSdp->k_key);
    return m_key;
}

/************************************************************************/
/* a                                                                    */
/************************************************************************/
int CSdpMessage::AddAttribute(const char *field)
{
    CHECK_RETURN_VAL(field, -1);
    CSdpAttribute attr(field);
    return AddAttribute(attr);
}

int CSdpMessage::AddAttribute(const char *field, const char *value)
{
    CHECK_RETURN_VAL_2(field, value, -1);
    CSdpAttribute attr(field, value);
    return AddAttribute(attr);
}

int CSdpMessage::AddAttribute(CSdpAttribute attribute)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_attribute_t *attr = attribute.Detach();
    CHECK_RETURN_VAL(attr, -1);
    osip_list_add(&m_pSdp->a_attributes, (void *) attr, -1);
    return 0;
}

CSdpAttribute& CSdpMessage::GetAttribute(int pos)
{
    m_attribute.Detach();
    CHECK_RETURN_VAL(m_pSdp, m_attribute);
    sdp_attribute_t *attr = 
        (sdp_attribute_t *) osip_list_get(&m_pSdp->a_attributes, pos);
    m_attribute.Attach(attr);
    return m_attribute;
}

int CSdpMessage::RemoveAttribute(int pos)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_attribute_t *attr = 
        (sdp_attribute_t *) osip_list_get(&m_pSdp->a_attributes, pos);
    if (attr)
    {
        osip_list_remove(&m_pSdp->a_attributes, pos);
        sdp_attribute_free(attr);
        return 0;
    }
    return -1;
}

int CSdpMessage::RemoveAttribute(const char *field)
{
    CHECK_RETURN_VAL_2(m_pSdp, field, -1);
    int pos = 0;
    while (!osip_list_eol(&m_pSdp->a_attributes, pos))
    {
        sdp_attribute_t *attr = (sdp_attribute_t *) osip_list_get(&m_pSdp->a_attributes, pos);
        if (attr && 0 == osip_strcasecmp(attr->a_att_field, field))
        {
            // 注意删除节点后，pos不移动
            osip_list_remove(&m_pSdp->a_attributes, pos);
            sdp_attribute_free(attr);
        }
        else
        {
            ++pos;
        }
    }
    return 0;
}

int CSdpMessage::ClearAttribute()
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_list_special_free(&m_pSdp->a_attributes, 
        (void (*)(void *)) &sdp_attribute_free);
    return 0;
}

/************************************************************************/
/* m                                                                    */
/************************************************************************/
// 若pos无效，则增加Media信息，否则替换原有的Media信息
int CSdpMessage::SetMedia(CSdpMedia media, int pos /* = 0 */)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    RemoveMedia(pos);
    sdp_media_t *m = media.Detach();
    CHECK_RETURN_VAL(m, -1);
    osip_list_add(&m_pSdp->m_medias, (void *) m, -1);
    return 0;
}

int CSdpMessage::AddMedia(CSdpMedia media)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_media_t *m = media.Detach();
    CHECK_RETURN_VAL(m, -1);
    osip_list_add(&m_pSdp->m_medias, (void *) m, -1);
    return 0;
}

CSdpMedia& CSdpMessage::GetMedia(int pos)
{
    m_media.Detach();
    CHECK_RETURN_VAL(m_pSdp, m_media);
    sdp_media_t *m = 
        (sdp_media_t *) osip_list_get(&m_pSdp->m_medias, pos);
    m_media.Attach(m);
    return m_media;
}

CSdpMedia& CSdpMessage::GetMedia(const char *type)
{
    m_media.Detach();


    CHECK_RETURN_VAL_2(m_pSdp, type, m_media);
    sdp_media_t *m = NULL;
    osip_list_iterator_t it;
    osip_list_get_first(&m_pSdp->m_medias, &it);
    while (osip_list_iterator_has_elem(it))
    {
        m = (sdp_media_t *) (it.actual->element);
        if (osip_strcasecmp(m->m_media, type) == 0)
        {
            break;
        }
        osip_list_get_next(&it);
        m = NULL;
    }
    m_media.Attach(m);
    return m_media;
}

int CSdpMessage::RemoveMedia(int pos)
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    sdp_media_t *media = 
        (sdp_media_t *) osip_list_get(&m_pSdp->m_medias, pos);
    if (media)
    {
        osip_list_remove(&m_pSdp->m_medias, pos);
        sdp_media_free(media);
        return 0;
    }
    return -1;
}

int CSdpMessage::RemoveMedia(const char *type)
{
    CHECK_RETURN_VAL_2(m_pSdp, type, 0);
    int pos = 0;
    while (!osip_list_eol(&m_pSdp->m_medias, pos))
    {
        sdp_media_t *media = (sdp_media_t *) osip_list_get(&m_pSdp->m_medias, pos);
        if (media && 0 == osip_strcasecmp(media->m_media, type))
        {
            osip_list_remove(&m_pSdp->m_medias, pos);
            sdp_media_free(media);
            return 0;
        }
        ++pos;
    }
    return 0;
}

int CSdpMessage::GetMediaCount()
{
    return osip_list_size(&m_pSdp->m_medias);
}

int CSdpMessage::ClearMedia()
{
    CHECK_RETURN_VAL(m_pSdp, -1);
    osip_list_special_free(&m_pSdp->m_medias, 
        (void (*)(void *)) &sdp_media_free);
    return 0;
}

/************************************************************************/
/* 其他方法                                                             */
/************************************************************************/
int CSdpMessage::GetRecvSendFlag()
{
    // 默认返回 eUnknown
    int nFlag = eUnknown;
    osip_list_iterator_t it;
    osip_list_get_first(&m_pSdp->a_attributes, &it);
    while (osip_list_iterator_has_elem(it))
    {
        sdp_attribute_t *attr = 
            (sdp_attribute_t *) (it.actual->element);
        const char *field = attr->a_att_field;
        if (field)
        {
            if (strncmp(field, "recvonly", 8) == 0)
            {
                nFlag = eRecvOnly;
                break;
            }
            else if (strncmp(field, "sendonly", 8) == 0)
            {
                nFlag = eSendOnly;
                break;
            }
            else if (strncmp(field, "sendrecv", 8) == 0)
            {
                nFlag = eSendRecv;
                break;
            }
        }
        osip_list_get_next(&it);
    }
    return nFlag;
}

//////////////////////////////////////////////////////////////////////////
CSdpUtility::CSdpUtility()
{
}

CSdpUtility::~CSdpUtility()
{
}

std::string CSdpUtility::GenerateSessionID()
{
#ifdef WIN32
	int sessionId = rand();
#else
	int sessionId = random();
#endif

	return Int2String(sessionId);
}
