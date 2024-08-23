#pragma once

#include <spdlog/spdlog.h>

#define ST_FILE_NAME __FILE_NAME__
#define ST_FUNCTION_NAME __func__
#define ST_LINE_NUMBER __LINE__

#define ST_TAG() ::Storytime::tag(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER)
#define ST_TAG_MESSAGE(message) ::Storytime::tag_message(ST_FILE_NAME, ST_FUNCTION_NAME, ST_LINE_NUMBER, message)

#define ST_LOG_TRACE(message, ...) spdlog::trace(ST_TAG_MESSAGE(message), ##__VA_ARGS__)
#define ST_LOG_DEBUG(message, ...) spdlog::debug(ST_TAG_MESSAGE(message), ##__VA_ARGS__)
#define ST_LOG_INFO(message, ...) spdlog::info(ST_TAG_MESSAGE(message), ##__VA_ARGS__)
#define ST_LOG_WARNING(message, ...) spdlog::warn(ST_TAG_MESSAGE(message), ##__VA_ARGS__)
#define ST_LOG_ERROR(message, ...) spdlog::error(ST_TAG_MESSAGE(message), ##__VA_ARGS__)
#define ST_LOG_CRITICAL(message, ...) spdlog::critical(ST_TAG_MESSAGE(message), ##__VA_ARGS__)

namespace Storytime {
    typedef spdlog::level::level_enum LogLevel;

    void set_log_level(LogLevel level);

    std::string tag(const char* file_name, const char* function_name, int line_number);

    std::string tag_message(const char* file_name, const char* function_name, int line_number, std::string_view message);
}
