#include "VideoOsdControl.h"

#include <stdio.h>
#include <string.h>

#include "PAL/Capture.h"

extern "C"
{
int rk_osd_get_enabled(int id, int *value);
int rk_osd_set_enabled(int id, int value);
int rk_osd_restart();
int rk_osd_get_position_x(int id, int *value);
int rk_osd_set_position_x(int id, int value);
int rk_osd_get_position_y(int id, int *value);
int rk_osd_set_position_y(int id, int value);
int rk_osd_get_date_style(int id, const char **value);
int rk_osd_set_date_style(int id, const char *value);
int rk_osd_get_time_style(int id, const char **value);
int rk_osd_set_time_style(int id, const char *value);
int rk_osd_get_display_text(int id, const char **value);
int rk_osd_set_display_text(int id, const char *value);
}

namespace
{

static const int kVideoOsdDateTimeId = 1;
static const int kVideoOsdCustomTextId = 2;
static int g_cached_master_switch = -1;

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

int ApplyVideoOsdConfig(const VideoOsdConfig& desired)
{
    int finalRet = 0;
    const std::string normalizedTextTemplate = NormalizeVideoOsdTextTemplate(desired.text_template);
    const int desiredTimeEnabled = (desired.time_enabled != 0) ? 1 : 0;
    const int desiredCustomTextEnabled = normalizedTextTemplate.empty() ? 0 : 1;
    const int desiredMasterSwitch =
        (desiredTimeEnabled != 0 || desired.event_enabled != 0 || desired.alert_enabled != 0) ? 1 : 0;

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

    if (!desired.time_format.empty()) {
        const std::string desiredDateStyle = NormalizeVideoOsdDateStyle(desired.time_format);
        const std::string desiredTimeStyle = NormalizeVideoOsdTimeStyle(desired.time_format);
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

    if (!desired.position.empty()) {
        int anchorX = 0;
        int anchorY = 0;
        if (ResolveVideoOsdAnchor(desired.position, &anchorX, &anchorY)) {
            int currentX = 0;
            int currentY = 0;
            const bool timePositionKnown = ReadVideoOsdPosition(kVideoOsdDateTimeId, &currentX, &currentY);
            if (!timePositionKnown || currentX != anchorX || currentY != anchorY) {
                const int retX = rk_osd_set_position_x(kVideoOsdDateTimeId, anchorX);
                const int retY = rk_osd_set_position_y(kVideoOsdDateTimeId, anchorY);
                printf("[VideoOsdControl] apply datetime_position ret=%d value=%d,%d current=%d,%d\n",
                       (retX != 0) ? retX : retY,
                       anchorX,
                       anchorY,
                       timePositionKnown ? currentX : -1,
                       timePositionKnown ? currentY : -1);
                finalRet = MergeError(finalRet, (retX != 0) ? retX : retY);
                needRestart = true;
            }

            currentX = 0;
            currentY = 0;
            const bool textPositionKnown = ReadVideoOsdPosition(kVideoOsdCustomTextId, &currentX, &currentY);
            if (!textPositionKnown || currentX != anchorX || currentY != anchorY) {
                const int retX = rk_osd_set_position_x(kVideoOsdCustomTextId, anchorX);
                const int retY = rk_osd_set_position_y(kVideoOsdCustomTextId, anchorY);
                printf("[VideoOsdControl] apply text_position ret=%d value=%d,%d current=%d,%d\n",
                       (retX != 0) ? retX : retY,
                       anchorX,
                       anchorY,
                       textPositionKnown ? currentX : -1,
                       textPositionKnown ? currentY : -1);
                finalRet = MergeError(finalRet, (retX != 0) ? retX : retY);
                needRestart = true;
            }
        }
    }

    if (needRestart) {
        const int ret = rk_osd_restart();
        printf("[VideoOsdControl] apply restart ret=%d\n", ret);
        finalRet = MergeError(finalRet, ret);
    }

    return finalRet;
}

bool QueryVideoOsdState(VideoOsdState* state)
{
    if (state == NULL) {
        return false;
    }

    *state = VideoOsdState();

    if (g_cached_master_switch >= 0) {
        state->has_master_enabled = true;
        state->master_enabled = g_cached_master_switch;
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

    int posX = 0;
    int posY = 0;
    if (ReadVideoOsdPosition(kVideoOsdDateTimeId, &posX, &posY) && posX >= 0 && posY >= 0) {
        state->has_time_position = true;
        state->time_x = posX;
        state->time_y = posY;
    }

    if (ReadVideoOsdPosition(kVideoOsdCustomTextId, &posX, &posY) && posX >= 0 && posY >= 0) {
        state->has_text_position = true;
        state->text_x = posX;
        state->text_y = posY;
    }

    if (ReadVideoOsdStringValue(rk_osd_get_display_text, kVideoOsdCustomTextId, text)) {
        state->has_text = true;
        state->text = text;
    }

    return state->has_master_enabled ||
           state->has_time_enabled ||
           state->has_text_enabled ||
           state->has_date_style ||
           state->has_time_style ||
           state->has_time_position ||
           state->has_text_position ||
           state->has_text;
}

} // namespace media
