#pragma once

#include "graphics/framebuffer.h"
#include "window/window.h"

#include <imgui.h>

namespace Storytime {
    struct ImGuiRendererConfig {
        std::filesystem::path settings_file_path;
        std::string glsl_version;
        EventManager* event_manager = nullptr;
        Window* window = nullptr;
    };

    class ImGuiRenderer {
    public:
        static std::string root_window_name;
        static std::string game_window_name;

    private:
        ImGuiRendererConfig config;
        ImVec2 game_window_size{};

    public:
        explicit ImGuiRenderer(const ImGuiRendererConfig& config);

        virtual ~ImGuiRenderer();

        static void begin_frame();

        void end_frame() const;

        void render(const Framebuffer& framebuffer);

    private:
        void initialize_imgui() const;

        static void terminate_imgui();

        static void render_root_window();

        void render_game_window(const Framebuffer& framebuffer);
    };
}
