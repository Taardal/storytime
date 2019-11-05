#pragma once

#include "window/Window.h"
#include "window/layers/LayerStack.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"

namespace Darkle {

    class Application {
    private:
        Window* window;
        Renderer* renderer;
        LayerStack layerStack;
        bool running;

    public:
        Application(Window* window, Renderer* renderer);

        ~Application();

        void run();

        void pushLayer(Layer* layer);

    private:
        void onEvent(const Event& event);
    };

}


