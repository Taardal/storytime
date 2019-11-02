#include "Application.h"
#include "Log.h"
#include <glad/glad.h>

namespace Darkle {

    Application::Application(Window* window) : window(window), running(false) {
        LOG_TRACE(TAG, "Creating");
        window->setOnEventListener([this](bool shouldClose) {
            onEvent(shouldClose);
        });
        LOG_TRACE(TAG, "Created");
    }

    Application::~Application() {
        LOG_TRACE(TAG, "Destroyed");
    }

    void Application::run() {
        LOG_INFO(TAG, "Running");
        running = true;
        while (running) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            window->onUpdate();
        }
    }

    void Application::onEvent(bool shouldClose) {
        LOG_TRACE(TAG, "OnEvent");
        running = !shouldClose;
    }

}
