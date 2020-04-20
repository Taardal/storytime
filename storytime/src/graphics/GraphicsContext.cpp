#include "system/Log.h"
#include "GraphicsContext.h"

namespace storytime
{
    GraphicsContext::GraphicsContext(const Config& config)
            : config(config)
    {
        ST_LOG_TRACE(ST_TAG, "Created");
    }

    GraphicsContext::~GraphicsContext()
    {
        ST_LOG_TRACE(ST_TAG, "Destroyed");
    }

    const GraphicsContext::Config& GraphicsContext::getConfig() const
    {
        return config;
    }

    void GraphicsContext::Init(GLFWwindow* glfwWindow) const
    {
        glfwMakeContextCurrent(glfwWindow);
        InitGlad();
        LogContext();
    }

    void GraphicsContext::InitGlad() const
    {
        ST_LOG_DEBUG(ST_TAG, "Initializing GLAD");
        bool initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
        if (initialized)
        {
            ST_LOG_INFO(ST_TAG, "Initialized GLAD");
        }
        else
        {
            ST_LOG_CRITICAL(ST_TAG, "Could not initialize GLAD");
        }
    }

    void GraphicsContext::LogContext() const
    {
        ST_LOG_INFO(ST_TAG, "Vendor [{0}]", glGetString(GL_VENDOR));
        ST_LOG_INFO(ST_TAG, "Renderer [{0}]", glGetString(GL_RENDERER));
        ST_LOG_INFO(ST_TAG, "Version [{0}]", glGetString(GL_VERSION));
    }

}