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
    #define ST_PLATFORM_MACOS
#elif defined(__linux__) || defined(__unix__)
    #define ST_PLATFORM_LINUX
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