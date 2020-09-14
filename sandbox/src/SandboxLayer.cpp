#include "system/Log.h"
#include "SandboxLayer.h"

SandboxLayer::SandboxLayer(st::Window* window, st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("SandboxLayer"),
          window(window),
          renderer(renderer),
          cameraController(cameraController),
          resourceLoader(resourceLoader),
          kittenTexture(resourceLoader->LoadTexture("res/textures/kitten.png")),
          tilesets(nullptr),
          tiles()
{
}

void SandboxLayer::OnAttach()
{
    std::cout << "OnAttach START" << std::endl;

    st::FileSystem fileSystem;
    const std::string& json = fileSystem.ReadFile("res/worlds/sandbox.json");
    const sti::World& world = sti::World::FromJson(json);
    std::cout << "Parsed world with " << world.Tilesets.size() << " tilesets" << std::endl;

    //
    // TILESETS
    //
    this->tilesets = new st::Ref<st::Texture>[world.Tilesets.size()];
    for (uint32_t i = 0; i < world.Tilesets.size(); ++i)
    {
        const std::string& path = world.Tilesets[i].Image;
        std::cout << "Loading tileset with path " << path << std::endl;
        const std::string& name = path.substr(path.find_last_of("\\/") + 1, path.length());
        std::cout << "Loading tileset with name " << name << std::endl;
        st::Ref<st::Texture> tilesetTexture = resourceLoader->LoadTexture("res/tilesets/" + name);
        std::cout << "Loaded tileset with ID " << tilesetTexture->GetId() << std::endl;
        this->tilesets[i] = tilesetTexture;
    }

    //
    // SUB TEXTURES / TILES
    //
    for (uint32_t i = 0; i < world.Tilesets.size(); ++i)
    {
        uint32_t globalId = world.Tilesets[i].FirstGlobalId;
        std::cout << "First global ID [" << globalId << "]" << std::endl;

        uint32_t numberOfTilesY = this->tilesets[i]->GetHeight() / world.Tilesets[i].TileHeight;
        std::cout << "Number of tiles Y [" << numberOfTilesY << "]" << std::endl;

        uint32_t numberOfTilesX = this->tilesets[i]->GetWidth() / world.Tilesets[i].TileWidth;
        std::cout << "Number of tiles X [" << numberOfTilesX << "]" << std::endl;

        //st::SubTexture* subTextures = new st::SubTexture[world.Tilesets[i].TileCount];
        std::vector<st::SubTexture> subTextures;
        for (uint32_t y = 0; y < numberOfTilesY; ++y)
        {
            for (uint32_t x = 0; x < numberOfTilesX; ++x)
            {
                st::SubTexture::Config config;
                config.texture = this->tilesets[i];
                config.size = { world.Tilesets[i].TileWidth, world.Tilesets[i].TileHeight };
                config.coordinates = { x, y };
                st::SubTexture subTexture(config);
                subTextures.push_back(subTexture);
                tiles.insert({ globalId, subTexture });
                globalId++;
            }
        }

        std::cout << "Sub textures [" << subTextures.size() << "]" << std::endl;
        std::cout << "Tiles [" << tiles.size() << "]" << std::endl;
    }

    //
    // "DRAW"
    //
    for (const sti::Layer& layer : world.Layers)
    {
        if (layer.Type == "tilelayer" && layer.Visible)
        {
            std::cout << "data size [" << layer.Data.size() << "]" << std::endl;
            for (int y = 0; y < layer.Width; y++)
            {
                for (int x = 0; x < layer.Height; x++)
                {
                    int i = x + y * layer.Width;
                    //std::cout << "Data index [" << i << "]" << " at [X: " << x << ", Y: " << y << "]" << std::endl;

                    int tileId = layer.Data[i];
                    if (tileId > 0)
                    {
                        //std::cout << "Tile ID [" << tileId << "]" << std::endl;
                        if (tiles.find(tileId) != tiles.end())
                        {
                            int x1 = x * world.TileWidth;
                            int y1 = y * world.TileHeight;
                            std::cout << "Drawing tile [" << tileId << "]" << " at [X: " << x << ", Y: " << y << "]" << std::endl;
                        }
                    }
                }
            }
        }
    }

    std::cout << "OnAttach END" << std::endl;
}

void SandboxLayer::OnDetach()
{
    //delete tilesets;
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

