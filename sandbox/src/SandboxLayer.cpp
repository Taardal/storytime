#include "system/Log.h"
#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("TriangleLayer"), cameraController(cameraController), resourceLoader(resourceLoader)
{
    kittenTexture = resourceLoader->LoadTexture("kitten.png");
    puppyTexture = resourceLoader->LoadTexture("puppy.png");
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::OnUpdate(st::Timestep timestep, st::Renderer* renderer, st::Input* input)
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
            quad.Position = { x, y, 0.0f };
            quad.Size = { 1.0f, 1.0f };
            //quad.Color = { (x + y) % 2, 0.2f, 0.5f, 1.0f };
            quad.RotationInDegrees = rotation;
            renderer->DrawQuad(quad);
        }
    }
}

void SandboxLayer::OnImGuiUpdate()
{
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
}

void SandboxLayer::OnEvent(const st::Event& event)
{
    cameraController->OnEvent(event);
}