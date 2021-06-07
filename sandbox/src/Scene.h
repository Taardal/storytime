#pragma once

#include <storytime/Storytime.h>
#include <storytiled/Storytiled.h>

class Scene
{
private:
    st::CoordinateSystem coordinateSystem;
    st::CameraController* cameraController;
    st::Renderer* renderer;
    sti::World world;
    st::Ref<st::Texture>* tilesetTextures;
    std::unordered_map<uint32_t, st::Ref<st::Texture>> imageLayerTextures;
    std::unordered_map<uint32_t, st::SubTexture> tileSubTextures;

public:
    Scene(st::CoordinateSystem coordinateSystem, st::CameraController* cameraController, st::Renderer* renderer, st::ResourceLoader* resourceLoader);

    void OnEvent(const st::Event& event);

    void OnUpdate(st::Timestep timestep);

    void OnRender();

private:
    void LoadImageLayerTextures(st::ResourceLoader* resourceLoader);

    void LoadTilesetTextures(st::ResourceLoader* resourceLoader);

    void CreateTileSubTextures();

    void RenderImageLayer(const sti::Layer& layer) const;

    void RenderTileLayer(const sti::Layer& layer) const;
};