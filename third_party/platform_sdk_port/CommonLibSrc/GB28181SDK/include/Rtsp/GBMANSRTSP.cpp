#include "GBMANSRTSP.h"
#include "GBTime.h"
#include <sstream>
#include "Interlocked.h"

using namespace std;

const std::string RTSP_SP = " ";
const std::string RTSP_SPLIT = ": ";
const std::string RTSP_CRLF = "\r\n";
const std::string RTSP_CSEQ = "CSeq";
const std::string RTSP_SCALE = "Scale";
const std::string RTSP_RANGE = "Range";

const std::string RTSP_PLAY = "PLAY";
const std::string RTSP_PAUSE = "PAUSE";
const std::string RTSP_TEARDOWN = "TEARDOWN";


Interlocked g_seq;

int GenSeqNum()
{
    return g_seq.Increment();
}


CGBMANSRTSPReq::CGBMANSRTSPReq(void)
{
    m_version = "RTSP/1.0";
}

CGBMANSRTSPReq::~CGBMANSRTSPReq(void)
{
}

bool CGBMANSRTSPReq::parseFromStr( char* buf, int len )
{
    //size_t size = 0;
    const char *beg = buf;
    const char *end = beg + len - 1;
    const char *next = beg;
    int nRet = true;

    if (0 == strncmp(buf, "RTSP/", 5))
        return false;
    else
        nRet = ParseRequestLine(beg, next);

    //size = next - buf;
    if (1 != nRet)
        return false;

    beg = next;
    while (beg < end)
    {
        // find the beginning of body
        if(*beg =='\r'&& *(beg+1) =='\n')
        {
            ParseBody(beg,next);
        }
        else
        {
            nRet = ParseHeaders(beg, next);
            if (nRet<0)
                break;
        }

        beg = next;
    }

    return true;
}

std::string CGBMANSRTSPReq::ToStr()
{
    std::ostringstream ostr;

    ostr << m_method << RTSP_SP	<< m_version << RTSP_CRLF;


    if (GetFieldValue(RTSP_CSEQ).size()==0)
    {
        int nSeq = GenSeqNum();
        ostr << RTSP_CSEQ << RTSP_SPLIT<< nSeq << RTSP_CRLF;
    }

    // Headers
    std::list<RTSP_HEADER>::iterator it = m_HeaderList.begin();
    for (;it!=m_HeaderList.end();++it)
    {
        ostr << it->strField << RTSP_SPLIT<< it->strValue << RTSP_CRLF;
    }

    if (m_strBody.size()!=0)
    {
        ostr << RTSP_CRLF;

        ostr << m_strBody << RTSP_CRLF;
    }

    return ostr.str();
}

int CGBMANSRTSPReq::PushHeader( std::string strField, std::string strValue )
{
    RTSP_HEADER header;
    header.strField = strField;
    header.strValue = strValue;
    m_HeaderList.push_back(header);
    return 0;
}

int CGBMANSRTSPReq::ClearHeaders()
{
    int nSize = m_HeaderList.size();
    m_HeaderList.clear();

    return nSize;
}

std::string CGBMANSRTSPReq::GetFieldValue( const std::string& strField )
{
    std::list<RTSP_HEADER>::iterator it = m_HeaderList.begin();
    for (;it!=m_HeaderList.end();++it)
    {
        if(strField==it->strField)
        {
            return it->strValue;
        }
    }

    return "";
}

std::string CGBMANSRTSPReq::GetRangeStr()
{
    return GetFieldValue(RTSP_RANGE);
}

std::string CGBMANSRTSPReq::GetScaleStr()
{
    return GetFieldValue(RTSP_SCALE);
}

// Request-Line: Method SP Request-URI SP RTSP-Version CRLF
int CGBMANSRTSPReq::ParseRequestLine(const char *buf, const char *&next)
{
    next = buf;
    const char *beg = buf;
    const char *end = buf;

    end = strchr (beg, ' ');
    if (NULL == end)
    {
        return -1;
    }
    m_method = std::string(beg, end - beg);

    beg = ++end;
    while ((*end != '\r') && (*end != '\n'))
    {
        if (*end)
        {
            ++end;
        }
        else
        {
            return 0;
        }
    }
    m_version = std::string(beg, end - beg);

    while (++end && (*end == '\r' || *end == '\n'));
    next = end;
    return 1;
}

int CGBMANSRTSPReq::ParseHeaders(const char *buf, const char *&next)
{
    next = buf;

    char *beg   = (char *) buf;
    char *end   = beg;
    char *field = NULL;
    char *value = NULL;
    char *ptr   = NULL;

    for ( ; ; )
    {
        if (*beg == '\0')
        {
            return false;
        }

        if (*beg == '\r' || *beg == '\n')
            break;

        // find the end of line.
        end = beg;
        while ((*end != '\r') && (*end != '\n'))
        {
            if (*end == '\0')
            {
                return false;
            }
            ++end;
        }

        // find header field.
        field = beg;
        ptr = strchr(field, ':');
        if (NULL == ptr)
        {
            return false;
        }
        // set end for header field string.
        std::string strField(field, ptr - beg);

        // find header value.
        value = ++ptr;
        // set end for header value string.
        std::string strValue(value, end - value);

        // set header field.
        PushHeader(strField,strValue);

        // find the beginning of next header line.
        beg = ++end;
        if ((*beg == '\r') || (*beg == '\n'))
            ++beg;
        // move next pointer.
        next = beg;
        if (*next == '\0')
        {
            // parse done
            return true;
        }
    }

    return true;
}

int CGBMANSRTSPReq::ParseBody( const char *buf, const char *&next )
{
    char *beg   = (char *)(buf+2);
    char *end   = beg;
    next = beg;

    while ((*end != '\r') && (*end != '\n'))
    {
        if (*end)
        {
            ++end;
        }
        else
        {
            return false;
        }
    }
    m_strBody = std::string(beg,end-beg);
    next = end;

    while ((*next == '\r') || (*next == '\n'))
        ++next;

    return 0;
}

std::string CGBMANSRTSPReq::GenRspStr( int nStatusCode, char* pReason /*= NULL*/ )
{
    std::ostringstream ostr;

    std::string strReason = pReason==NULL?"":pReason;

    ostr << m_version << RTSP_SP << nStatusCode << RTSP_SP << strReason << RTSP_CRLF;

    ostr << RTSP_CSEQ << RTSP_SPLIT<< GetFieldValue(RTSP_CSEQ) << RTSP_CRLF;

    return ostr.str();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGBMANSRTSPRsp::CGBMANSRTSPRsp( void )
{
    m_nStatusCode = 200;
}

CGBMANSRTSPRsp::~CGBMANSRTSPRsp( void )
{

}

bool CGBMANSRTSPRsp::parseFromStr( char* buf, int len )
{
   // size_t size = 0;
    const char *beg = buf;
    //const char *end = beg + len - 1;
    const char *next = beg;
    int nRet = true;

    if (0 != strncmp(buf, "RTSP/", 5))
        return false;
    else
        nRet = ParseStatusLine(beg, next);

    //size = next - buf;
    if (1 != nRet)
        return false;

    return true;
}

// Status-Code: RTSP-Version SP Status-Code SP Reason-Phrase CRLF
int CGBMANSRTSPRsp::ParseStatusLine(const char *buf, const char *&next)
{
    next = buf;
    const char *beg = buf;
    const char *end = buf;

    end = strchr (beg, ' ');
    if (NULL == end)
    {
        return -1;
    }
    m_version = std::string(beg, end - beg);

    beg = ++end;
    end = strchr(beg, ' ');
    if (NULL == end)
    {
        return -1;
    }
    int status;
    if (1 != sscanf(beg, "%d", &status))
    {
        return -1;
    }
    m_nStatusCode = status;

    beg = ++end;
    while ((*end != '\r') && (*end != '\n'))
    {
        if (*end)
        {
            ++end;
        }
        else
        {
            return -1;
        }
    }

    m_strReason = std::string(beg, end - beg);

    while (++end && (*end == '\r' || *end == '\n'));
    next = end;
    return true;
}

std::string CGBMANSRTSPReq::GetResponseMessage(int status,const std::string& description, const std::string& cseq)
{
    std::string reponse ="RTSP/1.0 %d %s\r\nCSeq: %s\r\nDate: %s\r\n";
    char buffer[256];
    memset(buffer,0,256);
    CGBTime time;
    sprintf(buffer,reponse.c_str(),status,description.c_str(), cseq.c_str(), time.GetStr().c_str());
    return string(buffer);
}

std::string CGBMANSRTSPReq::GetCseq()
{
    return GetFieldValue(RTSP_CSEQ);
}

