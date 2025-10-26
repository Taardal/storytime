#pragma once

#include <vulkan/vulkan.h>

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_VK(expression, message)\
        {\
            VkResult vk_result = expression;\
            if (vk_result == VK_SUCCESS) {\
                /* Continue */\
            } else {\
                ST_PRINT_ASSERT_ERROR(expression == VK_SUCCESS, message << " :: Vulkan result [" << get_vk_result_name(vk_result) << "] " << get_vk_result_description(vk_result));\
                ST_BREAK();\
            }\
        }
#else
    #define ST_ASSERT_VK
#endif

#define ST_ASSERT_THROW_VK(expression, message)\
    {\
        VkResult vk_result = expression;\
        if (vk_result == VK_SUCCESS) {\
            /* Continue */\
        } else {\
            ST_THROW(message << ": Vulkan call [" << #expression << "] failed with result [" << get_vk_result_name(vk_result) << "]: " << get_vk_result_description(vk_result));\
        }\
    }

#define ST_THROW_VK(vk_result, message)\
    ST_THROW(message << ": Vulkan result [" << get_vk_result_name(vk_result) << "]: " << get_vk_result_description(vk_result))

namespace Storytime {
    std::string get_vk_result_name(VkResult result);

    std::string get_vk_result_description(VkResult result);

    std::string get_vk_image_layout_name(VkImageLayout image_layout);

    std::string get_vk_image_tiling_name(VkImageTiling image_tiling);

    std::string get_vk_format_name(VkFormat format);

    VkFormat get_vk_format(std::string_view glsl_type);

    struct VulkanDepth {
        static constexpr f32 near = 0.0f;
        static constexpr f32 far = 1.0f;
    };
}
