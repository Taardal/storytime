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
    #define ST_PRINT_ASSERT_ERROR(expression, message)\
        std::string tag = ST_TAG;\
        std::stringstream ss;\
        ss << message;\
        std::string message_string = ss.str();\
        fprintf(stderr, "--------------------------------------------------------------------------------------------------------------\n");\
        fprintf(stderr, "[ASSERT] %s\n", message_string.c_str());\
        fprintf(stderr, "--------------------------------------------------------------------------------------------------------------\n");\
        fprintf(stderr, "- Assertion: %s\n", #expression);\
        fprintf(stderr, "- Tag: %s\n", tag.c_str());\
        fprintf(stderr, "\n");

    #define ST_ASSERT(expression, message)\
        if (expression) {\
            /* Continue */\
        } else {\
            ST_PRINT_ASSERT_ERROR(expression, message);\
            ST_BREAK();\
        }

    #define ST_ASSERT_NOT_NULL(pointer)\
        ST_ASSERT(pointer != nullptr, "Pointer [" << #pointer << "] cannot be null")

    #define ST_ASSERT_NOT_EMPTY(value)\
        ST_ASSERT(!value.empty(), "Value [" << #value << "] cannot be empty")

    #define ST_ASSERT_NOT_CEMPTY(value)\
        ST_ASSERT(strlen(value) > 0, "Value [" << #value << "] cannot be empty")

    #define ST_ASSERT_IN_BOUNDS(index, collection)\
        ST_ASSERT(index >= 0 && index < collection.size(), "Index [" << #index << " = " << index << "] must be inside collection bounds [" #collection << " = 0.." << collection.size() << "].");

    #define ST_ASSERT_GREATER_THAN_ZERO(value)\
        ST_ASSERT(value > 0, "Value [" << #value << "] must be greater than zero")

#else
    #define ST_PRINT_ASSERT_ERROR(expression, message)
    #define ST_ASSERT(expression, message)
    #define ST_ASSERT_NOT_NULL(pointer)
    #define ST_ASSERT_NOT_EMPTY(value)
    #define ST_ASSERT_NOT_CEMPTY(value)
    #define ST_ASSERT_IN_BOUNDS(index, collection)
    #define ST_ASSERT_GREATER_THAN_ZERO(value)
#endif
