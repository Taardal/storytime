#pragma once

#if defined(NDEBUG)
    #define ST_RELEASE
#else
    #define ST_DEBUG
#endif

#if defined(_WIN32)
    #if defined(_WIN64)
        #define ST_PLATFORM_WINDOWS
    #else
        #error "Unsupported platform: Windows x86"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define ST_PLATFORM_MACOS
    #else
        #error "Unsupported Apple platform"
    #endif
#elif defined(__linux__)
    #define ST_PLATFORM_LINUX
#else
    #error "Unsupported platform"
#endif

#if defined(ST_PLATFORM_MACOS) || defined(ST_PLATFORM_LINUX)
    #define ST_PLATFORM_UNIX
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