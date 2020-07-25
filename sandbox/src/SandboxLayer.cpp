#include "system/Log.h"
#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("TriangleLayer"),
          renderer(renderer),
          cameraController(cameraController),
          resourceLoader(resourceLoader),
          kittenTexture(resourceLoader->LoadTexture("kitten.png")),
          onClose(nullptr)
{
}

void SandboxLayer::SetOnCloseListener(const std::function<void()>& onClose) {
    this->onClose = onClose;
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(st::Input* input, const st::Timestep& timestep)
{
    cameraController->OnUpdate(timestep, input);

    static float rotation = 0.0f;
    rotation += timestep * 50.0f;

    for (uint32_t x = 0; x < 1; x++)
    {
        for (uint32_t y = 0; y < 1; y++)
        {
            st::Quad quad{};
            quad.Texture = kittenTexture;
            quad.Size = { 1.0f, 1.0f };
            quad.Position = { x * quad.Size.x, y * quad.Size.y, 0.0f };
            //quad.Color = { (x + y) % 2, 0.2f, 0.5f, 1.0f };
            //quad.TilingFactor = 20.0f;
            //quad.RotationInDegrees = rotation;
            renderer->SubmitQuad(quad);
        }
    }
}

void SandboxLayer::OnRender(st::Renderer* renderer)
{

}

void SandboxLayer::OnImGuiRender()
{
    static bool fullscreen = true;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    static ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
        windowFlags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    static bool showDockspace = true;
    ImGui::Begin("DockSpace Demo", &showDockspace, windowFlags);
    
    ImGui::PopStyleVar();
    if (fullscreen)
    {
        ImGui::PopStyleVar(2);
    }

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                onClose();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::End();


    auto stats = renderer->GetStatistics();
    ImGui::Begin("Rendering");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetVertexCount());
    ImGui::Text("Indices: %d", stats.GetIndexCount());
    size_t textureId = (size_t) kittenTexture->GetId();
    ImGui::Image((void*) textureId, ImVec2(320, 240), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();

    bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);
}

void SandboxLayer::OnEvent(const st::Event& event)
{
    cameraController->OnEvent(event);
}