#pragma once

#include "Window.h"

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
        void onEvent(bool running);

    };

}


