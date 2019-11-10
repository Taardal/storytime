#include "Log.h"
#include "Application.h"
#include "Timestep.h"
#include "window/events/KeyEvent.h"
#include "window/KeyCodes.h"

namespace storytime {

    Application::Application(Window* window, Renderer* renderer, Input* input)
            : window(window), renderer(renderer), input(input), running(false), lastFrameTime(0) {
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
            renderer->beginScene();
            Timestep timestep = window->getTime() - lastFrameTime;
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
        if (event.getType() != EventType::MouseMoved) {
            ST_DEBUG(ST_TAG, "Received event [{0}]", event.toString());
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
                    break;
                }
            }
            default: {
                for (auto iterator = layerStack.end() - 1; iterator != layerStack.begin(); iterator--) {
                    Layer* layer = *iterator;
                    layer->onEvent(event);
                }
            }
        }
    }

}
