#pragma once

#include "window/window.h"

namespace Storytime {
    struct ImGuiRendererConfig {
        Window* window = nullptr;
        std::string glsl_version;
    };

    class ImGuiRenderer {
    private:
        ImGuiRendererConfig config;

    public:
        explicit ImGuiRenderer(const ImGuiRendererConfig& config);

        virtual ~ImGuiRenderer();

        void begin_frame() const;

        void end_frame(float window_width, float window_height) const;
    };
}
