#include "system/log.h"
#include "GraphicsContext.h"

namespace Storytime
{
    GraphicsContext::GraphicsContext(const Config& config)
            : config(config)
    {
        ST_LOG_TRACE("Initializing GLAD");
        ST_ASSERT_THROW(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0);
        ST_LOG_DEBUG("Initialized GLAD");
        LogContext();
    }

    GraphicsContext::~GraphicsContext()
    {
        ST_LOG_TRACE("Destroyed");
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
        ST_LOG_DEBUG("Initializing GLAD");
        bool initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) != 0;
        if (initialized)
        {
            ST_LOG_INFO("Initialized GLAD");
        }
        else
        {
            ST_LOG_CRITICAL("Could not initialize GLAD");
        }
    }

    void GraphicsContext::LogContext() const
    {
        ST_LOG_INFO("Vendor [{0}]", glGetString(GL_VENDOR));
        ST_LOG_INFO("Renderer [{0}]", glGetString(GL_RENDERER));
        ST_LOG_INFO("Version [{0}]", glGetString(GL_VERSION));
    }

}