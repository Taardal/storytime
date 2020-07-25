#include "system/Log.h"
#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(st::Window* window, st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("TriangleLayer"),
          window(window),
          renderer(renderer),
          cameraController(cameraController),
          resourceLoader(resourceLoader),
          kittenTexture(resourceLoader->LoadTexture("kitten.png")),
          framebuffer(nullptr),
          viewportSize(0.0f, 0.0f)
{
    st::Framebuffer::Config config;
    config.Width = window->GetSize().Width;
    config.Height = window->GetSize().Height;
    framebuffer = st::CreateRef<st::Framebuffer>(config);
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnEvent(const st::Event& event)
{
    cameraController->OnEvent(event);
}

void SandboxLayer::OnUpdate(const st::Timestep& timestep, st::Input* input, st::Renderer* renderer)
{
    const st::Framebuffer::Config& config = framebuffer->GetConfig();
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

    static float rotation = 0.0f;
    rotation += timestep * 50.0f;
    for (uint32_t x = 0; x < 1; x++)
    {
        for (uint32_t y = 0; y < 1; y++)
        {
            st::Quad quad{};
            quad.Texture = kittenTexture;
            quad.Size = { 1.0f, 1.0f };
            quad.Position = { 0.0f, 0.0f, 0.0f };
            //quad.Position = { x * quad.Size.x, y * quad.Size.y, 0.0f };
            //quad.Color = { (x + y) % 2, 0.2f, 0.5f, 1.0f };
            //quad.TilingFactor = 20.0f;
            //quad.RotationInDegrees = rotation;
            renderer->SubmitQuad(quad);
        }
    }

    renderer->EndScene();
    framebuffer->Unbind();
}

void SandboxLayer::OnImGuiRender(st::ImGuiRenderer* imGuiRenderer)
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);

    SetupDockspacePanel();
    SetupViewportPanel(imGuiRenderer);
    SetupSettingsPanel();
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::SetupDockspacePanel() const
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

void SandboxLayer::SetupViewportPanel(st::ImGuiRenderer* imGuiRenderer)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
    ImGui::Begin("Viewport");
    viewportFocused = ImGui::IsWindowFocused();
    viewportHovered = ImGui::IsWindowHovered();
    imGuiRenderer->SetConsumeEvents(!viewportFocused || !viewportHovered);
    const ImVec2& viewportSize = ImGui::GetContentRegionAvail();
    this->viewportSize = { viewportSize.x, viewportSize.y };
    size_t textureId = (size_t) framebuffer->GetColorAttachmentId();
    ImGui::Image((void*) textureId, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
    ImGui::PopStyleVar();
}

void SandboxLayer::SetupSettingsPanel() const
{
    ImGui::Begin("Settings");
    auto stats = renderer->GetStatistics();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetVertexCount());
    ImGui::Text("Indices: %d", stats.GetIndexCount());
    ImGui::End();
}
