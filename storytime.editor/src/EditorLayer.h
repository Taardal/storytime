#pragma once

#include "storytime/Storytime.h"

namespace storytime
{
    class EditorLayer : public Layer
    {
    private:
        Window* window;
        Renderer* renderer;
        OrthographicCameraController* cameraController;
        ResourceLoader* resourceLoader;
        Ref<Texture> kittenTexture;
        Ref<Framebuffer> framebuffer;
        glm::vec2 viewportSize;
        bool viewportFocused;
        bool viewportHovered;

    public:
        EditorLayer(Window* window, Renderer* renderer, OrthographicCameraController* cameraController, ResourceLoader* resourceLoader);

        ~EditorLayer() override = default;

        void OnAttach() override;

        void OnEvent(const Event& event) override;

        void OnUpdate(const Timestep& timestep, Input* input, Renderer* renderer) override;

        void OnImGuiRender(ImGuiRenderer* imGuiRenderer) override;

        void OnDetach() override;

    private:
        void SetupDockspacePanel() const;

        void SetupViewportPanel(ImGuiRenderer* imGuiRenderer);

        void SetupSettingsPanel() const;
    };
}