#include "open_gl.h"

namespace Storytime {
    OpenGL::OpenGL(const OpenGLConfig& config) {
        // GLFW must have a current context before initializing GLAD
        ST_ASSERT(config.window != nullptr, "Invalid window");
        glfwMakeContextCurrent(*config.window);

        // Initialize GLAD
        ST_LOG_TRACE("Initializing GLAD");
        bool glad_initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
        if (!glad_initialized) {
            ST_THROW("Could not initialize GLAD");
        }
        ST_LOG_DEBUG("Initialized GLAD");

#ifdef ST_DEBUG
        if (config.major_version >= 4 && config.minor_version >= 3) {
            ST_GL_CALL(glDebugMessageCallback(on_debug_message, (void*) &config.log_level));
            ST_GL_CALL(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
        }
#endif
        ST_LOG_INFO("Using renderer [{}, {}]", glGetString(GL_RENDERER), glGetString(GL_VERSION));
    }

    void OpenGL::clear_errors() {
        while (glGetError() != GL_NO_ERROR);
    }

    void OpenGL::log_errors(const std::string& tag, const std::string& function_signature) {
        uint32_t error_code;
        uint32_t error_count = 0;
        while ((error_code = glGetError()) != GL_NO_ERROR) {
            std::stringstream ss;
            ss << tag << " OpenGL error on function [{0}] - {1}";
            std::string error_message = ss.str();
            spdlog::error(error_message, function_signature, get_error_message(error_code));
            error_count++;
        }
    }

    const char* OpenGL::get_error_message(uint32_t errorCode) {
        switch (errorCode) {
            case GL_INVALID_ENUM:
                return "INVALID_ENUM";
            case GL_INVALID_VALUE:
                return "INVALID_VALUE";
            case GL_INVALID_OPERATION:
                return "INVALID_OPERATION";
            case GL_STACK_OVERFLOW:
                return "STACK_OVERFLOW";
            case GL_STACK_UNDERFLOW:
                return "STACK_UNDERFLOW";
            case GL_OUT_OF_MEMORY:
                return "OUT_OF_MEMORY";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "INVALID_FRAMEBUFFER_OPERATION";
            default:
                return "UNKNOWN";
        }
    }

    void OpenGL::on_debug_message(
        uint32_t source,
        uint32_t type,
        uint32_t id,
        uint32_t severity,
        int32_t length,
        const char* message,
        const void* user_param
    ) {
        std::string log_message = "OpenGL message [id = {}, type = {}, severity = {}, source = {}, length = {}] - {}";
        std::string message_type = get_debug_message_type(type);
        std::string message_source = get_debug_message_source(source);
        std::string message_severity = get_debug_message_severity(severity);
        LogLevel log_level = *(LogLevel*) user_param;
        if (severity == GL_DEBUG_SEVERITY_HIGH && log_level >= LogLevel::err) {
            ST_LOG_ERROR(log_message, id, message_type, message_severity, message_source, length, message);
        } else if (severity == GL_DEBUG_SEVERITY_MEDIUM && log_level >= LogLevel::warn) {
            ST_LOG_WARNING(log_message, id, message_type, message_severity, message_source, length, message);
        } else if (severity == GL_DEBUG_SEVERITY_LOW && log_level >= LogLevel::info) {
            ST_LOG_INFO(log_message, id, message_type, message_severity, message_source, length, message);
        } else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION && log_level >= LogLevel::debug) {
            ST_LOG_DEBUG(log_message, id, message_type, message_severity, message_source, length, message);
        } else {
            ST_LOG_TRACE(log_message, id, message_type, message_severity, message_source, length, message);
        }
    }

    std::string OpenGL::get_debug_message_type(uint32_t type) {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return "DEPRECATED BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "UDEFINED BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "PERFORMANCE";
            case GL_DEBUG_TYPE_OTHER:
                return "OTHER";
            case GL_DEBUG_TYPE_MARKER:
                return "MARKER";
            default:
                return "UNKNOWN";
        }
    }

    std::string OpenGL::get_debug_message_source(uint32_t source) {
        switch (source) {
            case GL_DEBUG_SOURCE_API:
                return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:
                return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:
                return "OTHER";
            default:
                return "UNKNOWN";
        }
    }

    std::string OpenGL::get_debug_message_severity(uint32_t severity) {
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                return "HIGH";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "MEDIUM";
            case GL_DEBUG_SEVERITY_LOW:
                return "LOW";
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return "NOTIFICATION";
            default:
                return "UNKNOWN";
        }
    }
}
