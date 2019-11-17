#pragma once

#include "system/Log.h"
#include "system/Application.h"
#include "graphics/GraphicsContext.h"
#include "graphics/OrthographicCamera.h"
#include "graphics/OrthographicCameraController.h"
#include "window/Window.h"
#include "window/Input.h"

namespace storytime {

    class Engine {
    public:
        struct Config {
            LogLevel logLevel;
            Window::Config windowConfig;
            GraphicsContext::Config graphicsConfig;
        };

    private:
        GraphicsContext* graphicsContext;
        Window* window;
        Renderer* renderer;
        Input* input;
        OrthographicCamera* camera;
        OrthographicCameraController* cameraController;
        Application* application;

    public:
        explicit Engine(const Config& config);

        ~Engine();

        void run();
    };

}


