#pragma once

#include "storytime/Storytime.h"

class SandboxLayer : public st::Layer
{
private:
    st::OrthographicCameraController* cameraController = nullptr;
    st::ResourceLoader* resourceLoader = nullptr;
    st::Ref<st::Texture> kittenTexture = nullptr;
    st::Ref<st::Texture> puppyTexture = nullptr;

public:
    explicit SandboxLayer(st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader);

    ~SandboxLayer() override;

private:
    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(st::Timestep timestep, st::Renderer* renderer, st::Input* input) override;

public:
    void OnImGuiUpdate() override;

private:

    void OnEvent(const st::Event& event) override;
};