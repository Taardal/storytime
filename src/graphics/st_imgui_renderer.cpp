#include "st_imgui_renderer.h"

#include "system/st_assert.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace Storytime {

    ImGuiRenderer::ImGuiRenderer(const ImGuiRendererConfig& config) : config(config.assert_valid()) {
        ST_ASSERT(IMGUI_CHECKVERSION(), "Invalid ImGui version");
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        // Docking: https://github.com/ocornut/imgui/wiki/Docking
        if (config.docking_enabled) {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }

        // Multi Viewports: https://github.com/ocornut/imgui/wiki/Multi-Viewports
        if (config.viewports_enabled) {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }

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
    }

    void ImGuiRenderer::end_render(VkCommandBuffer command_buffer) const {
        // Takes all the ImGui widgets created during this frame (ImGui::Begin, ImGui::Button, etc.) and builds the final draw command lists.
        ImGui::Render();

        // Takes the baked data from ImGui::Render() and records Vulkan commands into the command buffer.
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), command_buffer);

        i32 multiple_viewports_enabled = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
        if (multiple_viewports_enabled) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ImGuiRenderer::on_check_vk_result(VkResult vk_result) {
        if (vk_result != VK_SUCCESS) {
            ST_THROW("Could not initialize ImGuiRenderer: " << format_vk_result(vk_result));
        }
    }
}
