#pragma once

namespace Storytime {
    struct CommandLineArguments {
        int count = 0;
        char** args = nullptr;

        const char* operator[](int index) const {
            ST_ASSERT(index < count);
            return args[index];
        }
    };

    struct Config {
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;
        u32 target_fps = 60;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 760;
        bool window_maximized = false;
        bool window_resizable = true;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string glsl_version = "#version 410";
    };
}
