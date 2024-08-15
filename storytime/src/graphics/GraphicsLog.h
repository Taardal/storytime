#pragma once

#include "GraphicsContext.h"

#ifdef ST_DEBUG
    #define ST_GL_CALL(function) \
            ::Storytime::GraphicsLog::ClearErrors(); \
            function; \
            ::Storytime::GraphicsLog::LogErrors(ST_TAG(), #function)
#else
    #define ST_GL_CALL(tag, function) function;
#endif

namespace Storytime
{
    class GraphicsLog
    {
    public:
        static void Init(const GraphicsContext::Config& graphicsConfig);

        static void ClearErrors();

        static void LogErrors(std::string_view tag, std::string_view functionSignature);

        static const char* GetErrorMessage(uint32_t errorCode);

    private:
        static void OnDebugMessage(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam);

        static const char* GetDebugMessageType(uint32_t type);

        static const char* GetDebugMessageSource(uint32_t source);

        static const char* GetDebugMessageSeverity(uint32_t severity);
    };
}