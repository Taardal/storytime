#pragma once

#include "Core.h"
#include "Tag.h"
#include "graphics/GraphicsLog.h"
#include <spdlog/spdlog.h>

#define ST_LOG_TRACE(message, ...) ::storytime::Log::Trace(::storytime::Log::Format(ST_TAG, message), ##__VA_ARGS__)
#define ST_LOG_DEBUG(message, ...) ::storytime::Log::Debug(::storytime::Log::Format(ST_TAG, message), ##__VA_ARGS__)
#define ST_LOG_INFO(message, ...) ::storytime::Log::Info(::storytime::Log::Format(ST_TAG, message), ##__VA_ARGS__)
#define ST_LOG_WARN(message, ...) ::storytime::Log::Warn(::storytime::Log::Format(ST_TAG, message), ##__VA_ARGS__)
#define ST_LOG_ERROR(message, ...) ::storytime::Log::Error(::storytime::Log::Format(ST_TAG, message), ##__VA_ARGS__)
#define ST_LOG_CRITICAL(message, ...) ::storytime::Log::Critical(::storytime::Log::Format(ST_TAG, message), ##__VA_ARGS__)

namespace storytime
{
    enum class LogLevel
    {
        None = 0,
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    class Log
    {
    public:
        static void Init(LogLevel level);

        template<typename... T>
        static void Trace(std::string_view message, const T& ... args)
        {
            spdlog::trace(message, args...);
        }

        template<typename... T>
        static void Debug(std::string_view message, const T& ... args)
        {
            spdlog::debug(message, args...);
        }

        template<typename... T>
        static void Info(std::string_view message, const T& ... args)
        {
            spdlog::info(message, args...);
        }

        template<typename... T>
        static void Warn(std::string_view message, const T& ... args)
        {
            spdlog::warn(message, args...);
        }

        template<typename... T>
        static void Error(std::string_view message, const T& ... args)
        {
            spdlog::error(message, args...);
        }

        template<typename... T>
        static void Critical(std::string_view message, const T& ... args)
        {
            spdlog::critical(message, args...);
        }

        static std::string Format(std::string_view tag, std::string_view message);

    private:
        static spdlog::level::level_enum GetSpdLogLevel(LogLevel level);
    };

}


