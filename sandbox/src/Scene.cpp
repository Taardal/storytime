#include "Scene.h"
#include <iostream>

Scene::Scene(st::CoordinateSystem coordinateSystem, st::CameraController* cameraController, st::Renderer* renderer, st::ResourceLoader* resourceLoader)
        : coordinateSystem(coordinateSystem),
          cameraController(cameraController),
          renderer(renderer),
          world(sti::World::FromJsonFile("res/tiled/pixelcave/pixelcave.json")),
          imageLayerTextures(),
          tilesetTextures(new st::Ref<st::Texture>[world.Tilesets.size()]),
          tileSubTextures()
{
    LoadImageLayerTextures(resourceLoader);
    LoadTilesetTextures(resourceLoader);
    CreateTileSubTextures();

    cameraController->SetZoomLevel(200.0f);
#if 1
    cameraController->SetPosition(0.0f, 0.0f);
#else
    cameraController->SetPosition(-(cameraController->GetSize().Width / 2), (cameraController->GetSize().Height / 2));
#endif
}

void Scene::OnEvent(const st::Event& event)
{
    cameraController->OnEvent(event);
}

void Scene::OnUpdate(st::Timestep timestep)
{
    cameraController->OnUpdate(timestep);
#if 0
    ST_LOG_TRACE("x {0}, y {1}", cameraController->GetCamera()->GetPosition().x, cameraController->GetCamera()->GetPosition().y);
#endif
}

void Scene::OnRender()
{
    for (const sti::Layer& layer : world.Layers)
    {
        if (!layer.Visible)
        {
            continue;
        }
        if (layer.Type == "imagelayer")
        {
            RenderImageLayer(layer);
        }
        if (layer.Type == "tilelayer")
        {
            RenderTileLayer(layer);
        }
    }
}

void Scene::LoadImageLayerTextures(st::ResourceLoader* resourceLoader)
{
    for (const sti::Layer& layer : world.Layers)
    {
        if (layer.Type == "imagelayer")
        {
            const std::string& path = layer.Image;
            ST_LOG_DEBUG("Loading image layer with path [{0}]", path);
            st::Ref<st::Texture> texture = resourceLoader->LoadTexture("res/tiled/pixelcave/" + path);
            ST_LOG_DEBUG("Loaded image layer with ID [{0}]", texture->GetId());
            imageLayerTextures.insert({ layer.Id, texture });
        }
    }
}

void Scene::LoadTilesetTextures(st::ResourceLoader* resourceLoader)
{
    ST_LOG_DEBUG("Parsed world with [{0}] tilesets", world.Tilesets.size());
    for (uint32_t i = 0; i < world.Tilesets.size(); i++)
    {
        const std::string& path = world.Tilesets[i].Image;
        ST_LOG_DEBUG("Loading tileset with path [{0}]", path);
        st::Ref<st::Texture> texture = resourceLoader->LoadTexture("res/tiled/pixelcave/" + path);
        ST_LOG_DEBUG("Loaded tileset with ID [{0}]", texture->GetId());
        tilesetTextures[i] = texture;
    }
}

void Scene::CreateTileSubTextures()
{
    for (uint32_t i = 0; i < world.Tilesets.size(); i++)
    {
        uint32_t globalId = world.Tilesets[i].FirstGlobalId;
        ST_LOG_DEBUG("First global ID [{0}]", globalId);

        uint32_t numberOfTilesY = tilesetTextures[i]->GetHeight() / world.Tilesets[i].TileHeight;
        ST_LOG_DEBUG("Number of tiles Y [{0}]", numberOfTilesY);

        uint32_t numberOfTilesX = tilesetTextures[i]->GetWidth() / world.Tilesets[i].TileWidth;
        ST_LOG_DEBUG("Number of tiles X [{0}]", numberOfTilesX);

        for (uint32_t y = 0; y < numberOfTilesY; y++)
        {
            for (uint32_t x = 0; x < numberOfTilesX; x++)
            {
                st::SubTexture::Config config;
                config.Texture = tilesetTextures[i];
                config.Size = { world.Tilesets[i].TileWidth, world.Tilesets[i].TileHeight };
                if (coordinateSystem == st::CoordinateSystem::RightUp)
                {
                    config.Coordinates = { x, numberOfTilesY - (y + 1) };
                }
                else
                {
                    config.Coordinates = { x, y };
                }
                st::SubTexture subTexture(config);
                tileSubTextures.insert({ globalId, subTexture });
                globalId++;
            }
        }
        ST_LOG_DEBUG("Tiles [{0}]", tileSubTextures.size());
    }
}

void Scene::RenderImageLayer(const sti::Layer& layer) const
{
    st::Ref<st::Texture> texture = imageLayerTextures.at(layer.Id);

    float x = layer.X;
    float y = layer.Y;

    if (coordinateSystem == st::CoordinateSystem::RightDown)
    {
        y = y - layer.OffsetY;
    }
    if (coordinateSystem == st::CoordinateSystem::RightUp)
    {
        y = ((float) (world.TileHeight * world.Height)) - (texture->GetHeight() + layer.Y + layer.OffsetY);
    }

    st::Quad quad{};
    quad.Texture = texture;
    quad.Size = { texture->GetWidth(), texture->GetHeight() };
    quad.Position = { x, y, 0.0f };
    renderer->SubmitQuad(quad);
}

void Scene::RenderTileLayer(const sti::Layer& layer) const
{
    for (int y = 0; y < layer.Height; y++)
    {
        for (int x = 0; x < layer.Width; x++)
        {
            int tileId = layer.Data[x + y * layer.Width];
            if (tileId > 0)
            {
                auto x1 = (float) (x * world.TileWidth);
                auto y1 = (float) (y * world.TileHeight);

                if (coordinateSystem == st::CoordinateSystem::RightDown)
                {
                    y1 = -y1;
                }
                if (coordinateSystem == st::CoordinateSystem::RightUp)
                {
                    y1 = ((float) (world.TileHeight * world.Height)) - (y1 + world.TileHeight);
                }

                const st::SubTexture& subTexture = tileSubTextures.at(tileId);

                st::Quad quad{};
                quad.Texture = subTexture.GetTexture();
                quad.Size = subTexture.GetSize();
                quad.Position = { x1, y1, 0.0f };
                renderer->SubmitQuadFoo(quad, subTexture.GetTextureCoordinates());
            }
        }
    }
}
