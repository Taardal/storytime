#include "Application.h"
#include "Log.h"

namespace Darkle {

    Application::Application() : running(false) {
        LOG_TRACE(TAG, "Created");
    }

    Application::~Application() {
        LOG_TRACE(TAG, "Destroyed");
    }

    void Application::run() {
        LOG_INFO(TAG, "Running");
        running = true;
    }

}
