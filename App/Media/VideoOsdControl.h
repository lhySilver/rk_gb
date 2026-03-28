#ifndef _VIDEO_OSD_CONTROL_H_
#define _VIDEO_OSD_CONTROL_H_

#include <string>

namespace media
{

struct VideoOsdConfig
{
    int time_enabled;
    int event_enabled;
    int alert_enabled;
    std::string text_template;
    std::string time_format;
    std::string position;

    VideoOsdConfig()
        : time_enabled(0),
          event_enabled(0),
          alert_enabled(0)
    {
    }
};

struct VideoOsdState
{
    bool has_master_enabled;
    int master_enabled;
    bool has_time_enabled;
    int time_enabled;
    bool has_text_enabled;
    int text_enabled;
    bool has_date_style;
    std::string date_style;
    bool has_time_style;
    std::string time_style;
    bool has_time_position;
    int time_x;
    int time_y;
    bool has_text_position;
    int text_x;
    int text_y;
    bool has_text;
    std::string text;

    VideoOsdState()
        : has_master_enabled(false),
          master_enabled(0),
          has_time_enabled(false),
          time_enabled(0),
          has_text_enabled(false),
          text_enabled(0),
          has_date_style(false),
          has_time_style(false),
          has_time_position(false),
          time_x(0),
          time_y(0),
          has_text_position(false),
          text_x(0),
          text_y(0),
          has_text(false)
    {
    }
};

bool ResolveVideoOsdAnchor(const std::string& position, int* x, int* y);
int ApplyVideoOsdConfig(const VideoOsdConfig& desired);
bool QueryVideoOsdState(VideoOsdState* state);

} // namespace media

#endif
