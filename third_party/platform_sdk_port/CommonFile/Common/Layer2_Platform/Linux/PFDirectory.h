
#ifndef __PF_DIRECTORY__
#define __PF_DIRECTORY__

class CPFDirectory
{
public:
	/////构造
	CPFDirectory(void);
	////析构
	~CPFDirectory(void);	

    static bool DirectoryExist(const char *pFile);
    static bool CreateDirectory(const char *pPathName);
    static bool RemoveDirectory(const char *pPathName);
    static bool RemoveDirectoryEx(const char *pPathName);   //递归删除文件目录

    static bool RenameDirectory(const char* pFile, const char* pNewFile);
};

#endif
