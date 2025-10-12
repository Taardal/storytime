#include "st_log.h"

namespace Storytime {
    void initialize_log(LogLevel level) {
        set_log_level(level);

        // https://github.com/gabime/spdlog/wiki/Custom-formatting#pattern-flags
        spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S:%e] [%l%$] [%s:%#] [%!] %v%$");
    }

    void set_log_level(const LogLevel level) {
        spdlog::set_level(level);
    }

    std::string tag(const char* file_name, const char* function_name, const int line_number) {
        std::stringstream ss;
        ss << file_name;
        ss << ":";
        ss << line_number;
        ss << " :: ";
        ss << function_name;
        return ss.str();
    }

    std::string tag(const std::source_location& source_location) {
        std::stringstream ss;
        ss << source_location.file_name();
        ss << ":";
        ss << source_location.line();
        ss << ":";
        ss << source_location.column();
        ss << " :: ";
        ss << source_location.function_name();
        return ss.str();
    }
}
