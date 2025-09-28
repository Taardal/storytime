#pragma once

#include "graphics/st_vulkan.h"
#include "window/st_window.h"

namespace Storytime {
    struct VulkanInstanceConfig {
        Window* window = nullptr;
        std::string app_name;
        std::string engine_name;
        bool validation_layers_enabled = false;
    };

    class VulkanInstance {
    public:
        typedef VulkanInstanceConfig Config;

    private:
        Config config;
        VkInstance instance = nullptr;
        VkDebugUtilsMessengerEXT debug_messenger = nullptr;
        VkSurfaceKHR surface = nullptr;

    public:
        VulkanInstance(const Config& config);

        ~VulkanInstance();

        operator VkInstance() const;

        VkSurfaceKHR get_surface() const;

        std::vector<VkPhysicalDevice> get_physical_devices() const;

    private:
        void create_instance(
            const std::vector<const char*>& extensions,
            const std::vector<const char*>& validation_layers,
            const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info
        );

        void destroy_instance() const;

        std::vector<const char*> get_required_extensions() const;

        std::vector<VkExtensionProperties> get_available_extensions() const;

        bool has_extensions(const std::vector<const char*>& extensions) const;

        std::vector<const char*> get_required_validation_layers() const;

        std::vector<VkLayerProperties> get_available_validation_layers() const;

        bool has_validation_layers(const std::vector<const char*>& validation_layers) const;

        void create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT& debug_messenger_create_info);

        void destroy_debug_messenger() const;

        VkDebugUtilsMessengerCreateInfoEXT get_debug_messenger_create_info() const;

        void create_surface();

        void destroy_surface() const;
    };
}
