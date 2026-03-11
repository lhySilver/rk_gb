#include "SysHelper.h"

#include <cstdio>
#include <cstring>
#include <random>
#include <string>

namespace {

bool ParseGuidString(const char* guid_text, GUID& guid)
{
    if (guid_text == NULL)
    {
        return false;
    }

    std::string text(guid_text);
    if (text.empty())
    {
        return false;
    }

    if (text.front() != '{')
    {
        text.insert(text.begin(), '{');
    }
    if (text.back() != '}')
    {
        text.push_back('}');
    }

    unsigned int data1 = 0;
    unsigned int data2 = 0;
    unsigned int data3 = 0;
    unsigned int data4[8] = {0};
    const int matched = std::sscanf(
        text.c_str(),
        "{%8X-%4X-%4X-%2X%2X-%2X%2X%2X%2X%2X%2X}",
        &data1,
        &data2,
        &data3,
        &data4[0],
        &data4[1],
        &data4[2],
        &data4[3],
        &data4[4],
        &data4[5],
        &data4[6],
        &data4[7]);
    if (matched != 11)
    {
        return false;
    }

    guid = GUID_NULL;
    guid.Data1 = data1;
    guid.Data2 = static_cast<unsigned short>(data2);
    guid.Data3 = static_cast<unsigned short>(data3);
    for (int index = 0; index < 8; ++index)
    {
        guid.Data4[index] = static_cast<unsigned char>(data4[index]);
    }
    return true;
}

}  // namespace

bool CSysHelper::CreateGUID(GUID& guid)
{
    std::random_device random_device;
    for (int index = 0; index < 8; ++index)
    {
        guid.Data4[index] = static_cast<unsigned char>(random_device());
    }
    guid.Data1 = (static_cast<unsigned int>(random_device()) << 16) ^ static_cast<unsigned int>(random_device());
    guid.Data2 = static_cast<unsigned short>(random_device());
    guid.Data3 = static_cast<unsigned short>(random_device());

    guid.Data3 = static_cast<unsigned short>((guid.Data3 & 0x0FFFu) | 0x4000u);
    guid.Data4[0] = static_cast<unsigned char>((guid.Data4[0] & 0x3Fu) | 0x80u);
    return true;
}

GUID CSysHelper::GetGuidFromString(const char* pGUIDStr)
{
    GUID guid = GUID_NULL;
    if (!ParseGuidString(pGUIDStr, guid))
    {
        ASSERT(false);
        return GUID_NULL;
    }
    return guid;
}

bool CSysHelper::GuidToString(const GUID& guid, char* pGUIDStr)
{
    if (pGUIDStr == NULL)
    {
        ASSERT(false);
        return false;
    }

    std::snprintf(
        pGUIDStr,
        40,
        "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        guid.Data1,
        guid.Data2,
        guid.Data3,
        guid.Data4[0],
        guid.Data4[1],
        guid.Data4[2],
        guid.Data4[3],
        guid.Data4[4],
        guid.Data4[5],
        guid.Data4[6],
        guid.Data4[7]);
    return true;
}

std::string CSysHelper::GuidToString(const GUID& guid)
{
    char guid_text[40] = {0};
    GuidToString(guid, guid_text);
    return std::string(guid_text);
}

