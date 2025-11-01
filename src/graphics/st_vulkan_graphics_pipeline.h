#pragma once

#include "graphics/st_vulkan_command_buffer.h"
#include "graphics/st_vulkan_device.h"

namespace Storytime {
    struct VulkanGraphicsPipelineConfig {
        std::string name = "GraphicsPipeline";
        VulkanDevice* device = nullptr;
        VkRenderPass render_pass = nullptr;
        std::vector<VkDescriptorSetLayout> descriptor_set_layouts{};
        std::vector<VkPushConstantRange> push_constant_ranges{};
        std::vector<VkPipelineShaderStageCreateInfo> shader_stage_create_infos{};
        std::vector<VkVertexInputBindingDescription> vertex_input_binding_descriptions{};
        std::vector<VkVertexInputAttributeDescription> vertex_input_attribute_descriptions{};

        const VulkanGraphicsPipelineConfig& assert_valid() const {
            ST_ASSERT_NOT_NULL(device);
            ST_ASSERT_NOT_NULL(render_pass);
            return *this;
        }
    };

    class VulkanGraphicsPipeline {
    public:
        typedef VulkanGraphicsPipelineConfig Config;

    private:
        Config config{};
        VkPipeline pipeline = nullptr;
        VkPipelineLayout pipeline_layout = nullptr;

    public:
        VulkanGraphicsPipeline() = default;

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
