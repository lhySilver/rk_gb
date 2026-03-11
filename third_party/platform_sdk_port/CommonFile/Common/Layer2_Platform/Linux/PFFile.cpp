
#include <stdio.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "base_type_define.h"
#include "PFFile.h"

CPFFile::CPFFile(void)
{
    m_FileHandle=-1;
    m_DelOnClose=false;
    m_bOpenForWrite = false;
}

CPFFile::~CPFFile(void)
{
}


bool CPFFile::FileExist(const char *pFile)
{
    //这种方法，目录也返回true
    //return access(pFile, 0) != -1;
    if (access(pFile, F_OK) != -1)
    {
        int ret;
        struct stat st;
        ret = stat(pFile, &st);//如果文件路径太长了这里会返回失败
        if (ret == 0)
        {
            bool bRet = (!(S_ISDIR(st.st_mode) || S_ISLNK(st.st_mode) || S_ISSOCK(st.st_mode)));
            return bRet;
        }
        ASSERT(false);
        return false;
    }
    else
    {
        return false;
    }
}

void CPFFile::deleteFile(const char *pFile)
{
    unlink(pFile);
}

bool CPFFile::SetAttributes(const char *szFileName, tuint32 dwFileAttributes)
{
	return false;
}

bool CPFFile::OpenFile(const char *pFilename, tuint32 shareAttr)
{
    strncpy(m_FileName,pFilename,sizeof(m_FileName)-1);
    tuint32 readWriteFlag=shareAttr & (OPEN_MODE_WRITE|OPEN_MODE_READ);
    if(readWriteFlag == (OPEN_MODE_WRITE|OPEN_MODE_READ) )
    {
        readWriteFlag = O_RDWR ;
        m_bOpenForWrite = true;
    }
    else if(readWriteFlag == OPEN_MODE_WRITE)
    {
        readWriteFlag =O_WRONLY;
        m_bOpenForWrite = true;
    }
    else if(readWriteFlag == OPEN_MODE_READ) readWriteFlag =O_RDONLY;
    else readWriteFlag =O_RDONLY;

    tuint32 creaeFlag=0;
    if(shareAttr & OPEN_CREATE_NEW ) creaeFlag=O_CREAT | O_EXCL;//创建文件，如果文件已存在就返回失败
    if(shareAttr & OPEN_CREATE_ALWAYS ) creaeFlag=O_TRUNC|O_CREAT;//如果没有就生成，打开时会清空原来文件内容
    if(shareAttr & OPEN_OPEN_EXISTING ) creaeFlag=0; //打开必须存在的文件，不存在就打开失败
    if(shareAttr & OPEN_OPEN_ALWAYS ) creaeFlag=O_CREAT ;//如果文件不存在就创建它
    if(shareAttr & OPEN_TRUNCATE_EXISTING ) creaeFlag=O_TRUNC;//打开存在的文件，且清空原来的内容

    tuint32 directFlag = O_LARGEFILE; //大文件支持
    if (shareAttr & OPEN_FLAG_DIRECT) directFlag |= O_DIRECT;

    if(shareAttr & OPEN_MODE_DELONCLOSE)
        m_DelOnClose=true;
    else
        m_DelOnClose=false;
    m_FileHandle=open(pFilename,readWriteFlag|creaeFlag|directFlag,0777);
    if(m_FileHandle == -1)
        return false;
    else
        return true;
}

bool CPFFile::IsOpened()
{
    if(m_FileHandle != -1)
        return true;
    else
        return false;
}

bool CPFFile::Write(const void *pBuf,tuint32 WriteBuflen)
{
    if(m_FileHandle == -1) return false;
    int writelen=write(m_FileHandle,pBuf,WriteBuflen);
    if(writelen == -1)
        return false;
    else
        return true;
}

bool CPFFile::Read(void *pBuf,tuint32 readBuflen,tuint32 &readbytes)
{
    if(m_FileHandle == -1) return false;
    readbytes=read(m_FileHandle,pBuf,readBuflen);
    if(readbytes == -1)
        return false;
    else
        return true;
}

tint64 CPFFile::GetSize()
{
    if(m_FileHandle == -1) return 0;
    off_t curpos=lseek(m_FileHandle,0,SEEK_CUR);
    off_t filesize=lseek(m_FileHandle,0,SEEK_END);
    lseek(m_FileHandle,curpos,SEEK_SET);
    return filesize;
}

tint64 CPFFile::Seek(tint64 lOff, SEEKMODE nFrom)
{
    if(m_FileHandle == -1) return -1;
    off_t curpos=0;
    if(nFrom == SEEKMODE_BEGIN) curpos=lseek(m_FileHandle,lOff,SEEK_SET);
    else if(nFrom == SEEKMODE_END) curpos=lseek(m_FileHandle,lOff,SEEK_END);
    else if(nFrom == SEEKMODE_CURRENT) curpos=lseek(m_FileHandle,lOff,SEEK_CUR);

    return curpos;
}

tint64 CPFFile::SeekToEnd()
{
    if(m_FileHandle == -1) return 0;
    off_t curpos=lseek(m_FileHandle,0,SEEK_END);
    return curpos;
}

void CPFFile::SeekToBegin()
{
    if(m_FileHandle == -1) return;
    lseek(m_FileHandle,0,SEEK_SET);
}

void CPFFile::CloseFile()
{
    if (m_bOpenForWrite && !m_DelOnClose)
    {
        fsync(m_FileHandle);
    }
    close(m_FileHandle);
    if(m_DelOnClose)
        unlink(m_FileName);
    m_FileHandle = -1;
}

bool CPFFile::SetFilePointer(tint64 distanceToMove, tuint64 *lpNewFilePointer, SEEKMODE startPointer)
{
    tint64 ret = Seek(distanceToMove, startPointer);
    if (lpNewFilePointer) *lpNewFilePointer = ret;
    return (ret != -1);
}

