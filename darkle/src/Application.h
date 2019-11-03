#pragma once

#include "window/Window.h"
#include "window/events/Event.h"

namespace Darkle {

    class Application {
    private:
        Window* window;
        bool running;

    public:
        explicit Application(Window* window);

        ~Application();

        void run();

    private:
        void onEvent(const Event& event);

    };

}


