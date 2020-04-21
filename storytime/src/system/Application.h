#pragma once

#include "window/Window.h"
#include "window/LayerStack.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"
#include "graphics/OrthographicCameraController.h"

namespace storytime
{
    class Application
    {
    private:
        Window* window;
        Renderer* renderer;
        ImGuiRenderer* imGuiRenderer;
        OrthographicCameraController* cameraController;
        LayerStack layerStack;
        float lastFrameTime = 0.0f;
        bool running = false;
        bool minimized = false;

    public:
        Application(Window* window, Renderer* renderer, ImGuiRenderer* imGuiRenderer, OrthographicCameraController* cameraController);

        ~Application();

        void Run();

        void PushLayer(Layer* layer);

    private:
        void OnEvent(const Event& event);

        void Stop();

        void OnLayerEvent(const Event& event) const;
    };

}
