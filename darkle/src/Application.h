#pragma once

#include "window/Window.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"

namespace Darkle {

    class Application {
    private:
        Window* window;
        Renderer* renderer;
        bool running;

    public:
        Application(Window* window, Renderer* renderer);

        ~Application();

        void run();

    private:
        void onEvent(const Event& event);

    };

}


