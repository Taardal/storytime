#pragma once

#include "graphics/st_vulkan_instance.h"

namespace Storytime {
    struct VulkanSurfaceConfig {
        Window* window = nullptr;
        VulkanInstance* instance = nullptr;
    };

    class VulkanSurface {
    public:
        typedef VulkanSurfaceConfig Config;

    private:
        Config config;
        VkSurfaceKHR surface = nullptr;

    public:
        VulkanSurface(const Config& config);

        ~VulkanSurface();

        operator VkSurfaceKHR() const;
    };
}
