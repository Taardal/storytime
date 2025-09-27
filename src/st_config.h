#pragma once

#include "system/st_command_line_arguments.h"

namespace Storytime {
    struct Config {
        CommandLineArguments command_line_arguments{};
        std::string glsl_version = "#version 410";
        std::filesystem::path imgui_settings_file_path = "res/imgui_defaults.ini";
        LogLevel log_level = LogLevel::info;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 768;
        f32 window_aspect_ratio = 16.0f / 9.0f;
        bool window_fullscreen = false;
        bool window_maximized = false;
        bool window_resizable = true;
        bool window_vsync = false;
    };
}