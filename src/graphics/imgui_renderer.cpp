#include "imgui_renderer.h"
#include "imgui_window_event.h"
#include "window/window_event.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Storytime {
    std::string ImGuiRenderer::root_window_name = "Root";
    std::string ImGuiRenderer::game_window_name = "Game";

    ImGuiRenderer::ImGuiRenderer(const ImGuiRendererConfig& config)
    : config(config),
      framebuffer({
          .width = (u32) config.window->get_size_in_pixels().width,
          .height = (u32) config.window->get_size_in_pixels().height,
      })
    {
        initialize_imgui();
        subscribe_to_events();
    }

    ImGuiRenderer::~ImGuiRenderer() {
        unsubscribe_from_events();
        terminate_imgui();
    }

    void ImGuiRenderer::initialize_imgui() const {
        ST_LOG_TRACE("Initializing ImGui");

        ST_ASSERT(IMGUI_CHECKVERSION(), "Invalid ImGui version");
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Docking: https://github.com/ocornut/imgui/wiki/Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Multi Viewports: https://github.com/ocornut/imgui/wiki/Multi-Viewports

        ST_ASSERT(config.window != nullptr, "Invalid window");
        constexpr bool glfw_callbacks_enabled = true;
        ImGui_ImplGlfw_InitForOpenGL(*config.window, glfw_callbacks_enabled);
        ImGui_ImplOpenGL3_Init(config.glsl_version.c_str());

        ST_LOG_DEBUG("Initialized ImGui");
    }

    void ImGuiRenderer::terminate_imgui() {
        ST_LOG_TRACE("Terminating ImGui");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ST_LOG_DEBUG("Terminated ImGui");
    }

    void ImGuiRenderer::subscribe_to_events() {
        event_subscriptions.push_back(
            config.event_manager->subscribe(WindowResizeEvent::type, [this](const Event& event) {
                auto& window_resize_event = (WindowResizeEvent&) event;
                framebuffer.resize(window_resize_event.width, window_resize_event.height);
            })
        );
        event_subscriptions.push_back(
            config.event_manager->subscribe(ImGuiWindowResizeEvent::type, [this](const Event& event) {
                auto& window_resize_event = (ImGuiWindowResizeEvent&) event;
                if (window_resize_event.window_id == game_window_name) {
                    framebuffer.resize(window_resize_event.width, window_resize_event.height);
                }
            })
        );
    }

    void ImGuiRenderer::unsubscribe_from_events() {
        config.event_manager->unsubscribe_and_clear(event_subscriptions);
    }

    void ImGuiRenderer::begin_frame() const {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        framebuffer.bind();
    }

    void ImGuiRenderer::render(const GameLoopStatistics& frame_info) {
        framebuffer.unbind();
        render_root_window();
        render_game_window(framebuffer);
        render_game_loop_statistics(frame_info);
    }

    void ImGuiRenderer::end_frame() const {
        // Prepare ImGui rendering data for the current frame
        ImGui::Render();

        // Render ImGui UI using OpenGL
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int multiple_viewports_enabled = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
        if (multiple_viewports_enabled) {
            // Create or destroy platform-specific windows managed by ImGui
            ImGui::UpdatePlatformWindows();

            // Render the ImGui interface to each window's OpenGL context
            // This may switch the OpenGL context from our main application window to other windows
            ImGui::RenderPlatformWindowsDefault();

            // Restore the OpenGL context to our main application window
            glfwMakeContextCurrent(*config.window);
        }
    }

    void ImGuiRenderer::render_root_window() {
        // Add styling
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        // Make the root window fill the entire application window when created
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);

        // Disable all window interactions
        // It should only serve as a static container for all other ImGui windows
        static ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        window_flags |= ImGuiWindowFlags_NoNavFocus;

        // Hide the close button in top-right corner of window
        static bool* window_close_button = nullptr;

        // Begin window
        ImGui::Begin(root_window_name.c_str(), window_close_button, window_flags);

        // Create a dockspace that allows for windows to be docked into the window
        ST_ASSERT(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable, "ImGui docking must be enabled");
        ImGui::DockSpace(ImGui::GetID("Dockspace"));

        // End window
        ImGui::End();

        // Cleanup styling
        ImGui::PopStyleVar();
    }

    void ImGuiRenderer::render_game_window(const Framebuffer& framebuffer) {
        // Add styling
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        // Begin window
        ImGui::Begin(game_window_name.c_str());

        // Retrieve the texture ID from the framebuffer object, which represents the rendered scene
        size_t framebuffer_texture_id = framebuffer.get_color_attachment_texture()->get_id();

        // Get the size of the window to render the framebuffer texture to fit inside the window
        game_window_size = ImGui::GetContentRegionAvail();

        bool game_window_size_is_valid = game_window_size.x > 0.0f && game_window_size.y > 0.0f;

        auto& framebuffer_config = framebuffer.get_config();
        bool game_window_size_has_changed = game_window_size.x != framebuffer_config.width || game_window_size.y != framebuffer_config.height;

        if (game_window_size_is_valid && game_window_size_has_changed) {
            auto game_window_resize_event = std::make_shared<ImGuiWindowResizeEvent>(game_window_name, game_window_size.x, game_window_size.y);
            ST_ASSERT(config.event_manager != nullptr, "Event manager cannot be null");
            config.event_manager->queue_event(ImGuiWindowResizeEvent::type, game_window_resize_event);
        }

        // Draw the framebuffer texture in the window.
        // Flip the Y texture coordinates to avoid the image being rendered upside down by OpenGL
        static ImVec2 start_texture_coordinate{ 0, 1 }; // Default is [0, 0]
        static ImVec2 end_texture_coordinate{ 1, 0 };   // Default is [1, 1]
        ImGui::Image((void*) framebuffer_texture_id, game_window_size, start_texture_coordinate, end_texture_coordinate);

        // End window
        ImGui::End();

        // Cleanup styling
        ImGui::PopStyleVar();
    }

    void ImGuiRenderer::render_game_loop_statistics(const GameLoopStatistics& game_loop_stats) {
        static f64 average_frames_per_second = 0.0;
        static f64 average_updates_per_second = 0.0;
        static f64 average_update_timestep_ms = 0.0;
        static f64 average_update_duration_ms = 0.0;
        static f64 average_render_duration_ms = 0.0;
        static f64 average_imgui_render_duration_ms = 0.0;
        static f64 average_cycle_duration_ms = 0.0;
        static f64 average_window_events_duration_ms = 0.0;
        static f64 average_game_events_duration_ms = 0.0;
        static f64 average_swap_buffers_duration_ms = 0.0;
        static f64 xa = 0.0;

        {
            f64 smoothing_factor = 0.1;
            average_frames_per_second = smooth_average(game_loop_stats.frames_per_second, average_frames_per_second, smoothing_factor);
            average_updates_per_second = smooth_average(game_loop_stats.updates_per_second, average_updates_per_second, smoothing_factor);
            average_update_timestep_ms = smooth_average(game_loop_stats.update_timestep_ms, average_update_timestep_ms, smoothing_factor);
        }
        {
            f64 smoothing_factor = 0.01;
            average_cycle_duration_ms = smooth_average(game_loop_stats.cycle_duration_ms, average_cycle_duration_ms, smoothing_factor);
            average_imgui_render_duration_ms = smooth_average(game_loop_stats.imgui_render_duration_ms, average_imgui_render_duration_ms, smoothing_factor);
            average_game_events_duration_ms = smooth_average(game_loop_stats.game_events_duration_ms, average_game_events_duration_ms);
            average_render_duration_ms = smooth_average(game_loop_stats.render_duration_ms, average_render_duration_ms, smoothing_factor);
            average_swap_buffers_duration_ms = smooth_average(game_loop_stats.swap_buffers_duration_ms, average_swap_buffers_duration_ms);
            average_window_events_duration_ms = smooth_average(game_loop_stats.window_events_duration_ms, average_window_events_duration_ms);
            average_update_duration_ms = smooth_average(game_loop_stats.update_duration_ms, average_update_duration_ms, smoothing_factor);
        }

        ImGui::Begin("Game loop");
        ImGui::Text("FPS: %.2lf", average_frames_per_second);
        ImGui::Text("UPS: %.2lf", average_updates_per_second);
        ImGui::Text("Update timestep: %.2lf ms", average_update_timestep_ms);
        ImGui::Text("Update duration: %.3lf ms", average_update_duration_ms);
        ImGui::Text("Render duration: %.3lf ms", average_render_duration_ms);
        ImGui::Text("ImGui render duration: %.3lf ms", average_imgui_render_duration_ms);
        ImGui::Text("Window events duration: %.3lf ms", average_window_events_duration_ms);
        ImGui::Text("Game events duration: %.3lf ms", average_game_events_duration_ms);
        ImGui::Text("Swap buffers duration: %.3lf ms", average_swap_buffers_duration_ms);
        ImGui::Text("Cycle duration: %.3lf ms", average_cycle_duration_ms);
        ImGui::End();
    }
}
