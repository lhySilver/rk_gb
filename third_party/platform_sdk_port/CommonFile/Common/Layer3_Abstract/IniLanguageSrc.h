#ifndef INI_LANGUAGE_SRC_HEAD
#define INI_LANGUAGE_SRC_HEAD
#include<string>

namespace IniLanguageSrc
{
	std::string GetResourceString(const std::string& id);
}

#define GET_LANGUAGE_TXT(szID) IniLanguageSrc::GetResourceString(szID)

#endif //INI_LANGUAGE_SRC_HEAD
