#include "Log.h"
#include "Application.h"
#include "Timestep.h"
#include "window/events/KeyEvent.h"
#include "window/events/WindowEvent.h"

namespace storytime
{
    Application::Application(Game* game, Camera* camera, Window* window, Renderer* renderer, ImGuiRenderer* imGuiRenderer)
            : game(game),
              camera(camera),
              window(window),
              renderer(renderer),
              imGuiRenderer(imGuiRenderer),
              lastFrameTime(0.0f),
              running(false),
              minimized(false)
    {
        window->SetOnEventListener([this](Event& event) {
            OnEvent(event);
        });
    }

    void Application::Run()
    {
        ST_LOG_INFO(ST_TAG, "Running...");
        running = true;
        while (running)
        {
            float time = window->GetTime();
            Timestep timestep = time - lastFrameTime;
            lastFrameTime = time;
            game->OnUpdate(timestep);
            if (!minimized)
            {
                renderer->BeginFrame(camera->GetViewProjection());
                game->OnRender();
                renderer->EndFrame();
            }
            window->OnUpdate();
        }
    }

    void Application::Stop()
    {
        ST_LOG_INFO(ST_TAG, "Stopping...");
        running = false;
    }

    void Application::OnEvent(Event& event)
    {
        if (event.GetType() != EventType::MouseMoved)
        {
            ST_LOG_TRACE(ST_TAG, "Received event [{0}]", event.ToString());
        }
        if (event.GetType() == EventType::WindowClose)
        {
            Stop();
            event.SetHandled(true);
        }
        if (event.GetType() == EventType::WindowResize)
        {
            auto* resizeEvent = (WindowResizeEvent*) &event;
            minimized = resizeEvent->GetWidth() == 0 || resizeEvent->GetHeight() == 0;
            renderer->SetViewport(resizeEvent->GetWidth(), resizeEvent->GetHeight());
        }
        if (event.GetType() == EventType::KeyPressed)
        {
            auto* keyEvent = (KeyEvent*) &event;
            if (keyEvent->GetKeyCode() == KeyCode::KEY_ESCAPE)
            {
                Stop();
                event.SetHandled(true);
            }
        }
        if (!event.IsHandled())
        {
            game->OnEvent(event);
        }
    }

}
