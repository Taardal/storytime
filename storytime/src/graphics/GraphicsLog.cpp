#include "system/Log.h"
#include "system/Core.h"
#include "GraphicsLog.h"

namespace storytime
{
    void GraphicsLog::Init(GraphicsContext* graphicsContext)
    {
#ifdef ST_DEBUG
        uint32_t versionMajor = graphicsContext->getConfig().VersionMajor;
        uint32_t versionMinor = graphicsContext->getConfig().VersionMinor;
        if (versionMajor >= 4 && versionMinor >= 3)
        {
            const void* userParam = nullptr;
            ST_GL_CALL(ST_TAG_TYPE(GraphicsLog), glDebugMessageCallback(OnDebugMessage, userParam));
            ST_GL_CALL(ST_TAG_TYPE(GraphicsLog), glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
        }
#endif
    }

    void GraphicsLog::ClearErrors()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    void GraphicsLog::LogErrors(std::string_view tag, std::string_view functionSignature)
    {
        uint32_t errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            ST_LOG_ERROR(tag, "OpenGL error on function [{0}] - {1}", functionSignature, GetErrorMessage(errorCode));
            ST_BREAK();
        }
    }

    const char* GraphicsLog::GetErrorMessage(uint32_t errorCode)
    {
        switch (errorCode)
        {
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

    void GraphicsLog::OnDebugMessage(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
    {
        const std::string& tag = ST_TAG_TYPE(GraphicsLog);
        const char* logMessage = "OpenGL message [id = {0}, type = {1}, severity = {2}, source = {3}, length = {4}] - {5}";
        const char* messageType = GetDebugMessageType(type);
        const char* messageSource = GetDebugMessageSource(source);
        const char* messageSeverity = GetDebugMessageSeverity(severity);
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                ST_LOG_ERROR(tag, logMessage, id, messageType, messageSeverity, messageSource, length, message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                ST_LOG_WARN(tag, logMessage, id, messageType, messageSeverity, messageSource, length, message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                ST_LOG_INFO(tag, logMessage, id, messageType, messageSeverity, messageSource, length, message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                ST_LOG_DEBUG(tag, logMessage, id, messageType, messageSeverity, messageSource, length, message);
                break;
            default:
                ST_LOG_TRACE(tag, logMessage, id, messageType, messageSeverity, messageSource, length, message);
                break;
        }
    }

    const char* GraphicsLog::GetDebugMessageType(uint32_t type)
    {
        switch (type)
        {
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

    const char* GraphicsLog::GetDebugMessageSource(uint32_t source)
    {
        switch (source)
        {
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

    const char* GraphicsLog::GetDebugMessageSeverity(uint32_t severity)
    {
        switch (severity)
        {
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