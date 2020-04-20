#include <string>
#include <sstream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Log.h"

namespace storytime
{
    void Log::Init(LogLevel level)
    {
        spdlog::set_level(GetSpdLogLevel(level));
    }

    std::string Log::Format(std::string_view tag, std::string_view message)
    {
        std::stringstream ss;
        ss << "[" << tag << "] - " << message;
        return ss.str();
    }

    spdlog::level::level_enum Log::GetSpdLogLevel(LogLevel level)
    {
        switch (level)
        {
            case LogLevel::Critical:
                return spdlog::level::critical;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Warn:
                return spdlog::level::warn;
            case LogLevel::Info:
                return spdlog::level::info;
            case LogLevel::Debug:
                return spdlog::level::debug;
            default:
                return spdlog::level::trace;
        }
    }

}