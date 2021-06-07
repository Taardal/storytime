#pragma once

#include "storytime/Storytime.h"
#include <storytiled/Storytiled.h>
#include <iostream>
#include <unordered_map>

class SandboxLayer : public st::Layer
{
private:
    st::Window* window;
    st::Renderer* renderer;
    st::CameraController* cameraController;
    st::ResourceLoader* resourceLoader;
    st::Ref<st::Texture> kittenTexture;
    st::Ref<st::Texture> skyTexture;
    sti::World world;
    st::Ref<st::Texture>* tilesets;
    std::unordered_map<uint32_t, st::SubTexture> tiles;
    std::vector<st::SubTexture> subTextures;

public:
    SandboxLayer(st::Window* window, st::Renderer* renderer, st::CameraController* cameraController, st::ResourceLoader* resourceLoader);

    ~SandboxLayer() override = default;

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(const st::Event& event) override;

    void OnUpdate(const st::Timestep& timestep, st::Input* input, st::Renderer* renderer) override;

    void OnImGuiRender(st::ImGuiRenderer* imGuiRenderer) override;

private:
    void SetupSettingsPanel() const;

    void DrawWorld() const;
};