#include "st_log.h"

namespace Storytime {
    void set_log_level(const LogLevel level) {
        spdlog::set_level(level);
    }

    std::string tag(const char* file_name, const char* function_name, const int line_number) {
        std::stringstream ss;
        ss << file_name << ":" << line_number << " (" << function_name << ")";
        return ss.str();
    }

    std::string tag_message(const char* file_name, const char* function_name, const int line_number, const std::string_view message) {
        std::stringstream ss;
        ss << "[" << file_name << ":" << line_number << "] [" << function_name << "] " << message;
        return ss.str();
    }
}