#pragma once

#include "system/log.h"
#include "system/environment.h"

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
    #define ST_ASSERT(expression) \
        if (expression) {\
            /* Continue */\
        } else {\
            ST_LOG_CRITICAL("Assertion failed: [{}]", #expression); \
            ST_BREAK(); \
        }

    #define ST_ASSERT_MSG(expression, message) \
        if (expression) {\
            /* Continue */\
        } else {\
            std::stringstream ss;\
            ss << message;\
            std::string message_string = ss.str();\
            ST_LOG_CRITICAL("Assertion failed: {}, {}", #expression, message_string); \
            ST_BREAK(); \
        }
#else
    #define ST_ASSERT(expression)
    #define ST_ASSERT_MSG(expression, message)
#endif
