#include "st_imgui_renderer.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Storytime {
    ImGuiRenderer::ImGuiRenderer(const ImGuiRendererConfig& config) : config(config) {
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

    ImGuiRenderer::~ImGuiRenderer() {
        ST_LOG_TRACE("Terminating ImGui");
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        ST_LOG_DEBUG("Terminated ImGui");
    }

    void ImGuiRenderer::begin_frame() const {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // If ImGui wants mouse/keyboard focus, we need to disable any mouse/keyboard input for the client application
        // For example, if you type "WASD" in an ImGui text field, the character in the client game should not move
        ImGuiIO& io = ImGui::GetIO();
        bool application_mouse_enabled = !io.WantCaptureMouse;
        bool application_keyboard_enabled = !io.WantCaptureKeyboard;
        config.mouse->set_enabled(application_mouse_enabled);
        config.keyboard->set_enabled(application_keyboard_enabled);
        config.window->set_mouse_events_enabled(application_mouse_enabled);
        config.window->set_keyboard_events_enabled(application_keyboard_enabled);
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
}
