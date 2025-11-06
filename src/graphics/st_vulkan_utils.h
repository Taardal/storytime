#pragma once

#include <vulkan/vulkan.h>

#ifdef ST_ENABLE_ASSERT
    #define ST_ASSERT_VK(expression, message)\
        {\
            VkResult vk_result = expression;\
            if (vk_result == VK_SUCCESS) {\
                /* Continue */\
            } else {\
                ST_PRINT_ASSERT_ERROR(expression == VK_SUCCESS, message << " :: " << ::Storytime::format_vk_result(vk_result));\
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
            ST_THROW(message << ": Vulkan call [" << #expression << "] failed: " << ::Storytime::format_vk_result(vk_result));\
        }\
    }

namespace Storytime {
    std::string format_vk_result(VkResult result);

    std::string get_vk_result_name(VkResult result);

    std::string get_vk_result_description(VkResult result);

    std::string get_vk_object_type_name(VkObjectType object_type);

    std::string get_vk_image_layout_name(VkImageLayout image_layout);

    std::string get_vk_image_tiling_name(VkImageTiling image_tiling);

    std::string get_vk_format_name(VkFormat format);

    VkFormat get_vk_format(std::string_view glsl_type);

    template<typename T>
    constexpr VkIndexType get_vk_index_type() {
        if constexpr (std::is_same_v<T, uint32_t>) {
            return VK_INDEX_TYPE_UINT32;
        } else if constexpr (std::is_same_v<T, uint16_t>) {
            return VK_INDEX_TYPE_UINT16;
        } else if constexpr (std::is_same_v<T, uint8_t>) {
            return VK_INDEX_TYPE_UINT8_EXT; // Requires extension VK_EXT_index_type_uint8
        } else {
            static_assert(false, "Unsupported index type. Only uint32_t, uint16_t (and optionally uint8_t) are valid.");
            return VK_INDEX_TYPE_MAX_ENUM;
        }
    }
}
