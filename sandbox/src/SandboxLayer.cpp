#include "system/Log.h"
#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("TriangleLayer"), renderer(renderer), input(input), cameraController(cameraController), resourceLoader(resourceLoader)
{
    kittenTexture = resourceLoader->LoadTexture("kitten.png");
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(st::Timestep timestep)
{
    cameraController->OnUpdate(timestep, input);

    static float rotation = 0.0f;
    rotation += timestep * 50.0f;

    for (uint32_t x = 0; x < 10; x++)
    {
        for (uint32_t y = 0; y < 10; y++)
        {
            st::Quad quad{};
            quad.Texture = kittenTexture;
            quad.Size = { 0.1f, 0.1f };
            quad.Position = { x * quad.Size.x, y * quad.Size.y, 0.0f };
            //quad.Color = { (x + y) % 2, 0.2f, 0.5f, 1.0f };
            quad.RotationInDegrees = rotation;
            renderer->DrawQuad(quad);
        }
    }
}

void SandboxLayer::OnImGuiUpdate()
{
    ImGui::Begin("Renderer statistics");

    auto stats = renderer->GetStatistics();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetVertexCount());
    ImGui::Text("Indices: %d", stats.GetIndexCount());

    ImGui::End();
}

void SandboxLayer::OnEvent(const st::Event& event)
{
    cameraController->OnEvent(event);
}