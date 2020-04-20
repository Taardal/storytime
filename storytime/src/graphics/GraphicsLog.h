#pragma once

#include "GraphicsContext.h"

namespace storytime
{
    class GraphicsLog
    {
    public:
        static void Init(GraphicsContext* graphicsContext);

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


