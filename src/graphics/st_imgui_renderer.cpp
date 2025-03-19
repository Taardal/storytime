#include "st_imgui_renderer.h"
#include "st_imgui_window_event.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Storytime {
    std::string ImGuiRenderer::root_window_name = "Root";
    std::string ImGuiRenderer::game_window_name = "Game";

    ImGuiRenderer::ImGuiRenderer(const ImGuiRendererConfig& config) : config(config) {
        initialize_imgui();
    }

    ImGuiRenderer::~ImGuiRenderer() {
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

        if (!config.settings_file_path.empty() && !std::filesystem::exists("imgui.ini")) {
            ImGui::LoadIniSettingsFromDisk(config.settings_file_path.c_str());
        }

        ST_LOG_DEBUG("Initialized ImGui");
    }

    void ImGuiRenderer::terminate_imgui() {
        ST_LOG_TRACE("Terminating ImGui");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ST_LOG_DEBUG("Terminated ImGui");
    }

    void ImGuiRenderer::begin_frame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::end_frame() const {
        // Prepare ImGui rendering data for the current frame
        ImGui::Render();

        // Render ImGui UI using OpenGL
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        i32 multiple_viewports_enabled = ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable;
        if (!multiple_viewports_enabled) {
            return;
        }

        // Create or destroy platform-specific windows managed by ImGui
        ImGui::UpdatePlatformWindows();

        // Render the ImGui interface to each window's OpenGL context
        // This may switch the OpenGL context from our main application window to other windows
        ImGui::RenderPlatformWindowsDefault();

        // Restore the OpenGL context to our main application window
        glfwMakeContextCurrent(*config.window);
    }

    void ImGuiRenderer::render(const Framebuffer& framebuffer) {
        render_root_window();
        render_game_window(framebuffer);
    }

    void ImGuiRenderer::render_root_window() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        // Make the root window fill the entire application window when created
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);

        // Disable interactions. It should only serve as a static container for all other ImGui windows.
        static ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoTitleBar;

        // Hide the close button in top-right corner of window
        static bool* window_close_button = nullptr;

        ImGui::Begin(root_window_name.c_str(), window_close_button, window_flags);

        // Create a dockspace that allows for windows to be docked into the window
        ST_ASSERT(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable, "ImGui docking must be enabled");
        ImGuiID dockspace_id = ImGui::GetID("Dockspace");
        ImGui::DockSpace(dockspace_id);

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void ImGuiRenderer::render_game_window(const Framebuffer& framebuffer) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
        ImGui::SetNextWindowClass(&window_class);

        static ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoMove;

        // Hide the close button in top-right corner of window
        static bool* window_close_button = nullptr;

        ImGui::Begin(game_window_name.c_str(), window_close_button, window_flags);

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

        ImGui::End();
        ImGui::PopStyleVar();
    }
}
