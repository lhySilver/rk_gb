#include "VideoOsdControl.h"

#include <stdio.h>
#include <string.h>

#include "PAL/Capture.h"

//extern "C"
//{
//int rk_osd_get_enabled(int id, int *value);
//int rk_osd_set_enabled(int id, int value);
//int rk_osd_restart();
//int rk_osd_get_position_x(int id, int *value);
//int rk_osd_set_position_x(int id, int value);
//int rk_osd_get_position_y(int id, int *value);
//int rk_osd_set_position_y(int id, int value);
//int rk_osd_get_date_style(int id, const char **value);
//int rk_osd_set_date_style(int id, const char *value);
//int rk_osd_get_time_style(int id, const char **value);
//int rk_osd_set_time_style(int id, const char *value);
//int rk_osd_get_display_text(int id, const char **value);
//int rk_osd_set_display_text(int id, const char *value);
//}
int rk_osd_get_enabled(int id, int *value){}
int rk_osd_set_enabled(int id, int value){}
int rk_osd_restart(){}
int rk_osd_get_position_x(int id, int *value){}
int rk_osd_set_position_x(int id, int value){}
int rk_osd_get_position_y(int id, int *value){}
int rk_osd_set_position_y(int id, int value){}
int rk_osd_get_date_style(int id, const char **value){}
int rk_osd_set_date_style(int id, const char *value){}
int rk_osd_get_time_style(int id, const char **value){}
int rk_osd_set_time_style(int id, const char *value){}
int rk_osd_get_display_text(int id, const char **value){}
int rk_osd_set_display_text(int id, const char *value){}


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

static bool ReadVideoOsdEnabled(int id, int* value)
{
    if (value == NULL) {
        return false;
    }

    return rk_osd_get_enabled(id, value) == 0;
}

static bool ReadVideoOsdStringValue(int (*getter)(int, const char**), int id, std::string& value)
{
    if (getter == NULL) {
        return false;
    }

    const char* out = NULL;
    if (getter(id, &out) != 0 || out == NULL || out[0] == '\0') {
        return false;
    }

    value = out;
    return true;
}

static bool ReadVideoOsdPosition(int id, int* x, int* y)
{
    if (x == NULL || y == NULL) {
        return false;
    }

    return rk_osd_get_position_x(id, x) == 0 && rk_osd_get_position_y(id, y) == 0;
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

    const int marginX = 16;
    const int marginY = 16;
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

    int finalRet = 0;
    const std::string normalizedTextTemplate = GetPrimaryVideoOsdTextTemplate(normalizedDesired);
    const int desiredTimeEnabled =
        (normalizedDesired.has_time_enabled && normalizedDesired.time_enabled != 0) ? 1 : 0;
    const int desiredEventEnabled =
        (normalizedDesired.has_event_enabled && normalizedDesired.event_enabled != 0) ? 1 : 0;
    const int desiredAlertEnabled =
        (normalizedDesired.has_alert_enabled && normalizedDesired.alert_enabled != 0) ? 1 : 0;
    const int desiredCustomTextEnabled = normalizedDesired.has_text_enabled ?
        ((normalizedDesired.text_enabled != 0) ? 1 : 0) :
        (normalizedTextTemplate.empty() ? 0 : 1);
    const int desiredMasterSwitch = normalizedDesired.has_master_enabled ?
        ((normalizedDesired.master_enabled != 0) ? 1 : 0) :
        ((desiredTimeEnabled != 0 || desiredEventEnabled != 0 || desiredAlertEnabled != 0) ? 1 : 0);
    const std::string desiredTimeFormat = normalizedDesired.has_time_format ?
        TrimWhitespaceCopy(normalizedDesired.time_format) :
        ((normalizedDesired.has_date_style || normalizedDesired.has_time_style) ?
            BuildVideoOsdTimeFormatFromStyles(normalizedDesired.date_style, normalizedDesired.time_style) :
            std::string());

    // Capture only exposes a setter, so media side keeps the last applied master switch.
    if (g_cached_master_switch < 0 || g_cached_master_switch != desiredMasterSwitch) {
        const int ret = CaptureSetOSDSwitch(desiredMasterSwitch);
        printf("[VideoOsdControl] apply master_switch ret=%d value=%d current=%d\n",
               ret,
               desiredMasterSwitch,
               g_cached_master_switch);
        if (ret == 0) {
            g_cached_master_switch = desiredMasterSwitch;
        }
        finalRet = MergeError(finalRet, ret);
    }

    int currentTimeEnabled = 0;
    const bool timeKnown = ReadVideoOsdEnabled(kVideoOsdDateTimeId, &currentTimeEnabled);
    if (!timeKnown || currentTimeEnabled != desiredTimeEnabled) {
        const int ret = rk_osd_set_enabled(kVideoOsdDateTimeId, desiredTimeEnabled);
        printf("[VideoOsdControl] apply datetime_enabled ret=%d value=%d current=%d\n",
               ret,
               desiredTimeEnabled,
               timeKnown ? currentTimeEnabled : -1);
        finalRet = MergeError(finalRet, ret);
    }

    int currentCustomTextEnabled = 0;
    const bool customKnown = ReadVideoOsdEnabled(kVideoOsdCustomTextId, &currentCustomTextEnabled);
    if (!customKnown || currentCustomTextEnabled != desiredCustomTextEnabled) {
        const int ret = rk_osd_set_enabled(kVideoOsdCustomTextId, desiredCustomTextEnabled);
        printf("[VideoOsdControl] apply text_enabled ret=%d value=%d current=%d\n",
               ret,
               desiredCustomTextEnabled,
               customKnown ? currentCustomTextEnabled : -1);
        finalRet = MergeError(finalRet, ret);
    }

    bool needRestart = false;

    if (!normalizedTextTemplate.empty()) {
        std::string currentText;
        const bool textKnown = ReadVideoOsdStringValue(rk_osd_get_display_text, kVideoOsdCustomTextId, currentText);
        if (!textKnown || currentText != normalizedTextTemplate) {
            const int ret = rk_osd_set_display_text(kVideoOsdCustomTextId, normalizedTextTemplate.c_str());
            printf("[VideoOsdControl] apply text ret=%d value=%s current=%s\n",
                   ret,
                   normalizedTextTemplate.c_str(),
                   textKnown ? currentText.c_str() : "unknown");
            finalRet = MergeError(finalRet, ret);
            needRestart = true;
        }
    }

    if (!desiredTimeFormat.empty()) {
        const std::string desiredDateStyle = NormalizeVideoOsdDateStyle(desiredTimeFormat);
        const std::string desiredTimeStyle = NormalizeVideoOsdTimeStyle(desiredTimeFormat);
        std::string currentDateStyle;
        std::string currentTimeStyle;
        const bool dateKnown = ReadVideoOsdStringValue(rk_osd_get_date_style, kVideoOsdDateTimeId, currentDateStyle);
        const bool timeStyleKnown = ReadVideoOsdStringValue(rk_osd_get_time_style, kVideoOsdDateTimeId, currentTimeStyle);
        if (!dateKnown || currentDateStyle != desiredDateStyle) {
            const int ret = rk_osd_set_date_style(kVideoOsdDateTimeId, desiredDateStyle.c_str());
            printf("[VideoOsdControl] apply date_style ret=%d value=%s current=%s\n",
                   ret,
                   desiredDateStyle.c_str(),
                   dateKnown ? currentDateStyle.c_str() : "unknown");
            finalRet = MergeError(finalRet, ret);
            needRestart = true;
        }
        if (!timeStyleKnown || currentTimeStyle != desiredTimeStyle) {
            const int ret = rk_osd_set_time_style(kVideoOsdDateTimeId, desiredTimeStyle.c_str());
            printf("[VideoOsdControl] apply time_style ret=%d value=%s current=%s\n",
                   ret,
                   desiredTimeStyle.c_str(),
                   timeStyleKnown ? currentTimeStyle.c_str() : "unknown");
            finalRet = MergeError(finalRet, ret);
            needRestart = true;
        }
    }

    int timeAnchorX = 0;
    int timeAnchorY = 0;
    const bool hasTimePosition = GetVideoOsdTimePosition(normalizedDesired, &timeAnchorX, &timeAnchorY);
    if (hasTimePosition) {
        int currentX = 0;
        int currentY = 0;
        const bool timePositionKnown = ReadVideoOsdPosition(kVideoOsdDateTimeId, &currentX, &currentY);
        if (!timePositionKnown || currentX != timeAnchorX || currentY != timeAnchorY) {
            const int retX = rk_osd_set_position_x(kVideoOsdDateTimeId, timeAnchorX);
            const int retY = rk_osd_set_position_y(kVideoOsdDateTimeId, timeAnchorY);
            printf("[VideoOsdControl] apply datetime_position ret=%d value=%d,%d current=%d,%d\n",
                   (retX != 0) ? retX : retY,
                   timeAnchorX,
                   timeAnchorY,
                   timePositionKnown ? currentX : -1,
                   timePositionKnown ? currentY : -1);
            finalRet = MergeError(finalRet, (retX != 0) ? retX : retY);
            needRestart = true;
        }
    }

    int textAnchorX = 0;
    int textAnchorY = 0;
    bool hasTextPosition = GetPrimaryVideoOsdTextPosition(normalizedDesired, &textAnchorX, &textAnchorY);
    if (!hasTextPosition && hasTimePosition) {
        textAnchorX = timeAnchorX;
        textAnchorY = timeAnchorY;
        hasTextPosition = true;
    }
    if (hasTextPosition) {
        int currentX = 0;
        int currentY = 0;
        const bool textPositionKnown = ReadVideoOsdPosition(kVideoOsdCustomTextId, &currentX, &currentY);
        if (!textPositionKnown || currentX != textAnchorX || currentY != textAnchorY) {
            const int retX = rk_osd_set_position_x(kVideoOsdCustomTextId, textAnchorX);
            const int retY = rk_osd_set_position_y(kVideoOsdCustomTextId, textAnchorY);
            printf("[VideoOsdControl] apply text_position ret=%d value=%d,%d current=%d,%d\n",
                   (retX != 0) ? retX : retY,
                   textAnchorX,
                   textAnchorY,
                   textPositionKnown ? currentX : -1,
                   textPositionKnown ? currentY : -1);
            finalRet = MergeError(finalRet, (retX != 0) ? retX : retY);
            needRestart = true;
        }
    }

    if (needRestart) {
        const int ret = rk_osd_restart();
        printf("[VideoOsdControl] apply restart ret=%d\n", ret);
        finalRet = MergeError(finalRet, ret);
    }

    if (finalRet == 0) {
        g_cached_event_switch = desiredEventEnabled;
        g_cached_alert_switch = desiredAlertEnabled;
        CacheVideoOsdProtocolState(normalizedDesired);
    }

    return finalRet;
}

bool QueryVideoOsdState(VideoOsdState* state)
{
    if (state == NULL) {
        return false;
    }

    *state = VideoOsdState();
    if (g_has_cached_protocol_state) {
        *state = g_cached_protocol_state;
        NormalizeVideoOsdTextItems(state);
    }

    if (g_cached_master_switch >= 0) {
        state->has_master_enabled = true;
        state->master_enabled = g_cached_master_switch;
    }

    if (g_cached_event_switch >= 0) {
        state->has_event_enabled = true;
        state->event_enabled = g_cached_event_switch;
    }

    if (g_cached_alert_switch >= 0) {
        state->has_alert_enabled = true;
        state->alert_enabled = g_cached_alert_switch;
    }

    int value = 0;
    if (ReadVideoOsdEnabled(kVideoOsdDateTimeId, &value)) {
        state->has_time_enabled = true;
        state->time_enabled = value;
    }

    if (ReadVideoOsdEnabled(kVideoOsdCustomTextId, &value)) {
        state->has_text_enabled = true;
        state->text_enabled = value;
    }

    std::string text;
    if (ReadVideoOsdStringValue(rk_osd_get_date_style, kVideoOsdDateTimeId, text)) {
        state->has_date_style = true;
        state->date_style = text;
    }

    if (ReadVideoOsdStringValue(rk_osd_get_time_style, kVideoOsdDateTimeId, text)) {
        state->has_time_style = true;
        state->time_style = text;
    }

    if (!state->has_time_format &&
        (state->has_date_style || state->has_time_style)) {
        state->has_time_format = true;
        state->time_format = BuildVideoOsdTimeFormatFromStyles(state->date_style, state->time_style);
    }

    int posX = 0;
    int posY = 0;
    if (ReadVideoOsdPosition(kVideoOsdDateTimeId, &posX, &posY) && posX >= 0 && posY >= 0) {
        state->has_time_position = true;
        state->time_x = posX;
        state->time_y = posY;
    }

    const bool textPositionKnown =
        ReadVideoOsdPosition(kVideoOsdCustomTextId, &posX, &posY) && posX >= 0 && posY >= 0;
    if (textPositionKnown) {
        VideoOsdTextItem* item = EnsurePrimaryVideoOsdTextItem(state);
        if (item != NULL) {
            item->has_position = true;
            item->x = posX;
            item->y = posY;
        }
    }

    if (ReadVideoOsdStringValue(rk_osd_get_display_text, kVideoOsdCustomTextId, text)) {
        VideoOsdTextItem* item = EnsurePrimaryVideoOsdTextItem(state);
        if (item != NULL) {
            item->has_text = true;
            item->text = NormalizeVideoOsdTextTemplate(text);
        }
    }
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
