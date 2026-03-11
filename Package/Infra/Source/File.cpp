#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
	#include <io.h>
	#include <direct.h>
	#include <Windows.h>
#elif defined(__GNUC__)
	#include <sys/statfs.h>
	#include <sys/statfs.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <dirent.h>
#endif
#include "Infra/File.h"

#ifdef WIN32
	#define fdopen _fdopen
#else
	#define _O_BINARY 0
	#define _O_RDONLY O_RDONLY
	#define _O_WRONLY O_WRONLY
	#define _O_CREAT O_CREAT
	#define _O_TRUNC O_TRUNC
	#define _O_RDWR O_RDWR
	#define _S_IREAD (S_IRUSR | S_IRGRP | S_IROTH)
	#define _S_IWRITE (S_IWUSR | S_IWGRP | S_IWOTH)
	#define _open open
	#define _access access
#endif

#include <string>

//////////////////////////////////////////////////////////////////////////////
///////// File find wrap

static int findnext(long handle, FileInfo* data);

static long findfirst(const char* fileName, FileInfo* data)
{
#ifdef WIN32
	_finddata_t finddata;
	long ret = (long)_findfirst(fileName, &finddata);
	if(ret < 0)
	{
		return ret;
	}
	data->attrib = finddata.attrib;
	data->size = finddata.size;
	memcpy(data->name, finddata.name, 107);
	data->name[sizeof(data->name) - 1] = '\0';
	data->time = finddata.time_write;
	return ret;
#else
	char buf[128];
	DIR *directory;

	strcpy(buf, fileName);

	int len = strlen(fileName);

	if (buf[len - 1] == '*')
	{
		buf[len - 1] = '\0';
	}
	if ((directory = opendir(buf)))
	{
		if (findnext(reinterpret_cast<long>(directory), data) < 0)
		{
			closedir(directory);
			directory = 0;
			return -1;
		}
		return reinterpret_cast<long>(directory);
	}
	return -1;
#endif
}

static int findnext(long handle, FileInfo* data)
{
#ifdef WIN32
	_finddata_t finddata;
	long ret = _findnext(handle, &finddata);
	if(ret < 0)
	{
		return ret;
	}
	data->attrib = finddata.attrib;
	data->size = finddata.size;
	memcpy(data->name, finddata.name, 107);
	data->name[sizeof(data->name) - 1] = '\0';
	data->time = finddata.time_write;
	return ret;
#else
	DIR *directory = reinterpret_cast<DIR *>(handle);

	if (directory)
	{
		struct dirent *dirent;

		if ((dirent = readdir(directory)))
		{
			struct stat fileStatus;
			char fullName[256];

			sprintf(fullName, "%s/%s", data->name, dirent->d_name);
			(void)stat(fullName, &fileStatus);
			strcpy(data->name, dirent->d_name);
			data->size = fileStatus.st_size;	
			data->time = fileStatus.st_mtime;
			data->attrib = 0;
			if (S_ISDIR(fileStatus.st_mode)) data->attrib |= CFile::directory;
			if (!(S_IWUSR & fileStatus.st_mode)) data->attrib |= CFile::readOnly;
			return 0;
		}
	}
	return -1;
#endif
}

static int findclose(long handle)
{
#ifdef WIN32
	return _findclose(handle);
#else
	DIR *directory = reinterpret_cast<DIR *>(handle);

	assert(handle == reinterpret_cast<long>(directory));
	if (directory)
	{
		closedir(directory);
		return 0;
	}
	return -1;
#endif
}

#ifndef WIN32
static int _mkdir( const char *dirname)
{
	return mkdir((char *)dirname, 777);
}

static int _rmdir( const char *dirname)
{
	return rmdir((char *)dirname);
}
#endif


static int _statfs(const char *path, uint64* userFreeBytes, uint64* totalBytes, uint64* totalFreeBytes)
{
#ifdef WIN32
	GetDiskFreeSpaceEx(path,
		(PULARGE_INTEGER)userFreeBytes,
		(PULARGE_INTEGER)totalBytes,
		(PULARGE_INTEGER)totalFreeBytes);
#elif defined(__GNUC__)
	struct statfs info;

	statfs(path, &info);
	*userFreeBytes = (uint64)(info.f_bsize) * info.f_bavail;
	*totalBytes = (uint64)(info.f_bsize) * info.f_blocks;
	*totalFreeBytes = (uint64)(info.f_bsize) * info.f_ffree;
#endif
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////   various io entries
static const int fsMaxOptNum = 32;
static char fsPaths[fsMaxOptNum][32] = {};
static FSOperations fsOpts[fsMaxOptNum] = 
{
	{
		fopen,
		fclose,
		fread,
		fwrite,
		fflush,
		fseek,
		ftell,
		fgets,
		fputs,
		_access,
		rename,
		remove,
		findfirst,
		findnext,
		findclose,
		_mkdir,
		_rmdir,
		_statfs,
	},
};

void hookFS(const char* path, const FSOperations* opts)
{
	int i;

	for(i = 1; i < fsMaxOptNum; i++)
	{
		if(opts)
		{
			if(strlen(fsPaths[i]) == 0)
			{
				strcpy(fsPaths[i], path);
				fsOpts[i] = *opts;
				break;
			}
		}
		else
		{
			if(strcmp(path, fsPaths[i]) == 0)
			{
				strcpy(fsPaths[i], "");
				break;
			}
		}
	}
}

static FSOperations* findOpts(const char* pFileName)
{
	int i;

	for(i = 1; i < fsMaxOptNum; i++)
	{
		size_t len = strlen(fsPaths[i]);
		if(len != 0 && strncmp(pFileName, fsPaths[i], len) == 0)
		{
			break;
		}
	}

	if(i == fsMaxOptNum)
	{
		i = 0;
	}

	return &fsOpts[i];
}

///////////////////////////////////////////////////////////////////////////////
///////   CFile implement
CFile::CFile()
{
	m_pFile = NULL;
	m_pBuffer = NULL;
	m_dwLength = 0;
	m_opt = &fsOpts[0];
}

CFile::~CFile()
{
	if(m_pFile)
	{
		Close();
	}
	if(m_pBuffer)
	{
		UnLoad();
	}
}

bool CFile::Open(const char* pFileName, uint dwFlags /* = modeReadWrite */)
{
	int oflag = _O_BINARY;
	const char* mode = "";

	strcpy(m_pFilePath, pFileName);
	m_opt = findOpts(pFileName);
	if(m_opt != &fsOpts[0])
	{
		switch (dwFlags & 0xf) 
		{
		case modeRead:
			mode = "rb";
			break;
		case modeWrite:
			if (dwFlags & modeNoTruncate)
			{
				mode = "ab";
			}
			else
			{
				mode = "wb";
			}
			break;
		case modeReadWrite:
			if(dwFlags & modeCreate)
			{
				if (dwFlags & modeNoTruncate)
				{
					mode = "ab+";
				}
				else
				{
					mode = "wb+";
				}
			}
			else
			{
				mode = "rb+";
			}
		}
		m_pFile = m_opt->fopen(pFileName, mode);

		if(!m_pFile)
		{
			return false;
		}

		return true;
	}

	switch (dwFlags & 0xf) 
	{
	case modeRead:
		oflag |= _O_RDONLY;
		break;
	case modeWrite:
		oflag |= _O_WRONLY;
		break;
	case modeReadWrite:
		oflag |= _O_RDWR;
		break;
	default:
		break;
	}

	if(dwFlags & modeCreate)
	{
		oflag |= _O_CREAT;
		if(!(dwFlags & modeNoTruncate))
		{
			oflag |= _O_TRUNC;
		}
	}

	if(dwFlags & osNoBuffer)
	{
#ifdef LINUX
		/*
		 * With the O_DIRECT patch the kernel will do DMA directly from/to the physical memory pointed 
		 * by the userspace buffer passed as parameter to the read/write syscalls. So there will be no CPU 
		 * and mem bandwidth spent in the copies between userspace memory and kernel cache, and there will
		 * be no CPU time spent in kernel in the management of the cache (like cache lookups, per-page locks etc..).
		 */
		oflag |= O_DIRECT;
#endif
	}

	int fd = ::_open(pFileName, oflag, _S_IREAD | _S_IWRITE);

	if(fd == -1)
	{
		m_pFile = NULL;
		return false;
	}

	switch (dwFlags & 0xf) 
	{
	case modeRead:
		mode = "rb";
		break;
	case modeWrite:
		if (dwFlags & modeNoTruncate)
		{
			mode = "ab";
		}
		else
		{
			mode = "wb";
		}
		break;
	case modeReadWrite:
		if(dwFlags & modeCreate)
		{
			if (dwFlags & modeNoTruncate)
			{
				mode = "ab+";
			}
			else
			{
				mode = "wb+";
			}
		}
		else
		{
			mode = "rb+";
		}
	}

	m_pFile = fdopen(fd, mode);

	if(!m_pFile)
	{
		return false;
	}

	return true;
}

bool CFile::IsOpened()
{
	return m_pFile != 0;
}

const char *CFile::GetFilePath() const
{
	return m_pFilePath;
}


void CFile::Close()
{
	if(!m_pFile)
	{
		return;
	}

	m_opt->fclose(m_pFile);

	m_pFile = NULL;
}

unsigned char * CFile::Load(const char* pFileName)
{
	uint ret;

	assert(!m_pBuffer);
	if(Open(pFileName, modeRead))
	{
		m_dwLength = GetLength();
		if(m_dwLength)
		{
			m_pBuffer = new unsigned char[m_dwLength];
			if(m_pBuffer)
			{
				Seek(0, begin);
				ret = Read(m_pBuffer, m_dwLength);
				assert(ret == m_dwLength);
			}
		}
		Close();
	}
	return m_pBuffer;
}

void CFile::UnLoad()
{
	if(m_pBuffer)
	{
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}
}

uint CFile::Read(void *pBuffer, uint dwCount)
{
	if(!m_pFile)
	{
		return 0;
	}

	return m_opt->fread(pBuffer, 1, dwCount, m_pFile);
}

uint CFile::Write(const void *pBuffer, uint dwCount)
{
	if(!m_pFile)
	{
		return 0;
	}

	return m_opt->fwrite(pBuffer, 1, dwCount, m_pFile);
}

void CFile::Flush()
{
	if(!m_pFile)
	{
		return;
	}

	m_opt->fflush(m_pFile);
}

uint CFile::Seek(long lOff, uint nFrom)
{
	if(!m_pFile)
	{
		return 0;
	}
	int origin = 0;
	switch(nFrom){
	case begin:
		origin = SEEK_SET;
		break;
	case current:
		origin = SEEK_CUR;
		break;
	case end:
		origin = SEEK_END;
		break;
	}

	if(!m_opt->fseek(m_pFile, lOff, origin)){
		return GetPosition();
	};
	return 0;
}

uint CFile::GetPosition()
{
	if(!m_pFile)
	{
		return 0;
	}

	int pos = m_opt->ftell(m_pFile);
	if(pos >= 0)
	{
		return pos;
	}
	return 0;
}

uint CFile::GetLength()
{
	if(!m_pBuffer)//非Load方式装载
	{
		uint oldpos = GetPosition();
		if(Seek(0, end))
		{
			m_dwLength = GetPosition();
		};
		Seek(oldpos, begin);
	}

	return m_dwLength;
}

char * CFile::Gets(char *s, int size)
{
	if(!m_pFile)
	{
		return 0;
	}

	return m_opt->fgets(s, size, m_pFile);
}

int CFile::Puts(const char *s)
{
	if(!m_pFile)
	{
		return 0;
	}

	return m_opt->fputs(s, m_pFile);
}

bool CFile::Rename(const char* oldName, const char* newName)
{
	FSOperations* opt = findOpts(oldName);

	return (opt->rename(oldName, newName) == 0);
}

bool CFile::Remove(const char* fileName)
{
	FSOperations* opt = findOpts(fileName);

	return (opt->remove(fileName) == 0);
}

bool CFile::MakeDirectory(const char* dirName)
{
	FSOperations* opt = findOpts(dirName);

	return(opt->mkdir(dirName) == 0);
}

bool CFile::RemoveDirectory(const char* dirName)
{
	FSOperations* opt = findOpts(dirName);

	return (opt->rmdir(dirName) == 0);
}

bool CFile::Access(const char *filename, AccessMode mode)
{
	FSOperations* opt = findOpts(filename);

	return (opt->access(filename, mode) == 0);
}

bool CFile::StatFS(const char* path, uint64* userFreeBytes, uint64* totalBytes, uint64* totalFreeBytes)
{
	FSOperations* opt = findOpts(path);

	return (opt->statfs(path, userFreeBytes, totalBytes, totalFreeBytes) == 0);
}

///////////////////////////////////////////////////////////////////////////////
///////   CFileFind implement
CFileFind::CFileFind()
{
	m_opt = &fsOpts[0];
}

CFileFind::~CFileFind()
{

}

bool CFileFind::findFile(const char* fileName)
{
	const char* p = fileName + strlen(fileName);
	while(*p != '/' && p != fileName)
	{
		p--;
	}
	m_path.assign(fileName, p - fileName + 1);

	m_opt = findOpts(fileName);
	strcpy(m_fileInfo.name, m_path.c_str());
	return ((m_handle = m_opt->findfirst(fileName, &m_fileInfo)) != -1);
}

bool CFileFind::findNextFile()
{
	strcpy(m_fileInfo.name, m_path.c_str());
	return (m_opt->findnext(m_handle, &m_fileInfo) != -1);
}

void CFileFind::close()
{
	m_opt->findclose(m_handle);
}

uint64 CFileFind::getLength()
{
	return m_fileInfo.size;
}

std::string CFileFind::getFileName()
{
	return m_fileInfo.name;
}

std::string CFileFind::getFilePath()
{
	return m_path + m_fileInfo.name;
}

bool CFileFind::isReadOnly()
{
	return ((m_fileInfo.attrib & CFile::readOnly) != 0);
}

bool CFileFind::isDirectory()
{
	return ((m_fileInfo.attrib & CFile::directory) != 0);
}

bool CFileFind::isHidden()
{
	return ((m_fileInfo.attrib & CFile::hidden) != 0);
}

bool CFileFind::isNormal()
{
	return (m_fileInfo.attrib == CFile::normal);
}
