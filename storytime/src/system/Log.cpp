#include <string>
#include <sstream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Log.h"

namespace storytime {

    void Log::setLevel(LogLevel level) {
        spdlog::set_level(getSpdLogLevel(level));
    }

    std::string Log::getPrettyMessage(std::string_view tag, std::string_view message, std::string_view functionName, int lineNumber) {
        std::stringstream ss;
        ss << "[" << tag << ":" << functionName << ":" << lineNumber << "] - " << message;
        return ss.str();
    }

    spdlog::level::level_enum Log::getSpdLogLevel(LogLevel level) {
        switch (level) {
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