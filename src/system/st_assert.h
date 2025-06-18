#pragma once

#include "system/st_log.h"
#include "system/st_environment.h"

#ifdef ST_DEBUG
    #define ST_ENABLE_BREAK
    #define ST_ENABLE_ASSERT
#endif

#ifdef ST_ENABLE_BREAK
    #if defined(ST_COMPILER_MSVC)
        #define ST_BREAK() __debugbreak()
    #elif defined(ST_COMPILER_CLANG) || defined(ST_COMPILER_GCC)
        #define ST_BREAK() __builtin_trap()
    #else
        #error "ST_BREAK() not implemented for this compiler"
    #endif
#else
    #define ST_BREAK()
#endif

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT(expression, message) \
        if (expression) {\
            /* Continue */\
        } else {\
            std::stringstream ss;\
            ss << message;\
            std::string message_string = ss.str();\
            ST_LOG_CRITICAL("Assertion failed: {}, {}", #expression, message_string); \
            ST_BREAK(); \
        }

    #define ST_ASSERT_NOT_NULL(pointer) \
        ST_ASSERT(pointer != nullptr, "Pointer [" << #pointer << "] cannot be null")

    #define ST_ASSERT_NOT_EMPTY(value) \
        ST_ASSERT(!value.empty(), "Value [" << #value << "] cannot be empty")

    #define ST_ASSERT_NOT_CEMPTY(value) \
        ST_ASSERT(strlen(value) > 0, "Value [" << #value << "] cannot be empty")
#else
    #define ST_ASSERT(expression, message)
    #define ST_ASSERT_NOT_NULL(pointer)
    #define ST_ASSERT_NOT_EMPTY(value)
    #define ST_ASSERT_NOT_CEMPTY(value)
#endif
