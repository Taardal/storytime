#include "st_vulkan_graphics_pipeline.h"

namespace Storytime {

    VulkanGraphicsPipeline::VulkanGraphicsPipeline(const Config& config) : config(config.assert_valid()) {
        create_pipeline_layout();
        create_pipeline();
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
        destroy_pipeline();
        destroy_pipeline_layout();
    }

    VulkanGraphicsPipeline::operator VkPipeline() const {
        return pipeline;
    }

    VkPipelineLayout VulkanGraphicsPipeline::get_pipeline_layout() const {
        return pipeline_layout;
    }

    void VulkanGraphicsPipeline::bind(const VulkanCommandBuffer& command_buffer, VkPipelineBindPoint pipeline_bind_point) const {
        command_buffer.bind_pipeline(pipeline_bind_point, pipeline);
    }

    void VulkanGraphicsPipeline::create_pipeline() {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = config.device->get_physical_device();
        const VkPhysicalDeviceLimits& physical_device_limits = physical_device.get_properties().limits;

        // Preflight checks

        u32 vertex_input_binding_count = config.vertex_input_binding_descriptions.size();
        u32 max_vertex_input_binding_count = physical_device_limits.maxVertexInputBindings;
        if (vertex_input_binding_count > max_vertex_input_binding_count) {
            ST_THROW("Could not create graphics pipeline [" << config.name << "] because the number of vertex input bindings [" << vertex_input_binding_count << "] exceeded the limit [" << max_vertex_input_binding_count << "]");
        }

        u32 vertex_input_attribute_count = config.vertex_input_attribute_descriptions.size();
        u32 max_vertex_input_attribute_count = physical_device_limits.maxVertexInputAttributes;
        if (vertex_input_attribute_count > max_vertex_input_attribute_count) {
            ST_THROW("Could not create graphics pipeline [" << config.name << "] because the number of vertex input attributes [" << vertex_input_attribute_count << "] exceeded the limit [" << max_vertex_input_attribute_count << "]");
        }

        // Create pipeline

        std::vector<VkDynamicState> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
        dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = (u32) dynamic_states.size();
        dynamic_state_create_info.pDynamicStates = dynamic_states.data();

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
        vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount = (u32) config.vertex_input_binding_descriptions.size();
        vertex_input_state_create_info.pVertexBindingDescriptions = config.vertex_input_binding_descriptions.data();
        vertex_input_state_create_info.vertexAttributeDescriptionCount = (u32) config.vertex_input_attribute_descriptions.size();
        vertex_input_state_create_info.pVertexAttributeDescriptions = config.vertex_input_attribute_descriptions.data();

        VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{};
        input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        viewport_state_create_info.pViewports = nullptr; // Viewport is specified as dynamic state.
        viewport_state_create_info.scissorCount = 1;
        viewport_state_create_info.pScissors = nullptr; // Scissor is specified as dynamic state.

        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
        rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable = VK_FALSE;
        rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization_state_create_info.lineWidth = 1.0f;
        rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterization_state_create_info.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisample_state_create_info{};
        multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_state_create_info.sampleShadingEnable = VK_FALSE;
        multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisample_state_create_info.minSampleShading = 1.0f;
        multisample_state_create_info.pSampleMask = nullptr;
        multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
        multisample_state_create_info.alphaToOneEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info{};
        depth_stencil_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depth_stencil_state_create_info.depthTestEnable = VK_TRUE; // Specifies if the depth of new fragments should be compared to the depth buffer to see if they should be discarded
        depth_stencil_state_create_info.depthWriteEnable = VK_TRUE; // Specifies if the new depth of fragments that pass the depth test should actually be written to the depth buffer.
        depth_stencil_state_create_info.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; // Specifies the comparison that is performed to keep or discard fragments. We are using lower depth = closer, so the depth of new fragments should be less.
        depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE; // Allows us to only keep fragments that fall within the specified depth range.
        depth_stencil_state_create_info.minDepthBounds = 0.0f; // Used for the optional depth bound test.
        depth_stencil_state_create_info.maxDepthBounds = 1.0f; // Used for the optional depth bound test.
        depth_stencil_state_create_info.stencilTestEnable = VK_FALSE; // Used to configure stencil buffer operations.
        depth_stencil_state_create_info.front = {}; // Used for the optional stencil test.
        depth_stencil_state_create_info.back = {}; // Used for the optional stencil test.

        VkPipelineColorBlendAttachmentState color_blend_attachment_state{};
        color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment_state.blendEnable = VK_TRUE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{};
        color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_create_info.logicOpEnable = VK_FALSE;
        color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount = 1;
        color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
        color_blend_state_create_info.blendConstants[0] = 0.0f;
        color_blend_state_create_info.blendConstants[1] = 0.0f;
        color_blend_state_create_info.blendConstants[2] = 0.0f;
        color_blend_state_create_info.blendConstants[3] = 0.0f;

        VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};
        graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_create_info.stageCount = (u32) config.shader_stage_create_infos.size();
        graphics_pipeline_create_info.pStages = config.shader_stage_create_infos.data();
        graphics_pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
        graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;
        graphics_pipeline_create_info.pViewportState = &viewport_state_create_info;
        graphics_pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
        graphics_pipeline_create_info.pMultisampleState = &multisample_state_create_info;
        graphics_pipeline_create_info.pDepthStencilState = &depth_stencil_state_create_info;
        graphics_pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
        graphics_pipeline_create_info.pDynamicState = &dynamic_state_create_info;
        graphics_pipeline_create_info.layout = pipeline_layout;
        graphics_pipeline_create_info.renderPass = config.render_pass;
        graphics_pipeline_create_info.subpass = 0;
        graphics_pipeline_create_info.basePipelineHandle = nullptr;
        graphics_pipeline_create_info.basePipelineIndex = -1;

        ST_ASSERT_THROW_VK(
            device.create_graphics_pipeline(graphics_pipeline_create_info, &pipeline, config.name),
            "Could not create graphics pipeline [" << config.name << "]"
        );
    }

    void VulkanGraphicsPipeline::destroy_pipeline() const {
        config.device->destroy_graphics_pipeline(pipeline);
    }

    void VulkanGraphicsPipeline::create_pipeline_layout() {
        const VulkanDevice& device = *config.device;
        const VulkanPhysicalDevice& physical_device = config.device->get_physical_device();
        const VkPhysicalDeviceLimits& physical_device_limits = physical_device.get_properties().limits;

        std::string pipeline_layout_name = std::format("{} layout", config.name.c_str());

        u32 bound_descriptor_set_count = config.descriptor_set_layouts.size();
        u32 max_bound_descriptor_set_count = physical_device_limits.maxBoundDescriptorSets;
        if (bound_descriptor_set_count > max_bound_descriptor_set_count) {
            ST_THROW("Could not create graphics pipeline layout [" << pipeline_layout_name << "] because the number of simultaneously bound descriptor sets [" << bound_descriptor_set_count << "] exceeded the limit [" << max_bound_descriptor_set_count << "]");
        }

        VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = (u32) bound_descriptor_set_count;
        pipeline_layout_create_info.pSetLayouts = config.descriptor_set_layouts.data();
        pipeline_layout_create_info.pushConstantRangeCount = (u32) config.push_constant_ranges.size();
        pipeline_layout_create_info.pPushConstantRanges = config.push_constant_ranges.data();

        ST_ASSERT_THROW_VK(
            device.create_pipeline_layout(pipeline_layout_create_info, &pipeline_layout, pipeline_layout_name),
            "Could not create graphics pipeline layout [" << pipeline_layout_name << "]"
        );
    }

    void VulkanGraphicsPipeline::destroy_pipeline_layout() const {
        config.device->destroy_pipeline_layout(pipeline_layout);
    }
}
