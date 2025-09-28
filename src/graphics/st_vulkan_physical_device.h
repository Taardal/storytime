#pragma once

#include "graphics/st_vulkan_instance.h"

namespace Storytime {
    struct QueueFamilyIndices {
        std::optional<u32> graphics_family;
        std::optional<u32> present_family;
    };

    struct PhysicalDeviceInfo {
        VkPhysicalDevice physical_device = nullptr;
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        VkSurfaceCapabilitiesKHR surface_capabilities{};
        std::vector<VkSurfaceFormatKHR> surface_formats;
        std::vector<VkPresentModeKHR> present_modes;
        QueueFamilyIndices queue_family_indices{};
        VkFormat depth_format = VK_FORMAT_UNDEFINED;
    };

    struct VulkanPhysicalDeviceConfig {
        VulkanInstance* instance = nullptr;
    };

    class VulkanPhysicalDevice {
    public:
        typedef VulkanPhysicalDeviceConfig Config;

    private:
        Config config;
        PhysicalDeviceInfo device_info;

    public:
        VulkanPhysicalDevice(const Config& config);

        VkFormat get_depth_format() const;

        const std::vector<VkExtensionProperties>& get_extensions() const;

        const VkPhysicalDeviceFeatures& get_features() const;

        const std::vector<VkPresentModeKHR>& get_present_modes() const;

        const VkPhysicalDeviceProperties& get_properties() const;

        const QueueFamilyIndices& get_queue_family_indices() const;

        const VkSurfaceCapabilitiesKHR& get_surface_capabilities() const;

        const std::vector<VkSurfaceFormatKHR>& get_surface_formats() const;

        VkResult create_device(const VkDeviceCreateInfo& device_create_info, VkDevice* device) const;

    private:
        std::vector<const char*> get_required_extensions() const;

        PhysicalDeviceInfo get_most_suitable_physical_device(const std::vector<VkPhysicalDevice>& available_devices,const std::vector<const char*>& required_extensions) const;

        PhysicalDeviceInfo find_physical_device_info(VkPhysicalDevice physical_device, const std::vector<const char*>& required_extensions) const;

        VkFormat find_depth_format(VkPhysicalDevice physical_device) const;

        std::vector<VkExtensionProperties> find_extensions(VkPhysicalDevice physical_device, const std::vector<const char*>& required_extensions) const;

        std::vector<VkExtensionProperties> get_available_extensions(VkPhysicalDevice physical_device) const;

        VkPhysicalDeviceFeatures find_features(VkPhysicalDevice physical_device) const;

        std::vector<VkPresentModeKHR> find_present_modes(VkPhysicalDevice physical_device) const;

        VkPhysicalDeviceProperties find_properties(VkPhysicalDevice physical_device) const;

        VkSurfaceCapabilitiesKHR find_surface_capabilities(VkPhysicalDevice physical_device) const;

        std::vector<VkSurfaceFormatKHR> find_surface_formats(VkPhysicalDevice physical_device) const;

        QueueFamilyIndices find_queue_family_indices(VkPhysicalDevice physical_device) const;

        u32 get_suitability_rating(const PhysicalDeviceInfo& physical_device_info, const std::vector<const char*>& required_extensions) const;

        bool has_required_depth_format(const PhysicalDeviceInfo& physical_device_info) const;

        bool has_required_extensions(const PhysicalDeviceInfo& physical_device_info, const std::vector<const char*>& required_extensions) const;

        bool has_required_features(const PhysicalDeviceInfo& physical_device_info) const;

        bool has_required_queue_family_indices(const PhysicalDeviceInfo& physical_device_info) const;

        bool has_required_swapchain_support(const PhysicalDeviceInfo& physical_device_info) const;
    };
}
