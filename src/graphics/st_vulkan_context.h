#pragma once

#include "window/st_window.h"

#define ST_VK_ALLOCATOR nullptr

namespace Storytime {
    struct VulkanContextConfig {
        std::string app_name;
        std::string engine_name;
        Window* window = nullptr;
        bool validation_layers_enabled = false;
    };

    class VulkanContext {
    public:
        typedef VulkanContextConfig Config;

    private:
        Config config;
        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;
        VkSurfaceKHR surface = nullptr;

    public:
        VulkanContext(const Config& config);

        ~VulkanContext();

        VkSurfaceKHR get_surface() const;

        VkResult get_physical_devices(u32* physical_device_count, VkPhysicalDevice* physical_devices) const;

        VkResult get_physical_devices(std::vector<VkPhysicalDevice>* physical_devices) const;

    private:
        void create_instance(
            const std::vector<const char*>& extensions,
            const std::vector<const char*>& validation_layers,
            const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
        );

        void destroy_instance() const;

        std::vector<const char*> get_enabled_instance_extensions() const;

        std::vector<VkExtensionProperties> get_available_instance_extensions() const;

        bool has_instance_extensions(const std::vector<const char*>& instance_extension_names) const;

        void create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info);

        void destroy_debug_messenger() const;

        VkDebugUtilsMessengerCreateInfoEXT get_debug_messenger_create_info() const;

        std::vector<const char*> get_enabled_validation_layers() const;

        std::vector<VkLayerProperties> get_available_validation_layers() const;

        bool has_validation_layers(const std::vector<const char*>& validation_layers) const;

        void create_surface();

        void destroy_surface() const;
    };
}
