#pragma once

#include <vulkan/vulkan.h>

#define ST_VK_ALLOCATOR nullptr

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

namespace Storytime {
    std::string get_vk_result_name(VkResult result);

    std::string get_vk_result_description(VkResult result);

    std::string get_vk_image_layout_name(VkImageLayout layout);

    VkFormat get_vk_format(std::string_view glsl_type);
}
