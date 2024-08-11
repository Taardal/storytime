#include "log.h"
#include "Application.h"
#include "Timestep.h"
#include "window/events/KeyEvent.h"
#include "window/events/WindowEvent.h"

namespace Storytime
{
    Application::Application(Window* window, Input* input, Renderer* renderer, ImGuiRenderer* imGuiRenderer,
                             OrthographicCameraController* cameraController)
            : window(window),
              input(input),
              renderer(renderer),
              imGuiRenderer(imGuiRenderer),
              cameraController(cameraController),
              layerStack(),
              lastFrameTime(0.0f),
              running(false),
              minimized(false)
    {
        window->SetOnEventListener([this](Event& event) {
            OnEvent(event);
        });
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    Application::~Application()
    {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void Application::PushLayer(Layer* layer)
    {
        layerStack.PushLayer(layer);
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
            if (!minimized)
            {
                for (Layer* layer : layerStack)
                {
                    layer->OnUpdate(timestep, input, renderer);
                }
                imGuiRenderer->Begin();
                for (Layer* layer : layerStack)
                {
                    layer->OnImGuiRender(imGuiRenderer);
                }
                imGuiRenderer->End(window->GetSize().Width, window->GetSize().Height);
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
        for (auto iterator = layerStack.rbegin(); iterator != layerStack.rend(); ++iterator)
        {
            if (event.IsHandled())
            {
                break;
            }
            Layer* layer = *iterator;
            layer->OnEvent(event);
        }
    }

}
