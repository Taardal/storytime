#pragma once

#include "window/Window.h"
#include "window/LayerStack.h"
#include "window/Input.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"

namespace storytime {

    class Application {
    private:
        LayerStack layerStack;
        Window* window;
        Renderer* renderer;
        Input* input;
        bool running;
        float lastFrameTime;

    public:
        Application(Window* window, Renderer* renderer, Input* input);

        ~Application();

        void run();

        void pushLayer(Layer* layer);

    private:
        void onEvent(const Event& event);

        void stop();
    };

}
