#pragma once

namespace Darkle {

    class Application {

    private:
        bool running;

    public:
        explicit Application();

        ~Application();

        void run();

    };

}


