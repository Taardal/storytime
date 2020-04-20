#include <window/events/WindowEvent.h>
#include "Log.h"
#include "Application.h"
#include "Timestep.h"
#include "window/events/KeyEvent.h"

namespace storytime {

    Application::Application(Window* window, Renderer* renderer, Input* input, OrthographicCameraController* cameraController)
            : window(window), renderer(renderer), input(input), cameraController(cameraController) {
        ST_LOG_TRACE(ST_TAG, "Creating");
        window->setOnEventListener([this](const Event& event) {
            onEvent(event);
        });
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    Application::~Application() {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    void Application::pushLayer(Layer* layer) {
        layerStack.pushLayer(layer);
    }

    void Application::run() {
        ST_LOG_INFO(ST_TAG, "Running...");
        running = true;
        while (running) {
            float time = window->getTime();
            Timestep timestep = time - lastFrameTime;
            lastFrameTime = time;
            if (!minimized) {
                renderer->BeginScene(cameraController->getCamera());
                for (Layer* layer : layerStack) {
                    layer->OnUpdate(timestep, renderer, input);
                }
                renderer->EndScene();
            }
            window->onUpdate();
        }
    }

    void Application::stop() {
        ST_LOG_INFO(ST_TAG, "Stopping...");
        running = false;
    }

    void Application::onEvent(const Event& event) {
        if (event.getType() != EventType::MouseMoved) {
            ST_LOG_TRACE(ST_TAG, "Received event [{0}]", event.toString());
        }
        switch (event.getType()) {
            case EventType::WindowClose: {
                stop();
                break;
            }
            case EventType::KeyPressed: {
                auto* keyEvent = (KeyEvent*) &event;
                if (keyEvent->getKeyCode() == KeyCode::KEY_ESCAPE) {
                    stop();
                }
                break;
            }
            case EventType::WindowResize: {
                auto* resizeEvent = (WindowResizeEvent*) &event;
                minimized = resizeEvent->getWidth() == 0 || resizeEvent->getHeight() == 0;
                renderer->SetViewport(resizeEvent->getWidth(), resizeEvent->getHeight());
                break;
            }
            default: onLayerEvent(event);
        }
    }

    void Application::onLayerEvent(const Event& event) const {
        for (auto iterator = layerStack.end(); iterator != layerStack.begin();) {
            Layer* layer = *--iterator;
            layer->OnEvent(event);
        }
    }

}
