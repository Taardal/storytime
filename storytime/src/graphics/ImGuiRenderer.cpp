#include "ImGuiRenderer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace storytime
{
    ImGuiRenderer::ImGuiRenderer(GraphicsContext* graphicsContext)
        : graphicsContext(graphicsContext), consumeEvents(false)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
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

    void ImGuiRenderer::Begin() const
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiRenderer::End(float windowWidth, float windowHeight) const
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(windowWidth, windowHeight);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* glfwWindow = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(glfwWindow);
        }
    }

    void ImGuiRenderer::SetConsumeEvents(bool consumeEvents)
    {
        this->consumeEvents = consumeEvents;
    }

    void ImGuiRenderer::OnEvent(Event& event) const
    {
        if (consumeEvents)
        {

        }
    }

}
