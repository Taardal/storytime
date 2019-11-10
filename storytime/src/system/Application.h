#pragma once

#include "window/Window.h"
#include "window/layers/LayerStack.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"

namespace storytime {

    class Application {
    private:
        LayerStack layerStack;
        Window* window;
        Renderer* renderer;
        bool running;
        double lastFrameTime;

    public:
        Application(Window* window, Renderer* renderer);

        ~Application();

        void run();

        void pushLayer(Layer* layer);

    private:
        void onEvent(const Event& event);

        void stop();
    };

}
