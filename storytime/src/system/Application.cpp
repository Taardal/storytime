#include "Log.h"
#include "Application.h"
#include "Timestep.h"
#include "window/events/KeyEvent.h"

namespace storytime {

    Application::Application(Window* window, Renderer* renderer, Input* input, OrthographicCameraController* cameraController)
            : window(window), renderer(renderer), input(input), cameraController(cameraController) {
        ST_TRACE(ST_TAG, "Creating");
        window->setOnEventListener([this](const Event& event) {
            onEvent(event);
        });
        ST_TRACE(ST_TAG, "Created");
    }

    Application::~Application() {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    void Application::pushLayer(Layer* layer) {
        layerStack.pushLayer(layer);
    }

    void Application::run() {
        ST_INFO(ST_TAG, "Running...");
        running = true;
        while (running) {

            float time = window->getTime();
            Timestep timestep = time - lastFrameTime;
            lastFrameTime = time;

            cameraController->onUpdate(timestep, input);
            renderer->beginScene(cameraController->getCamera());
            for (Layer* layer : layerStack) {
                layer->onUpdate(timestep, renderer, input);
            }

            window->onUpdate();
        }
    }

    void Application::stop() {
        ST_INFO(ST_TAG, "Stopping...");
        running = false;
    }

    void Application::onEvent(const Event& event) {
        ST_TRACE(ST_TAG, "Received event [{0}]", event.toString());
        switch (event.getType()) {
            case EventType::WindowClose: {
                stop();
                break;
            }
            case EventType::KeyPressed: {
                auto* keyEvent = (KeyEvent*) &event;
                if (keyEvent->getKeyCode() == KeyCode::KEY_ESCAPE) {
                    stop();
                    break;
                }
            }
            default: {
                cameraController->onEvent(event);
                for (auto iterator = layerStack.end() - 1; iterator != layerStack.begin(); iterator--) {
                    Layer* layer = *iterator;
                    layer->onEvent(event);
                }
            }
        }
    }

}
