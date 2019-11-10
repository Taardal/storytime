#include "system/Log.h"
#include "GraphicsContext.h"
#include <glad/glad.h>

namespace storytime {

    GraphicsContext::GraphicsContext(const Config& config) : config(config) {
        ST_TRACE(ST_TAG, "Created");
    }

    GraphicsContext::~GraphicsContext() {
        ST_TRACE(ST_TAG, "Destroyed");
    }

    const GraphicsContext::Config& GraphicsContext::getConfig() const {
        return config;
    }

    void GraphicsContext::init(GLFWwindow* glfwWindow) const {
        glfwMakeContextCurrent(glfwWindow);
        initGlad();
        logContext();
    }

    void GraphicsContext::initGlad() const {
        ST_DEBUG(ST_TAG, "Initializing GLAD");
        bool initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
        if (initialized) {
            ST_INFO(ST_TAG, "Initialized GLAD");
        } else {
            ST_CRITICAL(ST_TAG, "Could not initialize GLAD");
        }
    }

    void GraphicsContext::logContext() const {
        ST_INFO(ST_TAG, "Vendor [{0}]", glGetString(GL_VENDOR));
        ST_INFO(ST_TAG, "Renderer [{0}]", glGetString(GL_RENDERER));
        ST_INFO(ST_TAG, "Version [{0}]", glGetString(GL_VERSION));
    }

}