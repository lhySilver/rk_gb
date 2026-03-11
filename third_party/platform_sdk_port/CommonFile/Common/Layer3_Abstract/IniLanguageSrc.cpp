#include "IniLanguageSrc.h"
#include "inifile.h"
#include "PFHelper.h"

using namespace inifile;
namespace IniLanguageSrc
{
    class Internal
    {
    public:
        Internal()
        {
			languagesTextMap.clear();
			languagesTextEnMap.clear();
			char szPath[512]={0};
			if (CPFHelper::GetModuleFileName(NULL, szPath, 512) == 0)
			{
				return ;
			}

#ifdef WIN32
			char *pPos = strrchr(szPath,'\\');
			szPath[pPos-szPath] = '\0';
			strcat(szPath,"\\language\\");
#else
			char *pPos = strrchr(szPath,'/');
			szPath[pPos-szPath] = '\0';
			strcat(szPath,"/language/"); //linux的语言文件夹L小写
#endif

			std::string strlangindexfile;
			char bufLan[512]={0};
			sprintf(bufLan,"%sLanguageIndex.ini", szPath);
			strlangindexfile = bufLan;

			//ini的文件为是utf8格式时，读取到的第一行前三个字符是乱码,并且string util.cpp中trim函数必须是iswspace
			//ini的文件为是ansii格式时，读取正常，但是发送给客户端页面就不是utf8格式,所以暂时是在Chinese (PRC).ini的首行增加了[test]
			inifile::IniFile ini;
			if (ini.open(strlangindexfile) !=0)
			{
				return ;// true;
			}
			int ret = 0;
			string strLangsel;
			string strComment;
			std::string strltype="0x0409";
			ret = ini.getValue("info","languagesSel",strLangsel);
			if (ret == inifile::RET_OK)
			{
				strltype = strLangsel;
			}
			else
			{
				strltype="0x0409";
			}

			std::map<std::string, std::string>languagesmap;
			ret = ini.getValuses("languages",languagesmap);
			std::map<std::string, std::string>::iterator itlang=languagesmap.find(strltype);
			if ( itlang == languagesmap.end())
			{
				return ;// true;
			}
			string strlanfilename = itlang->second;

			std::string strlangfullfile;
			char bufLangFullFile[512]={0};
			char bufLangFullFile_En[512]={0};
			sprintf(bufLangFullFile,"%s%s.ini", szPath, strlanfilename.c_str());
			sprintf(bufLangFullFile_En,"%sEnglish (United States).ini", szPath);
			strlangfullfile = bufLangFullFile;

			inifile::IniFile inilang;
			if (inilang.open(strlangfullfile) !=0)
			{
				return ;// true;
			}
			
			inilang.getValuses("text",languagesTextMap);
			
			//找不到语言是默认从英文查找
			inifile::IniFile inilang_En;
			if (inilang_En.open(bufLangFullFile_En) ==0)
			{
				ret = inilang_En.getValuses("text",languagesTextEnMap);
					
			}
			return ;// true;
			}

        ~Internal()
        {
            languagesTextMap.clear();
			languagesTextEnMap.clear();
        }
		std::string GetString (const std::string& id)
		{
			if (!id.empty())
			{
				std::map<std::string, std::string>::iterator ittext = languagesTextMap.find(id);
				if ( ittext != languagesTextMap.end())
				{
					return ittext->second;
				}
			}
			if (!id.empty())
			{
				std::map<std::string, std::string>::iterator ittext = languagesTextEnMap.find(id);
				if ( ittext != languagesTextEnMap.end())
				{
					return ittext->second;
				}
			}
			return "";

		}
        std::map<std::string, std::string> languagesTextMap;
		std::map<std::string, std::string> languagesTextEnMap;
    
    };

    Internal& GetInternal()
    {
        static Internal internal;
        return internal;
    }    


    std::string GetResourceString(const std::string& id)
    {		
        return GetInternal().GetString (id);
    }

}
