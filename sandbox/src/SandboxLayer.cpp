#include "system/Log.h"
#include "SandboxLayer.h"
#include <algorithm>

SandboxLayer::SandboxLayer(st::Window* window, st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader)
        : Layer("SandboxLayer"),
          window(window),
          renderer(renderer),
          cameraController(cameraController),
          resourceLoader(resourceLoader),
          kittenTexture(resourceLoader->LoadTexture("res/textures/kitten.png")),
          skyTexture(resourceLoader->LoadTexture("res/tiled/pixelcave/environments/parallax_bg_set_02.png")),
          tilesets(nullptr),
          tiles(),
          subTextures()
{
    this->cameraController->SetZoomLevel(200.0f);
}

void SandboxLayer::OnAttach()
{
#if 0
    std::cout << "OnAttach START" << std::endl;

    st::FileSystem fileSystem;
    const std::string& json = fileSystem.ReadFile("res/worlds/sandbox.json");
    world = sti::World::FromJson(json);
    std::cout << "Parsed world with " << world.Tilesets.size() << " tilesets" << std::endl;

    //
    // TILESETS
    //
    this->tilesets = new st::Ref<st::Texture>[world.Tilesets.size()];
    for (uint32_t i = 0; i < world.Tilesets.size(); i++)
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
    for (uint32_t i = 0; i < world.Tilesets.size(); i++)
    {
        uint32_t globalId = world.Tilesets[i].FirstGlobalId;
        std::cout << "First global ID [" << globalId << "]" << std::endl;

        uint32_t numberOfTilesY = this->tilesets[i]->GetHeight() / world.Tilesets[i].TileHeight;
        std::cout << "Number of tiles Y [" << numberOfTilesY << "]" << std::endl;

        uint32_t numberOfTilesX = this->tilesets[i]->GetWidth() / world.Tilesets[i].TileWidth;
        std::cout << "Number of tiles X [" << numberOfTilesX << "]" << std::endl;

        //st::SubTexture* subTextures = new st::SubTexture[world.Tilesets[i].TileCount];

        for (uint32_t y = 0; y < numberOfTilesY; y++)
        {
            for (uint32_t x = 0; x < numberOfTilesX; x++)
            {
                st::SubTexture::Config config;
                config.Texture = this->tilesets[i];
                config.Size = { world.Tilesets[i].TileWidth, world.Tilesets[i].TileHeight };
                config.Coordinates = { x, numberOfTilesY - (y + 1) };
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

#endif
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

#if 0
    for (const sti::Layer& layer : world.Layers)
    {
        if (layer.Type == "tilelayer" && layer.Visible)
        {
            for (int y = 0; y < layer.Height; y++)
            {
                for (int x = 0; x < layer.Width; x++)
                {
                    int tileId = layer.Data[x + y * layer.Width];
                    if (tileId > 0)
                    {
                        const auto& iterator = tiles.find(tileId);
                        if (iterator != tiles.end())
                        {
                            st::SubTexture& subTexture = iterator->second;

                            auto x1 = (float) (x * world.TileWidth);
                            auto y1 = (float) (y * world.TileHeight);
                            //std::cout << "Drawing tile [" << tileId << "]" << " at [X: " << x << ", Y: " << y << "]" << std::endl;

                            auto x2 = x1 - (world.Width * world.TileWidth) / 2;
                            auto y2 = (world.Height * world.TileHeight) - y1 - (world.Height * world.TileHeight) / 2;

                            st::Quad quad{};
                            quad.Texture = subTexture.GetTexture();
                            quad.Size = subTexture.GetSize();
                            quad.Position = { x2, y2, 0.0f };
                            renderer->SubmitQuadFoo(quad, subTexture.GetTextureCoordinates());
                        }
                    }
                }
            }
        }
    }
#endif

#if 0
    st::Quad quad{};
    quad.Texture = subTextures[0].GetTexture();
    quad.Size = subTextures[0].GetSize();
    quad.Position = { 0.0f, 0.0f, 0.0f };

    static float textureWidth = 128;
    static float textureHeight = 240;
    static float tileSize = 16;
    static float tileCountY = 15;
    static float tileCountX = 8;

    static float x = 7;
    static float y = 1;

    static float minX = x * tileSize / textureWidth;
    static float maxX = (x + 1) * tileSize / textureWidth;
    static float minY = y * tileSize / textureHeight;
    static float maxY = (y + 1) * tileSize / textureHeight;

    static glm::vec2 c[4];
    c[0] = { minX, minY };
    c[1] = { maxX, minY };
    c[2] = { maxX, maxY };
    c[3] = { minX, maxY };

    const glm::vec2* c1 = subTextures[0].GetTextureCoordinates();
    //renderer->SubmitQuad(quad);
    renderer->SubmitQuadFoo(quad, c1);
#endif

#if 0
    float cameraX = cameraController->GetCamera()->GetPosition().x;
    float cameraY = cameraController->GetCamera()->GetPosition().y;

    //int32_t cameraWidth = cameraController->GetAspectRatio() * cameraController->GetZoomLevel();
    //int32_t cameraHeight = cameraController->GetZoomLevel();

    int32_t cameraWidth = cameraController->GetSize().Width;
    int32_t cameraHeight = cameraController->GetSize().Height;

    int32_t windowWidth = window->GetSize().Width;
    int32_t windowHeight = window->GetSize().Height;

    int32_t textureWidth = skyTexture->GetWidth();
    int32_t textureHeight = skyTexture->GetHeight();

    float quadWidth = std::min(cameraWidth, textureWidth);
    float quadHeight = std::min(cameraHeight, textureHeight);

    float x = cameraX;
    float y = cameraY;

    float minX = x;
    float maxX = x + quadWidth;
    float minY = y;
    float maxY = y + quadHeight;

    glm::vec2 c[4];
    c[0] = { minX / textureWidth, minY / textureHeight };
    c[1] = { maxX / textureWidth, minY / textureHeight };
    c[2] = { maxX / textureWidth, maxY / textureHeight };
    c[3] = { minX / textureWidth, maxY / textureHeight };

    st::Quad quad{};
    quad.Texture = skyTexture;
    quad.Size = { quadWidth, quadHeight };
    quad.Position = { 0.0f, 0.0f, 0.0f };
    renderer->SubmitQuadFoo(quad, c);
#endif

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

