#pragma once

#include "graphics/framebuffer.h"
#include "system/game_loop_statistics.h"
#include "window/window.h"

namespace Storytime {
    struct ImGuiRendererConfig {
        std::string glsl_version;
        Window* window = nullptr;
        EventManager* event_manager = nullptr;
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

        void render(const Framebuffer& framebuffer, const GameLoopStatistics& game_loop_stats);

        void end_frame() const;

    private:
        void initialize_imgui() const;

        static void terminate_imgui();

        static void render_root_window();

        void render_game_window(const Framebuffer& framebuffer);

        static void render_game_loop_window(const GameLoopStatistics& game_loop_stats);
    };
}
