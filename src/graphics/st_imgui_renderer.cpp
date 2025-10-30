#include "st_imgui_renderer.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace Storytime {

    ImGuiRenderer::ImGuiRenderer(const ImGuiRendererConfig& config) : config(config.assert_valid()) {
        ST_ASSERT(IMGUI_CHECKVERSION(), "Invalid ImGui version");
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Docking: https://github.com/ocornut/imgui/wiki/Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Multi Viewports: https://github.com/ocornut/imgui/wiki/Multi-Viewports

        ST_ASSERT(config.window != nullptr, "Invalid window");
        constexpr bool glfw_callbacks_enabled = true;
        ImGui_ImplGlfw_InitForVulkan(*config.window, glfw_callbacks_enabled);

        ImGui_ImplVulkan_InitInfo vulkan_init_info{};
        vulkan_init_info.ApiVersion = config.api_version;
        vulkan_init_info.Instance = *config.context;
        vulkan_init_info.PhysicalDevice = *config.physical_device;
        vulkan_init_info.Device = *config.device;
        vulkan_init_info.QueueFamily = config.physical_device->get_graphics_queue_family_index();
        vulkan_init_info.Queue = config.device->get_graphics_queue();
        vulkan_init_info.DescriptorPoolSize = IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE;
        vulkan_init_info.MinImageCount = config.frame_count;
        vulkan_init_info.ImageCount = config.swapchain->get_image_count();
        vulkan_init_info.PipelineInfoMain.RenderPass = config.swapchain->get_render_pass();
        vulkan_init_info.PipelineInfoMain.Subpass = 0;
        vulkan_init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        vulkan_init_info.PipelineCache = nullptr;
        vulkan_init_info.Allocator = nullptr;
        vulkan_init_info.CheckVkResultFn = on_check_vk_result;

        ImGui_ImplVulkan_Init(&vulkan_init_info);

        if (!config.settings_file_path.empty() && !std::filesystem::exists("imgui.ini")) {
            ImGui::LoadIniSettingsFromDisk(config.settings_file_path.c_str());
        }

        ST_LOG_DEBUG("Initialized ImGui");
    }

    ImGuiRenderer::~ImGuiRenderer() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ST_LOG_DEBUG("Terminated ImGui");
    }

    void ImGuiRenderer::begin_render() const {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // If ImGui wants mouse/keyboard focus, we need to disable the application's mouse/keyboard input.
        // For example, if you type "WASD" in an ImGui text field, the player in the game should not move.
        ImGuiIO& io = ImGui::GetIO();
        config.mouse->set_enabled(!io.WantCaptureMouse);
        config.keyboard->set_enabled(!io.WantCaptureKeyboard);
    }

    void ImGuiRenderer::render() const {
        // Takes all the ImGui widgets created during this frame (ImGui::Begin, ImGui::Button, etc.) and builds the final draw command lists.
        ImGui::Render();
    }

    void ImGuiRenderer::end_render(VkCommandBuffer command_buffer) const {
        // Takes all the ImGui widgets created during this frame (ImGui::Begin, ImGui::Button, etc.) and builds the final draw command lists.
        ImGui::Render();

        // Takes the baked data from ImGui::Render() and records Vulkan commands into the command buffer.
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), command_buffer);

        i32 multiple_viewports_enabled = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
        if (!multiple_viewports_enabled) {
            return;
        }

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void ImGuiRenderer::end_frame() const {
        i32 multiple_viewports_enabled = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
        if (!multiple_viewports_enabled) {
            return;
        }

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void ImGuiRenderer::on_check_vk_result(VkResult vk_result) {
        if (vk_result != VK_SUCCESS) {
            ST_THROW_VK(vk_result, "Could not initialize ImGuiRenderer");
        }
    }

    void ImGuiRenderer::create_descriptor_pool() {
        VkDescriptorPoolSize descriptor_pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
        };

        VkDescriptorPoolCreateInfo descriptor_pool_create_info{};
        descriptor_pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptor_pool_create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        descriptor_pool_create_info.poolSizeCount = (u32) IM_ARRAYSIZE(descriptor_pool_sizes);
        descriptor_pool_create_info.pPoolSizes = descriptor_pool_sizes;
        descriptor_pool_create_info.maxSets = 0;
        for (VkDescriptorPoolSize& descriptor_pool_size : descriptor_pool_sizes) {
            descriptor_pool_create_info.maxSets += descriptor_pool_size.descriptorCount;
        }

        std::string descriptor_pool_name = std::format("{} descriptor pool", config.name);

        ST_ASSERT_THROW_VK(
            config.device->create_descriptor_pool(descriptor_pool_create_info, &descriptor_pool),
            "Could not create descriptor pool [" << descriptor_pool_name << "]"
        );

        ST_ASSERT_THROW_VK(
            config.device->set_object_name(descriptor_pool, VK_OBJECT_TYPE_DESCRIPTOR_POOL, descriptor_pool_name.c_str()),
            "Could not set descriptor pool name [" << descriptor_pool_name << "]"
        );
    }

    void ImGuiRenderer::destroy_descriptor_pool() const {
        const VulkanDevice& device = *config.device;
        device.destroy_descriptor_pool(descriptor_pool);
    }
}
