#pragma once

#include <vulkan/vulkan.h>

#define ST_VK_ALLOCATOR nullptr

namespace Storytime {
    inline std::string append_vk_result(VkResult vk_result, std::string_view message) {
        return std::format("{}: [{}] {}", message, get_vk_result_name(vk_result), get_vk_result_description(vk_result));
    }
}

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_VK(expression, message)\
        {\
            VkResult vk_result = expression;\
            if (vk_result == VK_SUCCESS) {\
                /* Continue */\
            } else {\
                std::stringstream ss;\
                ss << message << ": [" << get_vk_result_name(vk_result) << "] " << get_vk_result_description(vk_result);\
                std::string message_string = ss.str();\
                ST_LOG_CRITICAL("Assertion failed: {} == VK_SUCCESS, {}", #expression, message_string); \
                ST_BREAK(); \
            }\
        }
#else
    #define ST_ASSERT_VK
#endif

#define ST_THROW_VK(vk_result, message)\
    ST_THROW(message << ": [" << get_vk_result_name(vk_result) << "] " << get_vk_result_description(vk_result))

#define ST_ASSERT_THROW_VK(expression, message)\
    {\
        VkResult vk_result = expression;\
        if (vk_result == VK_SUCCESS) {\
            /* Continue */\
        } else {\
            ST_THROW(message << ": [" << get_vk_result_name(vk_result) << "] " << get_vk_result_description(vk_result));\
        }\
    }
