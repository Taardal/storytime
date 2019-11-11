#pragma once

#include "storytime/Storytime.h"

namespace sandbox {

    class Sandbox : public st::Application {
    public:
        Sandbox(st::Window* window, st::Renderer* renderer, st::Input* input, st::OrthographicCameraController* cameraController);
    };

}


