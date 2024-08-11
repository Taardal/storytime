#include "system/Log.h"
#include "EditorLayer.h"

namespace Storytime
{
    EditorLayer::EditorLayer(Window* window, Renderer* renderer, OrthographicCameraController* cameraController, ResourceLoader* resourceLoader)
            : Layer("EditorLayer"),
              window(window),
              renderer(renderer),
              cameraController(cameraController),
              resourceLoader(resourceLoader),
              kittenTexture(resourceLoader->LoadTexture("kitten.png")),
              framebuffer(nullptr),
              viewportSize(0.0f, 0.0f)
    {
        Framebuffer::Config config;
        config.Width = window->GetSize().Width;
        config.Height = window->GetSize().Height;
        framebuffer = CreateRef<Framebuffer>(config);
    }

    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnEvent(const Event& event)
    {
        cameraController->OnEvent(event);
    }

    void EditorLayer::OnUpdate(const Timestep& timestep, Input* input, Renderer* renderer)
    {
        const Framebuffer::Config& config = framebuffer->GetConfig();
        bool validViewportSize = viewportSize.x > 0.0f && viewportSize.y > 0.0f;
        bool changedViewportSize = viewportSize.x != config.Width || viewportSize.y != config.Height;
        if (validViewportSize && changedViewportSize)
        {
            framebuffer->Resize((uint32_t) viewportSize.x, (uint32_t) viewportSize.y);
            cameraController->Resize((uint32_t) viewportSize.x, (uint32_t) viewportSize.y);
        }
        if (viewportFocused)
        {
            cameraController->OnUpdate(timestep, input);
        }
        framebuffer->Bind();
        renderer->BeginScene(cameraController->GetCamera());

        Quad quad{};
        quad.Texture = kittenTexture;
        quad.Size = { 1.0f, 1.0f };
        quad.Position = { 0.0f, 0.0f, 0.0f };
        //quad.Color = { (x + y) % 2, 0.2f, 0.5f, 1.0f };
        renderer->SubmitQuad(quad);

        renderer->EndScene();
        framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender(ImGuiRenderer* imGuiRenderer)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);

        SetupDockspacePanel();
        SetupViewportPanel(imGuiRenderer);
        SetupSettingsPanel();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::SetupDockspacePanel() const
    {
        static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
        windowFlags |= ImGuiWindowFlags_NoTitleBar;
        windowFlags |= ImGuiWindowFlags_NoCollapse;
        windowFlags |= ImGuiWindowFlags_NoResize;
        windowFlags |= ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        windowFlags |= ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        static bool showDockspace = true;
        ImGui::Begin("Editor", &showDockspace, windowFlags);
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspaceId = ImGui::GetID("EditorDockspace");
            const ImVec2& dockspaceSize = ImVec2(0.0f, 0.0f);
            ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
            ImGui::DockSpace(dockspaceId, dockspaceSize, dockspaceFlags);
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::SetupViewportPanel(ImGuiRenderer* imGuiRenderer)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        viewportFocused = ImGui::IsWindowFocused();
        viewportHovered = ImGui::IsWindowHovered();
        imGuiRenderer->SetConsumeEvents(!viewportFocused || !viewportHovered);
        const ImVec2& viewportSize = ImGui::GetContentRegionAvail();
        this->viewportSize = { viewportSize.x, viewportSize.y };
        size_t textureId = (size_t) framebuffer->GetColorAttachmentTexture()->GetId();
        ImGui::Image((void*) textureId, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::SetupSettingsPanel() const
    {
        const st::Renderer::Statistics& statistics = renderer->GetStatistics();
        ImGui::Begin("Settings");
        ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
        ImGui::Text("Quads: %d", statistics.QuadCount);
        ImGui::Text("Vertices: %d", statistics.GetVertexCount());
        ImGui::Text("Indices: %d", statistics.GetIndexCount());
        ImGui::End();
    }
}
