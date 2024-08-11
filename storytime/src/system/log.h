#pragma once

#include <spdlog/spdlog.h>

#define ST_FILE_NAME __FILE_NAME__
#define ST_FUNCTION_NAME __func__
#define ST_LINE_NUMBER __LINE__

#define ST_TAGZ() ::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, "")
//#define ST_TAG_MESSAGE(message) ::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message)

#define ST_LOG_T(message, ...) spdlog::trace(::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message), ##__VA_ARGS__)
#define ST_LOG_D(message, ...) spdlog::debug(::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message), ##__VA_ARGS__)
#define ST_LOG_I(message, ...) spdlog::info(::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message), ##__VA_ARGS__)
#define ST_LOG_W(message, ...) spdlog::warn(::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message), ##__VA_ARGS__)
#define ST_LOG_E(message, ...) spdlog::error(::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message), ##__VA_ARGS__)
#define ST_LOG_C(message, ...) spdlog::critical(::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message), ##__VA_ARGS__)

#define ST_LOG_TRACE(tag, message, ...) ST_LOG_T(message, ##__VA_ARGS__)
#define ST_LOG_DEBUG(tag, message, ...) ST_LOG_D(message, ##__VA_ARGS__)
#define ST_LOG_INFO(tag, message, ...) ST_LOG_I(message, ##__VA_ARGS__)
#define ST_LOG_WARN(tag, message, ...) ST_LOG_W(message, ##__VA_ARGS__)
#define ST_LOG_ERROR(tag, message, ...) ST_LOG_E(message, ##__VA_ARGS__)
#define ST_LOG_CRITICAL(tag, message, ...) ST_LOG_C(message, ##__VA_ARGS__)

namespace Storytime {
    typedef spdlog::level::level_enum LogLevel;

    void set_log_level(LogLevel level);

    std::string tag(const char* file_name, const char* function_name, int line_number, std::string_view message);
}
