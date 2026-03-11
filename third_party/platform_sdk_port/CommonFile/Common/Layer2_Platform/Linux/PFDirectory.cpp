
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "base_type_define.h"
#include "PFDirectory.h"
#include "PFFile.h"
#include "PFHelper.h"
#include <string>

CPFDirectory::CPFDirectory(void)
{
}

CPFDirectory::~CPFDirectory(void)
{
}


bool CPFDirectory::DirectoryExist(const char *pDir)
{
    //这种方法，文件也返回true
    //return access(pDir, 0) != -1;
    if (access(pDir, 0) != -1)
    {
        int ret;
        struct stat st;
        ret = stat(pDir, &st);
        if (ret == 0)
        {
            bool bRet = S_ISDIR(st.st_mode);
            return bRet;
        }
       // ASSERT(false);
        return false;
    }
    else
    {
        return false;
    }
}

bool CPFDirectory::CreateDirectory(const char *pPathName)
{
    int ret=mkdir(pPathName, 0777);
	return (ret == 0);
}

bool CPFDirectory::RemoveDirectory(const char *pPathName)
{
    int ret=rmdir(pPathName);
    return (ret == 0);
}

bool CPFDirectory::RemoveDirectoryEx(const char *pPathName)
{
    std::string strTemp = pPathName;
    strTemp += CPFHelper::DirectorySeparator;
    std::list<std::string> fileList;
    CPFHelper::GetFileList(strTemp.c_str(), fileList);
    if (fileList.size() <= 0)
    {
        return RemoveDirectory(pPathName);
    }
    std::string strCurDir = std::string(pPathName) + CPFHelper::DirectorySeparator;
    std::list<std::string>::const_iterator iterItem = fileList.begin(), _endItem = fileList.end();
    for (; iterItem != _endItem; ++ iterItem)
    {
        strTemp = strCurDir + *iterItem;
        struct stat statinfo;
        if (0 != ::lstat(strTemp.c_str(), &statinfo))
        {
            CPFFile::deleteFile(strTemp.c_str());
        }
        else if (!S_ISDIR(statinfo.st_mode))
        {
            CPFFile::deleteFile(strTemp.c_str());
        }
        else
        {
            RemoveDirectoryEx(strTemp.c_str());
        }
    }
    return RemoveDirectory(pPathName);
}

bool CPFDirectory::RenameDirectory(const char* pFile, const char* pNewFile)
{
    if(!DirectoryExist(pFile))
        return false;

    return rename(pFile, pNewFile) == 0;
}
