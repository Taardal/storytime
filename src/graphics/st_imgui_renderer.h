#pragma once

#include "window/st_keyboard.h"
#include "window/st_mouse.h"
#include "window/st_window.h"

namespace Storytime {
    struct ImGuiRendererConfig {
        Window* window = nullptr;
        Keyboard* keyboard = nullptr;
        Mouse* mouse = nullptr;
        std::filesystem::path settings_file_path;
        std::string glsl_version;
    };

    class ImGuiRenderer {
    private:
        ImGuiRendererConfig config;

    public:
        ImGuiRenderer(const ImGuiRendererConfig& config);

        ~ImGuiRenderer();

        void begin_frame() const;

        void end_frame() const;
    };
}
