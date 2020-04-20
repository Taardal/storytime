#pragma once

#include <memory>

#define ST_TO_STRING(value) #value

#if defined(_WIN32)
	#if defined(_WIN64)
		#define ST_PLATFORM_WINDOWS
	#else
		#error "Unsupported platform: Windows x86"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "Unsupported platform: iPhone Simulator"
    #elif TARGET_OS_IPHONE == 1
        #error "Unsupported platform: iPhone"
    #elif TARGET_OS_MAC == 1
        #define ST_PLATFORM_MACOS
    #else
        #error "Unknown platform: Apple"
    #endif
#elif defined(__ANDROID__)
    #error "Unsupported platform: Android"
#elif defined(__linux__)
    #error "Unsupported platform: Linux"
#else
	#error "Unknown platform"
#endif

#if defined(_MSC_VER)
    #define ST_COMPILER_MSVC
#elif defined(__clang__)
    #define ST_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define ST_COMPILER_GCC
#else
    #error "Unknown compiler"
#endif

#ifdef NDEBUG
    #define ST_RELEASE
#else
    #define ST_DEBUG
#endif

#ifdef ST_DEBUG
    #include "system/Log.h"
    #if __has_builtin(__debugbreak)
        #define ST_BREAK() __debugBreak()
    #elif __has_builtin(__builtin_debugtrap)
        #define ST_BREAK() __builtin_debugtrap()
    #else
        #include <csignal>
        #if defined(SIGTRAP)
            #define ST_BREAK() std::raise(SIGTRAP)
        #else
            #define ST_BREAK() std::raise(SIGABRT)
        #endif
    #endif
    #define ST_ASSERT(tag, expression) \
        if (expression) \
        {} \
        else \
        { \
            ST_LOG_ERROR(tag, "Could not assert [{0}]", ST_TO_STRING(expression)); \
            ST_BREAK(); \
        }
#else
    #define ST_BREAK()
    #define ST_ASSERT(tag, expression)
#endif

namespace storytime {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}