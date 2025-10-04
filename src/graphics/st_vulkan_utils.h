#pragma once

#include <vulkan/vulkan.h>

namespace Storytime {
    std::string get_vk_result_name(VkResult result);

    std::string get_vk_result_description(VkResult result);

    std::string get_vk_image_layout_name(VkImageLayout layout);

    VkFormat get_vk_format(std::string_view glsl_type);
}
