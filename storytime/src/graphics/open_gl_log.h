#pragma once

#include "open_gl.h"

#ifdef ST_DEBUG
#define ST_GL_CALL(function) \
            ::Storytime::OpenGLLog::clear_errors(); \
            function; \
            ::Storytime::OpenGLLog::log_errors(ST_TAG(), #function)
#else
    #define ST_GL_CALL(tag, function) function;
#endif

namespace Storytime {
    struct OpenGLLogConfig {
        LogLevel log_level = LogLevel::trace;
        u32 version_major = 0;
        u32 version_minor = 0;
    };

    class OpenGLLog {
    public:
        static void initialize(const OpenGLLogConfig& config);

        static void clear_errors();

        static void log_errors(const std::string& tag, const std::string& function_signature);

        static const char* get_error_message(uint32_t errorCode);

    private:
        static void on_debug_message(
            uint32_t source,
            uint32_t type,
            uint32_t id,
            uint32_t severity,
            int32_t length,
            const char* message,
            const void* user_param
        );

        static std::string get_debug_message_type(uint32_t type);

        static std::string get_debug_message_source(uint32_t source);

        static std::string get_debug_message_severity(uint32_t severity);
    };
}
