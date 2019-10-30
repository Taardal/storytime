#include "GraphicsContext.h"
#include "Log.h"
#include <glad/glad.h>

namespace Darkle {

    GraphicsContext::GraphicsContext() {
        LOG_TRACE(TAG, "Creating");
        LOG_TRACE(TAG, "Created");
    }

    GraphicsContext::~GraphicsContext() {
        LOG_TRACE(TAG, "Destroying");
        LOG_TRACE(TAG, "Destroyed");
    }

    void GraphicsContext::init(GLFWwindow* glfwWindow) const {
        glfwMakeContextCurrent(glfwWindow);
        initGlad();
        logContext();
    }

    void GraphicsContext::initGlad() const {
        LOG_DEBUG(TAG, "Initializing GLAD");
        bool initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
        if (initialized) {
            LOG_INFO(TAG, "Initialized GLAD");
        } else {
            LOG_CRITICAL(TAG, "Could not initialize GLAD");
        }
    }

    void GraphicsContext::logContext() const {
        LOG_INFO(TAG, "Vendor [{0}]", glGetString(GL_VENDOR));
        LOG_INFO(TAG, "Renderer [{0}]", glGetString(GL_RENDERER));
        LOG_INFO(TAG, "Version [{0}]", glGetString(GL_VERSION));
    }

}