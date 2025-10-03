#pragma once

#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_swapchain.h"

namespace Storytime {
    struct VulkanGraphicsPipelineConfig {
        VulkanDevice* device = nullptr;
        VulkanSwapchain* swapchain = nullptr;
        std::string name = "Pipeline";
        std::filesystem::path vertex_shader_path;
        std::filesystem::path fragment_shader_path;
        VkVertexInputBindingDescription vertex_input_binding_description{};
        std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions{};
        std::vector<VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings{};
    };

    class VulkanGraphicsPipeline {
    public:
        typedef VulkanGraphicsPipelineConfig Config;

    private:
        Config config;
        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipeline_layout = nullptr;
        VkDescriptorSetLayout descriptor_set_layout = nullptr;

    public:
        VulkanGraphicsPipeline(const Config& config);

        ~VulkanGraphicsPipeline();

        operator VkPipeline() const;

        VkPipelineLayout get_pipeline_layout() const;

        VkDescriptorSetLayout get_descriptor_set_layout() const;

        void bind(const VulkanCommandBuffer& command_buffer, VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS) const;

    private:
        void create_pipeline();

        void destroy_pipeline() const;

        VkShaderModule create_shader_module(const std::filesystem::path& path) const;

        void destroy_shader_module(VkShaderModule shader_module) const;
    };
}
