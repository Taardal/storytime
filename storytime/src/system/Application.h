#pragma once

#include "Game.h"
#include "window/Window.h"
#include "window/LayerStack.h"
#include "window/events/Event.h"
#include "graphics/Renderer.h"
#include "graphics/CameraController.h"

namespace storytime
{
    class Application
    {
    private:
        Game* game;
        Camera* camera;
        Window* window;
        Renderer* renderer;
        ImGuiRenderer* imGuiRenderer;
        float lastFrameTime;
        bool running;
        bool minimized;

    public:
        Application(Game* game, Camera* camera, Window* window, Renderer* renderer, ImGuiRenderer* imGuiRenderer);

        void Run();

    private:
        void Start();

        void Stop();

        void OnEvent(Event& event);
    };

}
