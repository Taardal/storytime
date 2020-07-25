#pragma once

#include "storytime/Storytime.h"

class SandboxLayer : public st::Layer
{
private:
    st::Window* window;
    st::Renderer* renderer;
    st::OrthographicCameraController* cameraController;
    st::ResourceLoader* resourceLoader;
    st::Ref<st::Texture> kittenTexture;
    st::Ref<st::Framebuffer> framebuffer;
    glm::vec2 viewportSize;
    bool viewportFocused;
    bool viewportHovered;

public:
    SandboxLayer(st::Window* window, st::Renderer* renderer, st::OrthographicCameraController* cameraController, st::ResourceLoader* resourceLoader);

    ~SandboxLayer() override = default;

    void OnAttach() override;

    void OnEvent(const st::Event& event) override;

    void OnUpdate(const st::Timestep& timestep, st::Input* input, st::Renderer* renderer) override;

    void OnImGuiRender(st::ImGuiRenderer* imGuiRenderer) override;

    void OnDetach() override;

private:
    void SetupDockspacePanel() const;

    void SetupViewportPanel(st::ImGuiRenderer* imGuiRenderer);

    void SetupSettingsPanel() const;
};