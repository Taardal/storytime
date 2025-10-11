#pragma once

#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_swapchain.h"

namespace Storytime {
    struct VulkanGraphicsPipelineConfig {
        std::string name = "Pipeline";
        VulkanDevice* device = nullptr;
        VkRenderPass render_pass = nullptr;
        std::vector<VkDescriptorSetLayout> descriptor_set_layouts{};
        std::vector<VkPushConstantRange> push_constant_ranges{};
        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos{};
        std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions{};
        std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions{};
    };

    class VulkanGraphicsPipeline {
    public:
        typedef VulkanGraphicsPipelineConfig Config;

    private:
        Config config;
        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipeline_layout = nullptr;

    public:
        VulkanGraphicsPipeline(const Config& config);

        ~VulkanGraphicsPipeline();

        operator VkPipeline() const;

        VkPipelineLayout get_pipeline_layout() const;

        void bind(const VulkanCommandBuffer& command_buffer, VkPipelineBindPoint pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS) const;

    private:
        void create_pipeline();

        void destroy_pipeline() const;

        void create_pipeline_layout();

        void destroy_pipeline_layout() const;
    };
}
