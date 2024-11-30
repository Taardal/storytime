#pragma once

#include "graphics/framebuffer.h"
#include "system/frame_info.h"
#include "window/window.h"

namespace Storytime {
    struct ImGuiRendererConfig {
        std::string glsl_version;
        Window* window = nullptr;
        EventManager* event_manager = nullptr;
    };

    class ImGuiRenderer {
    public:
        static std::string root_window_id;
        static std::string game_window_id;

    private:
        ImGuiRendererConfig config;
        Framebuffer framebuffer;
        std::vector<SubscriptionID> event_subscriptions;
        ImVec2 game_window_size{};

    public:
        explicit ImGuiRenderer(const ImGuiRendererConfig& config);

        virtual ~ImGuiRenderer();

        void begin_frame() const;

        void render(const GameLoopStatistics& frame_info);

        void end_frame() const;

    private:
        void initialize_imgui() const;

        static void terminate_imgui();

        void subscribe_to_events();

        void unsubscribe_from_events();

        static void render_root_window();

        void render_game_window(const Framebuffer& framebuffer);

        static void render_frame_info(const GameLoopStatistics& frame_info);
    };
}
