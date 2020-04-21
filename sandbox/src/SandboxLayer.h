#pragma once

#include "storytime/Storytime.h"

class SandboxLayer : public st::Layer
{
private:
    st::Renderer* renderer;
    st::Input* input;
    st::OrthographicCameraController* cameraController;
    st::ResourceLoader* resourceLoader;
    st::Ref<st::Texture> kittenTexture;

public:
    explicit SandboxLayer(st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader);

    ~SandboxLayer() override = default;

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(st::Timestep timestep) override;

    void OnImGuiUpdate() override;

private:
    void OnEvent(const st::Event& event) override;
};