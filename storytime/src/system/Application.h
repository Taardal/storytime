#pragma once

#include "window/Window.h"
#include "window/LayerStack.h"
#include "window/Input.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"
#include "graphics/OrthographicCameraController.h"

namespace storytime {

    class Application {
    private:
        Window* window;
        Renderer* renderer;
        Input* input;
        OrthographicCameraController* cameraController;
        LayerStack layerStack;
        float lastFrameTime = 0.0f;
        bool running = false;
        bool minimized = false;

    public:
        Application(Window* window, Renderer* renderer, Input* input, OrthographicCameraController* cameraController);

        ~Application();

        void run();

        void pushLayer(Layer* layer);

    private:
        void onEvent(const Event& event);

        void stop();

        void onLayerEvent(const Event& event) const;
    };

}
