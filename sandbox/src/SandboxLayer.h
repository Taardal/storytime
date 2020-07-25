#pragma once

#include "storytime/Storytime.h"

class SandboxLayer : public st::Layer
{
private:
    st::Renderer* renderer;
    st::OrthographicCameraController* cameraController;
    st::ResourceLoader* resourceLoader;
    st::Ref<st::Texture> kittenTexture;
    std::function<void()> onClose;

public:
    SandboxLayer(st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader);

    ~SandboxLayer() override = default;

    void SetOnCloseListener(const std::function<void()>& onClose);

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(st::Input* input, const st::Timestep& timestep) override;

    void OnRender(st::Renderer* renderer) override;

    void OnImGuiRender() override;

private:
    void OnEvent(const st::Event& event) override;
};