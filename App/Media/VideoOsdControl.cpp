#include "VideoOsdControl.h"

#include <stdio.h>
#include <string.h>

#include "PAL/Capture.h"


#include "ExchangeAL/CameraExchange.h"
#include "ExchangeAL/Exchange.h"
#include "ExchangeAL/MediaExchange.h"
#include "ExchangeAL/ExchangeKind.h"
#include "Manager/ConfigManager.h"


namespace
{

static const int kVideoOsdDateTimeId = 1;
static const int kVideoOsdCustomTextId = 2;
static int g_cached_master_switch = -1;
static int g_cached_event_switch = -1;
static int g_cached_alert_switch = -1;
static bool g_has_cached_protocol_state = false;
static media::VideoOsdState g_cached_protocol_state;

static std::string ToLowerCopy(const std::string& text)
{
    std::string out = text;
    for (size_t i = 0; i < out.size(); ++i) {
        if (out[i] >= 'A' && out[i] <= 'Z') {
            out[i] = static_cast<char>(out[i] - 'A' + 'a');
        }
    }
    return out;
}

static std::string TrimWhitespaceCopy(const std::string& text)
{
    size_t begin = 0;
    while (begin < text.size() &&
           (text[begin] == ' ' || text[begin] == '\t' || text[begin] == '\r' || text[begin] == '\n')) {
        ++begin;
    }

    size_t end = text.size();
    while (end > begin &&
           (text[end - 1] == ' ' || text[end - 1] == '\t' || text[end - 1] == '\r' || text[end - 1] == '\n')) {
        --end;
    }

    return text.substr(begin, end - begin);
}

static bool ContainsToken(const std::string& text, const char* token)
{
    return token != NULL && text.find(token) != std::string::npos;
}

static std::string NormalizeVideoOsdTextTemplate(const std::string& textIn)
{
    const std::string text = TrimWhitespaceCopy(textIn);
    return (ToLowerCopy(text) == "null") ? "" : text;
}

static std::string NormalizeVideoOsdDateStyle(const std::string& formatIn)
{
    const std::string format = ToLowerCopy(formatIn);
    if (format.empty()) {
        return "CHR-YYYY-MM-DD";
    }

    if (ContainsToken(format, "yyyy/mm/dd")) {
        return "CHR-YYYY/MM/DD";
    }
    if (ContainsToken(format, "mm/dd/yyyy")) {
        return "CHR-MM/DD/YYYY";
    }
    if (ContainsToken(format, "dd/mm/yyyy")) {
        return "CHR-DD/MM/YYYY";
    }
    if (ContainsToken(format, "mm-dd-yyyy")) {
        return "CHR-MM-DD-YYYY";
    }
    if (ContainsToken(format, "dd-mm-yyyy")) {
        return "CHR-DD-MM-YYYY";
    }

    return "CHR-YYYY-MM-DD";
}

static std::string NormalizeVideoOsdTimeStyle(const std::string& formatIn)
{
    const std::string format = ToLowerCopy(formatIn);
    if (format.empty()) {
        return "24hour";
    }

    if (ContainsToken(format, "tt") ||
        ContainsToken(format, " am") ||
        ContainsToken(format, " pm") ||
        ContainsToken(format, "a/p") ||
        ContainsToken(format, "12hour")) {
        return "12hour";
    }

    return "24hour";
}

static std::string BuildVideoOsdTimeFormatFromStyles(const std::string& dateStyleIn,
                                                     const std::string& timeStyleIn)
{
    std::string datePart = "yyyy-MM-dd";
    const std::string dateStyle = ToLowerCopy(TrimWhitespaceCopy(dateStyleIn));
    if (ContainsToken(dateStyle, "yyyy/mm/dd")) {
        datePart = "yyyy/MM/dd";
    } else if (ContainsToken(dateStyle, "mm/dd/yyyy")) {
        datePart = "MM/dd/yyyy";
    } else if (ContainsToken(dateStyle, "dd/mm/yyyy")) {
        datePart = "dd/MM/yyyy";
    } else if (ContainsToken(dateStyle, "mm-dd-yyyy")) {
        datePart = "MM-dd-yyyy";
    } else if (ContainsToken(dateStyle, "dd-mm-yyyy")) {
        datePart = "dd-MM-yyyy";
    }

    std::string timePart = "HH:mm:ss";
    const std::string timeStyle = ToLowerCopy(TrimWhitespaceCopy(timeStyleIn));
    if (ContainsToken(timeStyle, "12hour")) {
        timePart = "hh:mm:ss tt";
    }

    return datePart + " " + timePart;
}

static bool QueryMainStreamResolution(int& width, int& height)
{
    width = 0;
    height = 0;
    return CaptureGetResolution(0, &width, &height) == 0 && width > 0 && height > 0;
}

static int MergeError(int current, int candidate)
{
    return (current != 0) ? current : candidate;
}

static void NormalizeVideoOsdTextItems(media::VideoOsdState* state)
{
    if (state == NULL) {
        return;
    }

    if (!state->has_text_items) {
        state->text_items.clear();
        return;
    }

    size_t writeIndex = 0;
    for (size_t index = 0; index < state->text_items.size(); ++index) {
        media::VideoOsdTextItem item = state->text_items[index];
        if (item.has_text) {
            item.text = NormalizeVideoOsdTextTemplate(item.text);
            item.has_text = !item.text.empty();
        }
        if (!item.has_text && !item.has_position) {
            continue;
        }
        if (writeIndex != index) {
            state->text_items[writeIndex] = item;
        }
        ++writeIndex;
    }

    state->text_items.resize(writeIndex);
    state->has_text_items = !state->text_items.empty();
}

static media::VideoOsdTextItem* EnsurePrimaryVideoOsdTextItem(media::VideoOsdState* state)
{
    if (state == NULL) {
        return NULL;
    }

    if (!state->has_text_items) {
        state->has_text_items = true;
        state->text_items.clear();
    }
    if (state->text_items.empty()) {
        state->text_items.push_back(media::VideoOsdTextItem());
    }

    return &state->text_items[0];
}

static const media::VideoOsdTextItem* GetPrimaryVideoOsdTextItem(const media::VideoOsdState& state)
{
    if (!state.has_text_items || state.text_items.empty()) {
        return NULL;
    }

    return &state.text_items[0];
}

static std::string GetPrimaryVideoOsdTextTemplate(const media::VideoOsdState& state)
{
    const media::VideoOsdTextItem* item = GetPrimaryVideoOsdTextItem(state);
    if (item == NULL || !item->has_text) {
        return "";
    }

    return NormalizeVideoOsdTextTemplate(item->text);
}

static bool GetVideoOsdTimePosition(const media::VideoOsdState& state, int* x, int* y)
{
    if (x == NULL || y == NULL ||
        !state.has_time_position ||
        state.time_x < 0 ||
        state.time_y < 0) {
        return false;
    }

    *x = state.time_x;
    *y = state.time_y;
    return true;
}

static bool GetPrimaryVideoOsdTextPosition(const media::VideoOsdState& state, int* x, int* y)
{
    if (x == NULL || y == NULL) {
        return false;
    }

    const media::VideoOsdTextItem* item = GetPrimaryVideoOsdTextItem(state);
    if (item == NULL || !item->has_position || item->x < 0 || item->y < 0) {
        return false;
    }

    *x = item->x;
    *y = item->y;
    return true;
}

static void CacheVideoOsdProtocolState(const media::VideoOsdState& desired)
{
    g_cached_protocol_state = desired;
    NormalizeVideoOsdTextItems(&g_cached_protocol_state);
    if (!g_cached_protocol_state.has_time_format &&
        (g_cached_protocol_state.has_date_style ||
         g_cached_protocol_state.has_time_style)) {
        g_cached_protocol_state.has_time_format = true;
        g_cached_protocol_state.time_format = BuildVideoOsdTimeFormatFromStyles(
            g_cached_protocol_state.date_style,
            g_cached_protocol_state.time_style);
    }
    g_has_cached_protocol_state = true;
}

} // namespace

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

std::string strToHexAscii(const std::string &input) {
    std::stringstream ss;
    
    for (char c : input) {
        // 转两位大写十六进制
        ss << hex << uppercase << setw(2) << setfill('0') << (unsigned int)(unsigned char)c;
    }
    
    return ss.str();
}
std::string hexToStr(const std::string &hexStr) {
    std::string result;
    for (int i = 0; i < hexStr.size(); i += 2) {
        // 每两位截取
        string byteHex = hexStr.substr(i, 2);
        // 转成一个字节
        char c = (char)strtol(byteHex.c_str(), NULL, 16);
        result += c;
    }
    return result;
}


namespace media
{

bool ResolveVideoOsdAnchor(const std::string& positionIn, int* x, int* y)
{
    if (x == NULL || y == NULL) {
        return false;
    }

    *x = 0;
    *y = 0;

    const std::string position = ToLowerCopy(positionIn);
    if (position.empty()) {
        return false;
    }

    if ((sscanf(position.c_str(), "%d,%d", x, y) == 2 ||
         sscanf(position.c_str(), "%d:%d", x, y) == 2) &&
        *x >= 0 &&
        *y >= 0) {
        return true;
    }

    int width = 1280;
    int height = 720;
    (void)QueryMainStreamResolution(width, height);

    const int marginX = 0;
    const int marginY = 0;
    const int estimatedTextWidth = 176;
    const int estimatedTextHeight = 64;

    if (position == "top_left" || position == "left_top" || position == "top-left") {
        *x = marginX;
        *y = marginY;
        return true;
    }

    if (position == "top_right" || position == "right_top" || position == "top-right") {
        *x = (width > estimatedTextWidth + marginX) ? (width - estimatedTextWidth) : marginX;
        *y = marginY;
        return true;
    }

    if (position == "bottom_left" || position == "left_bottom" || position == "bottom-left") {
        *x = marginX;
        *y = (height > estimatedTextHeight + marginY) ? (height - estimatedTextHeight) : marginY;
        return true;
    }

    if (position == "bottom_right" || position == "right_bottom" || position == "bottom-right") {
        *x = (width > estimatedTextWidth + marginX) ? (width - estimatedTextWidth) : marginX;
        *y = (height > estimatedTextHeight + marginY) ? (height - estimatedTextHeight) : marginY;
        return true;
    }

    if (position == "center" || position == "middle") {
        *x = width / 2;
        *y = height / 2;
        return true;
    }

    return false;
}

int ApplyVideoOsdConfig(const VideoOsdState& desired)
{
    VideoOsdState normalizedDesired = desired;
    NormalizeVideoOsdTextItems(&normalizedDesired);

	printf("has_time_enabled: %d, time_enabled: %d\n", normalizedDesired.has_time_enabled, normalizedDesired.time_enabled);
	printf("has_text_enabled: %d, text_enabled: %d\n", normalizedDesired.has_text_enabled, normalizedDesired.text_enabled);
	printf("has_time_format: %d, time_format: %s\n", normalizedDesired.has_time_format, normalizedDesired.time_format.c_str());
	printf("has_date_style: %d, date_style: %s\n", normalizedDesired.has_date_style, normalizedDesired.date_style.c_str());
	printf("has_time_style: %d, time_style: %s\n", normalizedDesired.has_time_style, normalizedDesired.time_style.c_str());
	printf("has_time_position: %d\n", normalizedDesired.has_time_position);
	printf("time_x: %d, time_y: %d\n", normalizedDesired.time_x, normalizedDesired.time_y);
	printf("has_text_items: %d\n", normalizedDesired.has_text_items);
    for (size_t index = 0; index < normalizedDesired.text_items.size(); ++index) {
        media::VideoOsdTextItem item = normalizedDesired.text_items[index];
		printf("text_items[%d].has_text: %d\n", index, item.has_text);
		printf("text_items[%d].text: %s\n", index, item.text.c_str());
		const char *p = item.text.c_str();
		printf("[");
		for (int i = 0; p[i] != '\0'; i++)
			printf("%02X ", p[i]);
		printf("]\n");
		printf("text_items[%d].has_position: %d\n", index, item.has_position);
		printf("text_items[%d].x: %d y: %d\n", index, item.x, item.y);
    }

	CConfigTable table;
	OSDTimeConf_S curOSDTimeConfig;
	OSDTimeConf_S newOSDTimeConfig;
	g_configManager.getConfig(getConfigName(CFG_OSD_TIME), table);
    TExchangeAL<OSDTimeConf_S>::getConfig(table, curOSDTimeConfig);
	newOSDTimeConfig = curOSDTimeConfig;

	if (normalizedDesired.has_time_enabled)
	{
		newOSDTimeConfig.show = normalizedDesired.time_enabled;
	}
	if (normalizedDesired.has_time_position)
	{
		newOSDTimeConfig.x = normalizedDesired.time_x;
		newOSDTimeConfig.y = normalizedDesired.time_y;
	}
	if (normalizedDesired.has_time_format)
	{
		//yyyy-MM-dd HH:mm:ss
		//yyyy年MM月dd日 HH:mm:ss
		const std::string format = ToLowerCopy(normalizedDesired.time_format);
		if (ContainsToken(format, "yyyy年mm月dd日") == false)
			newOSDTimeConfig.date_type = 0;
		else
			newOSDTimeConfig.date_type = 1;
		newOSDTimeConfig.time_type = 0;
	}
	if (newOSDTimeConfig.date_type != curOSDTimeConfig.date_type || 
		newOSDTimeConfig.time_type != curOSDTimeConfig.time_type || 
		newOSDTimeConfig.x != curOSDTimeConfig.x || 
		newOSDTimeConfig.y != curOSDTimeConfig.y || 
		newOSDTimeConfig.show != curOSDTimeConfig.show)
	{
		table.clear();
		TExchangeAL<OSDTimeConf_S>::setConfig(newOSDTimeConfig, table);
		g_configManager.setConfig(getConfigName(CFG_OSD_TIME), table, 0, IConfigManager::applyOK);
	}

	OSDTextAllConf_S curOSDTextAllConfig;
	OSDTextAllConf_S newOSDTextAllConfig;
	g_configManager.getConfig(getConfigName(CFG_OSD_TEXT), table);
    TExchangeAL<OSDTextAllConf_S>::getConfig(table, curOSDTextAllConfig);
	newOSDTextAllConfig = curOSDTextAllConfig;

	if (normalizedDesired.has_text_items)
	{
		int i;
		for (i = 0; i < OSD_TEXT_MAX && i < normalizedDesired.text_items.size(); i++)
		{
			media::VideoOsdTextItem &item = normalizedDesired.text_items[i];
			if (item.has_text)
			{
				newOSDTextAllConfig.osd_text[i].text = strToHexAscii(item.text);
				const char *p = newOSDTextAllConfig.osd_text[i].text.c_str();
				printf("ApplyVideoOsdConfig -> [%s]\n", p);
//				snprintf(newOSDTextAllConfig.osd_text[i].text, sizeof(newOSDTextAllConfig.osd_text[i].text), item.text.c_str());
//				const char *p = newOSDTextAllConfig.osd_text[i].text.c_str();
//				printf("ApplyVideoOsdConfig -> [");
//				for (int i = 0; p[i] != '\0'; i++)
//					printf("%02X ", p[i]);
//				printf("]\n");
			}
			if (item.has_position)
			{
				newOSDTextAllConfig.osd_text[i].x = item.x;
				newOSDTextAllConfig.osd_text[i].y = item.y;
			}
			newOSDTextAllConfig.osd_text[i].show = 1;
		}
		for (; i < OSD_TEXT_MAX; i++)
		{
			newOSDTextAllConfig.osd_text[i].show = 0;
		}
	}
	else
	{
		for (int i = 0; i < OSD_TEXT_MAX; i++)
		{
			newOSDTextAllConfig.osd_text[i].show = 0;
		}
	}
	for (int i = 0; i < OSD_TEXT_MAX; i++)
	{
		if (newOSDTextAllConfig.osd_text[i].text != curOSDTextAllConfig.osd_text[i].text || 
			newOSDTextAllConfig.osd_text[i].x != curOSDTextAllConfig.osd_text[i].x || 
			newOSDTextAllConfig.osd_text[i].y != curOSDTextAllConfig.osd_text[i].y || 
			newOSDTextAllConfig.osd_text[i].show != curOSDTextAllConfig.osd_text[i].show)
		{
			table.clear();
			TExchangeAL<OSDTextAllConf_S>::setConfig(newOSDTextAllConfig, table);
			g_configManager.setConfig(getConfigName(CFG_OSD_TEXT), table, 0, IConfigManager::applyOK);
			break;
		}
	}

    return 0;
}

bool QueryVideoOsdState(VideoOsdState* state)
{
    if (state == NULL) {
        return false;
    }

    *state = VideoOsdState();
//    if (g_has_cached_protocol_state) {
//        *state = g_cached_protocol_state;
//        NormalizeVideoOsdTextItems(state);
//    }
//
//    if (g_cached_master_switch >= 0) {
//        state->has_master_enabled = true;
//        state->master_enabled = g_cached_master_switch;
//    }
//
//    if (g_cached_event_switch >= 0) {
//        state->has_event_enabled = true;
//        state->event_enabled = g_cached_event_switch;
//    }
//
//    if (g_cached_alert_switch >= 0) {
//        state->has_alert_enabled = true;
//        state->alert_enabled = g_cached_alert_switch;
//    }

	CConfigTable table;
	OSDTimeConf_S curOSDTimeConfig;
	g_configManager.getConfig(getConfigName(CFG_OSD_TIME), table);
    TExchangeAL<OSDTimeConf_S>::getConfig(table, curOSDTimeConfig);

	OSDTextAllConf_S curOSDTextAllConfig;
	g_configManager.getConfig(getConfigName(CFG_OSD_TEXT), table);
    TExchangeAL<OSDTextAllConf_S>::getConfig(table, curOSDTextAllConfig);

	state->has_time_enabled = true;
	state->time_enabled = curOSDTimeConfig.show;

	state->has_text_enabled = true;
	state->text_enabled = 0;
	for (int i = 0; i < OSD_TEXT_MAX; i++)
	{
		if (curOSDTextAllConfig.osd_text[i].show)
		{
			state->text_enabled = 1;
			break;
		}
	}

	state->has_time_format = true;
	if (0 == curOSDTimeConfig.date_type)
		state->time_format = "yyyy-MM-dd HH:mm:ss";
	else
		state->time_format = "yyyy年MM月dd日 HH:mm:ss";
	printf("state->time_format: %s\n", state->time_format.c_str());

	state->has_time_position = true;
	state->time_x = curOSDTimeConfig.x;
	state->time_y = curOSDTimeConfig.y;

	state->has_text_items = true;
	for (int i = 0; i < OSD_TEXT_MAX; i++)
	{
		if (curOSDTextAllConfig.osd_text[i].show)
		{
			media::VideoOsdTextItem item;
			item.has_text = true;			
			item.text = hexToStr(curOSDTextAllConfig.osd_text[i].text);
			item.has_position = true;
			item.x = curOSDTextAllConfig.osd_text[i].x;
			item.y = curOSDTextAllConfig.osd_text[i].y;
			state->text_items.push_back(item);
		}
	}

	printf("state->text_items size: %d\n", state->text_items.size());

    NormalizeVideoOsdTextItems(state);

    if (!state->has_master_enabled &&
        (state->has_time_enabled || state->has_event_enabled || state->has_alert_enabled)) {
        state->has_master_enabled = true;
        state->master_enabled = ((state->has_time_enabled && state->time_enabled != 0) ||
                                 (state->has_event_enabled && state->event_enabled != 0) ||
                                 (state->has_alert_enabled && state->alert_enabled != 0)) ? 1 : 0;
    }

    return state->has_master_enabled ||
           state->has_event_enabled ||
           state->has_alert_enabled ||
           state->has_time_enabled ||
           state->has_text_enabled ||
           state->has_time_format ||
           state->has_date_style ||
           state->has_time_style ||
           state->has_time_position ||
           state->has_text_items;
}

} // namespace media
