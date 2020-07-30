#include "system/Log.h"
#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(st::Window* window, st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("SandboxLayer"),
          window(window),
          renderer(renderer),
          cameraController(cameraController),
          resourceLoader(resourceLoader),
          kittenTexture(resourceLoader->LoadTexture("kitten.png"))
{
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
    cameraController->OnUpdate(timestep, input);
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
}

void SandboxLayer::OnImGuiRender(st::ImGuiRenderer* imGuiRenderer)
{
}

void SandboxLayer::OnDetach()
{
}

void SandboxLayer::SetupSettingsPanel() const
{
    const st::Renderer::Statistics& statistics = renderer->GetStatistics();
    ImGui::Begin("Settings");
    ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
    ImGui::Text("Quads: %d", statistics.QuadCount);
    ImGui::Text("Vertices: %d", statistics.GetVertexCount());
    ImGui::Text("Indices: %d", statistics.GetIndexCount());
    ImGui::End();
}

void SandboxLayer::DrawWorld() const
{
    const sti::World& world = sti::World::FromJson("sandbox.json");
    for (const sti::Layer& layer : world.Layers)
    {
        if (layer.Type == "tilelayer" && layer.Visible)
        {
            const glm::vec3& cameraPosition = cameraController->GetCamera()->GetPosition();
            int topRow = (cameraPosition.y - world.TileHeight) / world.TileHeight;
            int bottomRow = (cameraPosition.y + window->GetSize().Height + world.TileHeight) / world.TileHeight;
            int leftColumn = (cameraPosition.x - world.TileWidth) / world.TileWidth;
            int rightColumn = (cameraPosition.x + window->GetSize().Width + world.TileWidth) / world.TileWidth;
            for (int row = topRow; row < bottomRow; row++) {
                if (row < 0) {
                    continue;
                }
                if (row >= world.Height) {
                    break;
                }
                for (int column = leftColumn; column < rightColumn; column++) {
                    if (column < 0) {
                        continue;
                    }
                    if (column >= world.Width) {
                        break;
                    }
                    int tileId = layer.Data[row + column * world.Width];
                    if (tileId) {
                        // Get sub-texture for tileset (i.e. texture atlas)
                        // Draw sub-texture
                    }
                }
            }
        }
    }
}

