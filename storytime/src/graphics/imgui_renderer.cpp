#include "imgui_renderer.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Storytime {
    ImGuiRenderer::ImGuiRenderer(const ImGuiRendererConfig& config) : config(config) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ST_ASSERT(config.window != nullptr);
        bool glfw_callbacks_enabled = true;
        ImGui_ImplGlfw_InitForOpenGL(*config.window, glfw_callbacks_enabled);
        ImGui_ImplOpenGL3_Init(config.glsl_version.c_str());
    }

    ImGuiRenderer::~ImGuiRenderer() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::begin_frame() const {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::end_frame(f32 window_width, f32 window_height) const {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(window_width, window_height);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* glfwWindow = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(glfwWindow);
        }
    }
}
