#pragma once

#include <spdlog/spdlog.h>

#define ST_TAG typeid(*this).name()
#define ST_TAG_TYPE(type) typeid(type).name()
#define ST_TRACE(tag, message, ...) ::storytime::Log::trace(::storytime::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define ST_DEBUG(tag, message, ...) ::storytime::Log::debug(::storytime::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define ST_INFO(tag, message, ...) ::storytime::Log::info(::storytime::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define ST_WARN(tag, message, ...) ::storytime::Log::warn(::storytime::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define ST_ERROR(tag, message, ...) ::storytime::Log::error(::storytime::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define ST_CRITICAL(tag, message, ...) ::storytime::Log::critical(::storytime::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)

namespace storytime {

    enum class LogLevel {
        None = 0,
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    class Log {
    public:
        static void setLevel(LogLevel level);

        static std::string getPrettyMessage(std::string_view tag, std::string_view message, std::string_view functionName, int lineNumber);

        template<typename... T>
        static void trace(std::string_view message, const T& ... args) {
            spdlog::trace(message, args...);
        }

        template<typename... T>
        static void debug(std::string_view message, const T& ... args) {
            spdlog::debug(message, args...);
        }

        template<typename... T>
        static void info(std::string_view message, const T& ... args) {
            spdlog::info(message, args...);
        }

        template<typename... T>
        static void warn(std::string_view message, const T& ... args) {
            spdlog::warn(message, args...);
        }

        template<typename... T>
        static void error(std::string_view message, const T& ... args) {
            spdlog::error(message, args...);
        }

        template<typename... T>
        static void critical(std::string_view message, const T& ... args) {
            spdlog::critical(message, args...);
        }

    private:
        static spdlog::level::level_enum getSpdLogLevel(LogLevel level);
    };

}


