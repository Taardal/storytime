#pragma once

#include <memory>

#ifdef _WIN32
	#ifdef _WIN64
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

#define ST_TO_STRING(value) #value

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