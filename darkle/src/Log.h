#pragma once

#include <spdlog/spdlog.h>

#define TAG typeid(*this).name()
#define TAG_TYPE(type) typeid(type).name()
#define LOG_TRACE(tag, message, ...) Darkle::Log::trace(Darkle::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define LOG_DEBUG(tag, message, ...) Darkle::Log::debug(Darkle::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define LOG_INFO(tag, message, ...) Darkle::Log::info(Darkle::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define LOG_WARN(tag, message, ...) Darkle::Log::warn(Darkle::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define LOG_ERROR(tag, message, ...) Darkle::Log::error(Darkle::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)
#define LOG_CRITICAL(tag, message, ...) Darkle::Log::critical(Darkle::Log::getPrettyMessage(tag, message, __func__, __LINE__), __VA_ARGS__)

namespace Darkle {

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
    private:
        Log() = default;

        ~Log() = default;

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


