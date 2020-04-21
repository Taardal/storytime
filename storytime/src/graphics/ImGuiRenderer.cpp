#include "ImGuiRenderer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace storytime
{
    ImGuiRenderer::ImGuiRenderer(GraphicsContext* graphicsContext)
        : graphicsContext(graphicsContext)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
    }

    ImGuiRenderer::~ImGuiRenderer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiRenderer::Init(GLFWwindow* glfwWindow) const
    {
        bool glfwCallbacksEnabled = true;
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, glfwCallbacksEnabled);
        ImGui_ImplOpenGL3_Init(graphicsContext->getConfig().GlslVersion);
    }

    void ImGuiRenderer::BeginScene() const
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::EndScene() const
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}
