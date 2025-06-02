#pragma once

#include "window/st_window.h"

#include <imgui.h>

namespace Storytime {
    struct ImGuiRendererConfig {
        EventManager* event_manager = nullptr;
        Window* window = nullptr;
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
