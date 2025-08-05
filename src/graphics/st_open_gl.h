#pragma once

#include "window/st_window.h"

#ifdef ST_DEBUG
    #define ST_GL_CALL(function) \
    ::Storytime::OpenGL::clear_errors(); \
    function; \
    ::Storytime::OpenGL::log_errors(#function)
#else
    #define ST_GL_CALL(function) function;
#endif

namespace Storytime {
    struct OpenGLConfig {
        Window* window = nullptr;
        LogLevel log_level;
        u32 major_version;
        u32 minor_version;
        std::string glsl_version;
    };

    class OpenGL {
    public:
        static void initialize(const OpenGLConfig& config);

        static void clear_errors();

        static void log_errors(const std::string& function_signature);

        static const char* get_error_message(u32 errorCode);

    private:
        static void on_debug_message(
            u32 source,
            u32 type,
            u32 id,
            u32 severity,
            i32 length,
            const char* message,
            const void* user_param
        );

        static std::string get_debug_message_type(u32 type);

        static std::string get_debug_message_source(u32 source);

        static std::string get_debug_message_severity(u32 severity);

    };
}
