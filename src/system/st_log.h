#pragma once

#include <source_location>

// Set compile-time log-level. This determines which log statements are included in the compiled code.
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

#define ST_FILE_NAME __FILE_NAME__
#define ST_FUNCTION_NAME __func__
#define ST_LINE_NUMBER __LINE__

#define ST_TAG ::Storytime::tag(std::source_location::current())

#define ST_LOG_TRACE(message, ...) SPDLOG_TRACE(message, ##__VA_ARGS__)
#define ST_LOG_DEBUG(message, ...) SPDLOG_DEBUG(message, ##__VA_ARGS__)
#define ST_LOG_INFO(message, ...) SPDLOG_INFO(message, ##__VA_ARGS__)
#define ST_LOG_WARNING(message, ...) SPDLOG_WARN(message, ##__VA_ARGS__)
#define ST_LOG_ERROR(message, ...) SPDLOG_ERROR(message, ##__VA_ARGS__)
#define ST_LOG_CRITICAL(message, ...) SPDLOG_CRITICAL(message, ##__VA_ARGS__)

#define ST_LOG_T(message, ...) ST_LOG_TRACE(message, ##__VA_ARGS__)
#define ST_LOG_D(message, ...) ST_LOG_DEBUG(message, ##__VA_ARGS__)
#define ST_LOG_I(message, ...) ST_LOG_INFO(message, ##__VA_ARGS__)
#define ST_LOG_W(message, ...) ST_LOG_WARNING(message, ##__VA_ARGS__)
#define ST_LOG_E(message, ...) ST_LOG_ERROR(message, ##__VA_ARGS__)
#define ST_LOG_C(message, ...) ST_LOG_CRITICAL(message, ##__VA_ARGS__)

namespace Storytime {
    typedef spdlog::level::level_enum LogLevel;

    void initialize_log(LogLevel level);

    // Set runtime log-level. This determines which log statements are actually printed at runtime.
    // This does not matter if the compile-time log-level is higher and the log statements are compiled out.
    void set_log_level(LogLevel level);

    std::string tag(const char* file_name, const char* function_name, int line_number);

    std::string tag(const std::source_location& source_location);
}
