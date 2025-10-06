#include "st_vulkan_graphics_pipeline.h"

#include "system/st_file_reader.h"

namespace Storytime {
    VulkanGraphicsPipeline::VulkanGraphicsPipeline(const Config& config) : config(config) {
        create_pipeline();
    }

    VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
        destroy_pipeline();
    }

    VulkanGraphicsPipeline::operator VkPipeline() const {
        return pipeline;
    }

    VkPipelineLayout VulkanGraphicsPipeline::get_pipeline_layout() const {
        return pipeline_layout;
    }

    VkDescriptorSetLayout VulkanGraphicsPipeline::get_descriptor_set_layout() const {
        return descriptor_set_layout;
    }

    void VulkanGraphicsPipeline::bind(const VulkanCommandBuffer& command_buffer, VkPipelineBindPoint pipeline_bind_point) const {
        command_buffer.bind_pipeline(pipeline_bind_point, pipeline);
    }

    void VulkanGraphicsPipeline::create_pipeline() {
        const VulkanDevice& device = *config.device;

        //
        // Programmable stages
        //

        VkShaderModule vertex_shader = create_shader_module(config.vertex_shader_path);
        VkShaderModule fragment_shader = create_shader_module(config.fragment_shader_path);

        std::string vertex_shader_name = std::format("{} vertex shader [{}]", config.name.c_str(), config.vertex_shader_path.c_str());
        if (device.set_object_name(vertex_shader, VK_OBJECT_TYPE_SHADER_MODULE, vertex_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set pipeline vertex shader name [" << vertex_shader_name << "]");
        }
        std::string fragment_shader_name = std::format("{} fragment shader [{}]", config.name.c_str(), config.fragment_shader_path.c_str());
        if (device.set_object_name(fragment_shader, VK_OBJECT_TYPE_SHADER_MODULE, fragment_shader_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set pipeline fragment shader name [" << fragment_shader_name << "]");
        }

        VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info{};
        vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_shader_stage_create_info.module = vertex_shader;
        vertex_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info{};
        fragment_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_shader_stage_create_info.module = fragment_shader;
        fragment_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo shader_stage_create_infos[] = {
            vertex_shader_stage_create_info,
            fragment_shader_stage_create_info,
        };

        //
        // Fixed function stages
        //

        std::vector<VkDynamicState> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
        dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = static_cast<u32>(dynamic_states.size());
        dynamic_state_create_info.pDynamicStates = dynamic_states.data();

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
        vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
        vertex_input_state_create_info.pVertexBindingDescriptions = &config.vertex_input_binding_description;
        vertex_input_state_create_info.vertexAttributeDescriptionCount = (u32) config.vertex_input_attribute_descriptions.size();
        vertex_input_state_create_info.pVertexAttributeDescriptions = config.vertex_input_attribute_descriptions.data();

        VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{};
        input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        // const VkExtent2D& swapchain_image_extent = config.swapchain->get_image_extent();
        //
        // VkViewport viewport{};
        // viewport.x = 0.0f;
        // viewport.y = 0.0f;
        // viewport.width = (f32) swapchain_image_extent.width;
        // viewport.height = (f32) swapchain_image_extent.height;
        // viewport.minDepth = 0.0f;
        // viewport.maxDepth = 1.0f;
        //
        // VkRect2D scissor{};
        // scissor.offset = {0, 0};
        // scissor.extent = swapchain_image_extent;
        //
        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        // viewport_state_create_info.pViewports = &viewport;
        viewport_state_create_info.scissorCount = 1;
        // viewport_state_create_info.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
        rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable = VK_FALSE;
        rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization_state_create_info.lineWidth = 1.0f;
        rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterization_state_create_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
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
        depth_stencil_state_create_info.depthCompareOp = VK_COMPARE_OP_LESS; // Specifies the comparison that is performed to keep or discard fragments. We are using lower depth = closer, so the depth of new fragments should be less.
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

        //
        // Creation
        //

        VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{};
        descriptor_set_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptor_set_layout_create_info.bindingCount = config.descriptor_set_layout_bindings.size();
        descriptor_set_layout_create_info.pBindings = config.descriptor_set_layout_bindings.data();

        if (device.create_descriptor_set_layout(descriptor_set_layout_create_info, &descriptor_set_layout) != VK_SUCCESS) {
            ST_THROW("Could not create graphics pipeline descriptor set layout");
        }

        VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = 1;
        pipeline_layout_create_info.pSetLayouts = &descriptor_set_layout;
        pipeline_layout_create_info.pushConstantRangeCount = 0;
        pipeline_layout_create_info.pPushConstantRanges = nullptr;

        if (device.create_pipeline_layout(pipeline_layout_create_info, &pipeline_layout) != VK_SUCCESS) {
            ST_THROW("Could not create graphics pipeline layout");
        }

        std::string pipeline_layout_name = std::format("{} Layout", config.name.c_str());
        if (device.set_object_name(pipeline_layout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, pipeline_layout_name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set graphics pipeline layout name [" << pipeline_layout_name << "]");
        }

        VkGraphicsPipelineCreateInfo graphics_pipeline_create_info{};
        graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphics_pipeline_create_info.stageCount = 2;
        graphics_pipeline_create_info.pStages = shader_stage_create_infos;
        graphics_pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
        graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;
        graphics_pipeline_create_info.pViewportState = &viewport_state_create_info;
        graphics_pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
        graphics_pipeline_create_info.pMultisampleState = &multisample_state_create_info;
        graphics_pipeline_create_info.pDepthStencilState = &depth_stencil_state_create_info;
        graphics_pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
        graphics_pipeline_create_info.pDynamicState = &dynamic_state_create_info;
        graphics_pipeline_create_info.layout = pipeline_layout;
        graphics_pipeline_create_info.renderPass = config.swapchain->get_render_pass();
        graphics_pipeline_create_info.subpass = 0;
        graphics_pipeline_create_info.basePipelineHandle = nullptr;
        graphics_pipeline_create_info.basePipelineIndex = -1;

        if (device.create_graphics_pipeline(graphics_pipeline_create_info, &pipeline) != VK_SUCCESS) {
            ST_THROW("Could not create graphics pipeline");
        }

        if (device.set_object_name(pipeline, VK_OBJECT_TYPE_PIPELINE, config.name.c_str()) != VK_SUCCESS) {
            ST_THROW("Could not set graphics pipeline name [" << config.name << "]");
        }

        //
        // Shader modules are no longer needed after the graphics pipeline has been created.
        //
        // In Vulkan, VkShaderModule objects are really just containers for SPIR-V bytecode. When we create the graphics pipeline,
        // the driver reads and compiles the SPIR-V bytecode into its internal pipeline representation. After that, the modules are
        // no longer referenced.
        //
        destroy_shader_module(fragment_shader);
        destroy_shader_module(vertex_shader);
    }

    void VulkanGraphicsPipeline::destroy_pipeline() const {
        config.device->destroy_descriptor_set_layout(descriptor_set_layout);
        config.device->destroy_pipeline_layout(pipeline_layout);
        config.device->destroy_graphics_pipeline(pipeline);
    }

    VkShaderModule VulkanGraphicsPipeline::create_shader_module(const std::filesystem::path& path) const {
        std::vector<char> shader_bytes = FileReader::read_bytes(path);
        if (shader_bytes.empty()) {
            ST_THROW("Could not read graphics pipeline shader [" << path << "]");
        }

        VkShaderModuleCreateInfo shader_module_create_info{};
        shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.codeSize = shader_bytes.size();
        shader_module_create_info.pCode = (const u32*) shader_bytes.data();

        VkShaderModule shader_module;
        if (config.device->create_shader_module(shader_module_create_info, &shader_module) != VK_SUCCESS) {
            ST_THROW("Could not create graphics shader module");
        }
        return shader_module;
    }

    void VulkanGraphicsPipeline::destroy_shader_module(VkShaderModule shader_module) const {
        config.device->destroy_shader_module(shader_module);
    }
}
