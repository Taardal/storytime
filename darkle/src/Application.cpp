#include "Log.h"
#include "Application.h"
#include "window/events/KeyEvent.h"
#include "window/input/KeyCodes.h"
#include <glad/glad.h>

namespace Darkle {

    Application::Application(Window* window) : window(window), running(false) {
        LOG_TRACE(TAG, "Creating");
        window->setOnEventListener([this](const Event& event) {
            onEvent(event);
        });
        LOG_TRACE(TAG, "Created");
    }

    Application::~Application() {
        LOG_TRACE(TAG, "Destroyed");
    }

    void Application::run() {
        LOG_INFO(TAG, "Running...");
        running = true;
        while (running) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            window->onUpdate();
        }
    }

    void Application::onEvent(const Event& event) {
        LOG_DEBUG(TAG, "Received event [{0}]", event.toString());
        if (event.getType() == EventType::KeyPressed) {
            auto* keyEvent = (KeyEvent*) &event;
            if (keyEvent->getKeyCode() == KeyCode::KEY_ESCAPE) {
                LOG_INFO(TAG, "Stopping...");
                running = false;
            }
        }
    }

}
