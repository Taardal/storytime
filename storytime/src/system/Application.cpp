#include "Log.h"
#include "Application.h"
#include "Timestep.h"
#include "window/events/KeyEvent.h"
#include "window/events/WindowEvent.h"

namespace storytime
{
    Application::Application(Window* window, Renderer* renderer, ImGuiRenderer* imGuiRenderer, OrthographicCameraController* cameraController)
            : window(window), renderer(renderer), imGuiRenderer(imGuiRenderer), cameraController(cameraController)
    {
        window->SetOnEventListener([this](const Event& event) {
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
                renderer->BeginScene(cameraController->GetCamera());
                for (Layer* layer : layerStack)
                {
                    layer->OnUpdate(timestep);
                }
                renderer->EndScene();
                imGuiRenderer->BeginScene();
                for (Layer* layer : layerStack)
                {
                    layer->OnImGuiUpdate();
                }
                imGuiRenderer->EndScene();
            }
            window->OnUpdate();
        }
    }

    void Application::Stop()
    {
        ST_LOG_INFO(ST_TAG, "Stopping...");
        running = false;
    }

    void Application::OnEvent(const Event& event)
    {
        if (event.GetType() != EventType::MouseMoved)
        {
            ST_LOG_TRACE(ST_TAG, "Received event [{0}]", event.ToString());
        }
        switch (event.GetType())
        {
            case EventType::WindowClose:
            {
                Stop();
                break;
            }
            case EventType::KeyPressed:
            {
                auto* keyEvent = (KeyEvent*) &event;
                if (keyEvent->GetKeyCode() == KeyCode::KEY_ESCAPE)
                {
                    Stop();
                }
                break;
            }
            case EventType::WindowResize:
            {
                auto* resizeEvent = (WindowResizeEvent*) &event;
                minimized = resizeEvent->GetWidth() == 0 || resizeEvent->GetHeight() == 0;
                renderer->SetViewport(resizeEvent->GetWidth(), resizeEvent->GetHeight());
                break;
            }
            default:
            {
                OnLayerEvent(event);
            }
        }
    }

    void Application::OnLayerEvent(const Event& event) const
    {
        for (auto iterator = layerStack.end(); iterator != layerStack.begin();)
        {
            Layer* layer = *--iterator;
            layer->OnEvent(event);
        }
    }

}
