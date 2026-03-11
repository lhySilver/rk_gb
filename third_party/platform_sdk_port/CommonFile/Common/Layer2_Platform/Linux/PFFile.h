
#ifndef __PF_FILE__
#define __PF_FILE__

class CPFFile
{
public:
    /////构造
    CPFFile(void);
    ////析构
    ~CPFFile(void);
    enum OPEN_MODE
    {
        OPEN_MODE_READ=0x01,
        OPEN_MODE_WRITE=0x02,

        OPEN_SHARE_READ=0x04,
        OPEN_SHARE_WRITE=0x08,
        OPEN_SHARE_DELETE=0x10,

        OPEN_CREATE_NEW=0x20,
        OPEN_CREATE_ALWAYS=0x40,
        OPEN_OPEN_EXISTING=0x80,
        OPEN_OPEN_ALWAYS=0x100,
        OPEN_TRUNCATE_EXISTING=0x200,

        OPEN_MODE_DELONCLOSE=0x400,

        OPEN_FLAG_DIRECT = 0x800,  //直通模式，不通过读写缓存
    };

    enum SEEKMODE
    {
        SEEKMODE_BEGIN = 0,
        SEEKMODE_CURRENT = 1,
        SEEKMODE_END = 2,
    };

	enum ATTRIBUTES
	{
		ATTRIBUTE_HIDDEN = 0x2,
		ATTRIBUTE_SYSTEM = 0x4,
		ATTRIBUTE_NORMAL = 0x80,
	};

    static bool FileExist(const char *pFile);
    static void deleteFile(const char *pFile);
	static bool SetAttributes(const char *szFileName, tuint32 dwFileAttributes);

    bool OpenFile(const char *pFilename, tuint32 shareAttr);
    bool IsOpened();
    bool Write(const void *pBuf,tuint32 WriteBuflen);
    bool Read(void *pBuf,tuint32 readBuflen,tuint32 &readbytes);
    tint64 Seek(tint64 lOff, SEEKMODE nFrom);
    tint64 GetSize();
    tint64 SeekToEnd();
    void SeekToBegin();
    void CloseFile();
    bool SetFilePointer(tint64 distanceToMove, tuint64 *lpNewFilePointer, SEEKMODE startPointer);
private:
    int		m_FileHandle;
    bool	m_DelOnClose;
    char	m_FileName[256];
    bool    m_bOpenForWrite;  //如果是可写打开的，那么Close时需要调用fsync
};

#endif
