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
        Input* input;
        Renderer* renderer;
        ImGuiRenderer* imGuiRenderer;
        OrthographicCameraController* cameraController;
        LayerStack layerStack;
        float lastFrameTime;
        bool running;
        bool minimized;

    public:
        Application(Window* window, Input* input, Renderer* renderer, ImGuiRenderer* imGuiRenderer, OrthographicCameraController* cameraController);

        ~Application();

        void Run();

        void Stop();

        void PushLayer(Layer* layer);

    private:
        void OnEvent(const Event& event);

        void OnLayerEvent(const Event& event) const;
    };

}
