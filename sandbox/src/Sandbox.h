#pragma once

#include "storytime/Storytime.h"

namespace sandbox {

    class Sandbox : public storytime::Application {
    public:
        Sandbox(storytime::Window* window, storytime::Renderer* renderer);
    };

}


