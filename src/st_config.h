#pragma once

#include "system/st_command_line_arguments.h"

namespace Storytime {
    struct Config {
        std::string app_name = "Storytime App";
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;
        i32 window_width = 1280;
        i32 window_height = 768;
        f32 window_aspect_ratio = 16.0f / 9.0f;
        bool window_fullscreen = false;
        bool window_maximized = false;
        bool window_resizable = true;
        bool window_vsync = false;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string glsl_version = "#version 410";
        std::filesystem::path imgui_settings_file_path = "res/imgui_defaults.ini";
        u32 rendering_buffer_count = 2;
#ifdef ST_RELEASE
        bool vulkan_validation_layers_enabled = false;
#else
        bool vulkan_validation_layers_enabled = true;
#endif
    };
}