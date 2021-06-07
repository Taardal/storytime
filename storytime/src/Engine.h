#pragma once

#include "system/Log.h"
#include "system/Application.h"
#include "system/FileSystem.h"
#include "system/ResourceLoader.h"
#include "graphics/GraphicsContext.h"
#include "graphics/Renderer.h"
#include "graphics/ImGuiRenderer.h"
#include "graphics/Camera.h"
#include "graphics/CameraController.h"
#include "window/Window.h"
#include "window/Input.h"
#include "Game.h"

namespace storytime
{
    class Engine
    {
    public:
        struct Config
        {
            LogLevel logLevel;
            Window::Config windowConfig;
            GraphicsContext::Config graphicsConfig;
        };

    private:
        FileSystem* fileSystem;
        ResourceLoader* resourceLoader;
        GraphicsContext* graphicsContext;
        Window* window;
        Renderer* renderer;
        ImGuiRenderer* imGuiRenderer;
        Input* input;
        Camera* camera;
        CameraController* cameraController;
        Game* game;
        Application* application;

    public:
        explicit Engine(const Config& config);

        ~Engine();

        void Run() const;
    };

}


