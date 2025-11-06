#pragma once

#include "graphics/st_vulkan_context.h"

namespace Storytime {
    struct VulkanPhysicalDeviceConfig {
        VulkanContext* context = nullptr;

        const VulkanPhysicalDeviceConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(context);
            return *this;
        }
    };

    class VulkanPhysicalDevice {
    public:
        typedef VulkanPhysicalDeviceConfig Config;

    private:
        struct QueueFamilyIndices {
            std::optional<u32> graphics_family;
            std::optional<u32> present_family;
        };

    public:
        static const std::vector<const char*> enabled_extensions;

    private:
        Config config{};
        VkSurfaceKHR surface = nullptr;
        VkPhysicalDevice physical_device = nullptr;
        QueueFamilyIndices queue_family_indices{};

    public:
        VulkanPhysicalDevice() = default;

        VulkanPhysicalDevice(const Config& config);

        operator VkPhysicalDevice() const;

        u32 get_graphics_queue_family_index() const;

        u32 get_present_queue_family_index() const;

        VkResult create_device(const VkDeviceCreateInfo& device_create_info, VkDevice* device) const;

        void get_features(VkPhysicalDeviceFeatures* features) const;

        VkPhysicalDeviceFeatures get_features() const;

        void get_properties(VkPhysicalDeviceProperties* properties) const;

        VkPhysicalDeviceProperties get_properties() const;

        void get_memory_properties(VkPhysicalDeviceMemoryProperties* memory_properties) const;

        VkPhysicalDeviceMemoryProperties get_memory_properties() const;

        void get_format_properties(VkFormat format, VkFormatProperties* format_properties) const;

        VkFormatProperties get_format_properties(VkFormat format) const;

        void get_queue_family_properties(u32* queue_family_property_count, VkQueueFamilyProperties* queue_family_properties) const;

        std::vector<VkQueueFamilyProperties> get_queue_family_properties() const;

        VkResult get_extensions(u32* extension_count, VkExtensionProperties* extensions, const char* layer_name = nullptr) const;

        VkResult get_extensions(std::vector<VkExtensionProperties>* extensions, const char* layer_name = nullptr) const;

        VkResult get_present_modes(u32* present_mode_count, VkPresentModeKHR* present_modes) const;

        VkResult get_present_modes(std::vector<VkPresentModeKHR>* present_modes) const;

        VkResult get_surface_capabilities(VkSurfaceCapabilitiesKHR* surface_capabilities) const;

        VkResult get_surface_formats(u32* surface_format_count, VkSurfaceFormatKHR* surface_formats) const;

        VkResult get_surface_formats(std::vector<VkSurfaceFormatKHR>* surface_formats) const;

        VkResult get_surface_support(u32 queue_family_index, VkSurfaceKHR surface, VkBool32* supported) const;

        i32 find_supported_memory_type(const VkMemoryRequirements& memory_requirements, VkMemoryPropertyFlags required_memory_properties) const;

        VkFormat find_supported_format(const std::vector<VkFormat>& format_candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    private:
        VkPhysicalDevice pick_physical_device() const;

        u32 rate_physical_device(VkPhysicalDevice physical_device) const;

        bool has_required_extensions(VkPhysicalDevice physical_device) const;

        bool has_required_queue_family_indices(VkPhysicalDevice physical_device) const;

        bool has_required_swapchain_support(VkPhysicalDevice physical_device) const;

        bool has_required_features(VkPhysicalDevice physical_device) const;

        QueueFamilyIndices find_queue_family_indices(VkPhysicalDevice physical_device) const;

        static std::vector<const char*> get_enabled_device_extensions();

        static std::vector<VkExtensionProperties> get_available_device_extensions(VkPhysicalDevice physical_device);

        static bool has_device_extensions(VkPhysicalDevice physical_device, const std::vector<const char*>& device_extension_names);
    };
}
