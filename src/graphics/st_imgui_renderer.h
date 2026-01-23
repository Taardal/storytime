#pragma once

#include "graphics/st_vulkan_context.h"
#include "graphics/st_vulkan_physical_device.h"
#include "graphics/st_vulkan_device.h"
#include "graphics/st_vulkan_swapchain.h"
#include "window/st_keyboard.h"
#include "window/st_mouse.h"
#include "window/st_window.h"

namespace Storytime {
    struct ImGuiRendererConfig {
        std::string name = "ImGuiRenderer";
        const Window& window;
        const Keyboard& keyboard;
        const Mouse& mouse;
        const VulkanContext& context;
        const VulkanPhysicalDevice& physical_device;
        const VulkanDevice& device;
        const VulkanSwapchain& swapchain;
        u32 api_version = VK_API_VERSION_1_3;
        u32 frame_count = 0;
        std::filesystem::path settings_file_path;
        bool docking_enabled = false;
        bool viewports_enabled = false;

        const ImGuiRendererConfig& assert_valid() const {
            ST_ASSERT_GREATER_THAN_ZERO(api_version);
            ST_ASSERT_GREATER_THAN_ZERO(frame_count);
            return *this;
        }
    };

    class ImGuiRenderer {
    private:
        ImGuiRendererConfig config;

    public:
        ImGuiRenderer(const ImGuiRendererConfig& config);

        ~ImGuiRenderer();

        void begin_render() const;

        void end_render(VkCommandBuffer command_buffer) const;

    private:
        static void on_check_vk_result(VkResult vk_result);
    };
}
