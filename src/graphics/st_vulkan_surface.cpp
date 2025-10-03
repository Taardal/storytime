#include "graphics/st_vulkan_surface.h"

namespace Storytime {
    VulkanSurface::VulkanSurface(const Config& config) : config(config) {
        // if (config.instance->create_surface(&surface) != VK_SUCCESS) {
        //     ST_THROW("Could not create Vulkan surface");
        // }
    }

    VulkanSurface::~VulkanSurface() {
        // config.instance->destroy_surface(surface);
    }

    VulkanSurface::operator VkSurfaceKHR() const {
        return surface;
    }
}
