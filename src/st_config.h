#pragma once

#include "system/st_command_line_arguments.h"

namespace Storytime {
    struct Config {
        std::string app_name = "Storytime App";
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;

        // Window
        i32 window_width = 1280;
        i32 window_height = 768;
        f32 window_aspect_ratio = 16.0f / 9.0f;
        bool window_fullscreen = false;
        bool window_maximized = false;
        bool window_resizable = true;
        bool window_vsync = false;

        // Renderer
        u32 rendering_buffer_count = 3;
        glm::vec4 rendering_clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
        std::string vulkan_app_name = "Storytime";
        std::string vulkan_engine_name = "Storytime Engine";
        u32 vulkan_app_version = VK_MAKE_API_VERSION(0, 1, 0, 0);
        u32 vulkan_engine_version = VK_MAKE_API_VERSION(0, 1, 0, 0);
        u32 vulkan_api_version = VK_API_VERSION_1_3;
#ifdef ST_RELEASE
        bool vulkan_validation_layers_enabled = false;
#else
        bool vulkan_validation_layers_enabled = true;
#endif
        bool vsync_enabled = false;

        // ImGui
        std::filesystem::path imgui_settings_file_path = "res/imgui_defaults.ini";
#ifdef ST_RELEASE
        bool imgui_docking_enabled = false;
        bool imgui_viewports_enabled = false;
#else
        bool imgui_docking_enabled = true;
        bool imgui_viewports_enabled = true;
#endif
    };
}