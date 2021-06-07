#include "system/Core.h"
#include "system/Log.h"
#include "Window.h"
#include "window/events/WindowEvent.h"
#include "window/events/KeyEvent.h"
#include "window/events/MouseEvent.h"

namespace storytime
{
    Window::Config::Config()
            : Title("Storytime"),
              Width(800),
              Height(600),
              Maximized(false)
    {}

    float Window::Config::GetAspectRatio() const
    {
        return (float) Width / (float) Height;
    }
}

namespace storytime
{
    Window::Window(const Config& config, GraphicsContext* graphicsContext, ImGuiRenderer* imGuiRenderer)
            : config(config),
              glfwCallbackData(),
              glfwWindow(nullptr)
    {
        InitGlfw();
        SetGlfwWindowHints(graphicsContext);
        glfwWindow = CreateGlfwWindow();
        graphicsContext->Init(glfwWindow);
        imGuiRenderer->Init(glfwWindow);
        SetGlfwCallbacks();
    }

    Window::~Window()
    {
        DestroyGlfwWindow();
        TerminateGlfw();
    }

    Window::Size Window::GetSize() const
    {
        int32_t width;
        int32_t height;
        glfwGetWindowSize(glfwWindow, &width, &height);
        return { width, height };
    }

    float Window::GetTime() const
    {
        return (float) glfwGetTime();
    }

    void Window::SetOnEventListener(const std::function<void(Event&)>& onEvent)
    {
        glfwCallbackData.OnEvent = onEvent;
    }

    void Window::OnUpdate() const
    {
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow);
    }

    void Window::InitGlfw() const
    {
        ST_LOG_DEBUG("Initializing GLFW");
        if (glfwInit())
        {
            glfwSetErrorCallback(OnGlfwError);
            ST_LOG_INFO("Initialized GLFW");
        }
        else
        {
            ST_LOG_CRITICAL("Could not initialize GLFW");
        }
    }

    void Window::OnGlfwError(int32_t error, const char* description)
    {
        ST_LOG_ERROR("GLFW error [{0}: {1}]", error, description);
    }

    void Window::SetGlfwWindowHints(GraphicsContext* graphicsContext) const
    {
        const GraphicsContext::Config& graphicsConfig = graphicsContext->getConfig();
        ST_LOG_DEBUG("Setting GLFW context version [{0}.{1}]", graphicsConfig.OpenGLVersionMajor, graphicsConfig.OpenGLVersionMinor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, graphicsConfig.OpenGLVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, graphicsConfig.OpenGLVersionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, this->config.Maximized ? GLFW_TRUE : GLFW_FALSE);
#ifdef ST_PLATFORM_MACOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif
    }

    GLFWwindow* Window::CreateGlfwWindow() const
    {
        ST_LOG_DEBUG("Creating GLFW window [{0}, {1}x{2}]", config.Title, config.Width, config.Height);
        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        GLFWwindow* glfwWindow = glfwCreateWindow(config.Width, config.Height, config.Title, fullscreenMonitor, sharedWindow);
        if (glfwWindow != nullptr)
        {
            ST_LOG_INFO("Created GLFW window");
        }
        else
        {
            ST_LOG_ERROR("Could not create GLFW window");
        }
        return glfwWindow;
    }

    void Window::SetGlfwCallbacks()
    {
        ST_LOG_DEBUG("Setting GLFW window callback data");
        glfwSetWindowUserPointer(glfwWindow, &glfwCallbackData);
        SetGlfwKeyCallbacks();
        SetGlfwWindowCallbacks();
        SetGlfwMouseCallbacks();
    }

    void Window::SetGlfwWindowCallbacks() const
    {
        ST_LOG_DEBUG("Setting GLFW window callbacks");
        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* glfwWindow) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            WindowCloseEvent event;
            callbackData->OnEvent(event);
        });
        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* glfwWindow, int32_t width, int32_t height) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            WindowResizeEvent event(width, height);
            callbackData->OnEvent(event);
        });
    }

    void Window::SetGlfwKeyCallbacks() const
    {
        ST_LOG_DEBUG("Setting GLFW key callbacks");
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* glfwWindow, int32_t key, int32_t scanCode, int32_t action, int32_t mods) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key);
                    callbackData->OnEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    callbackData->OnEvent(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyRepeatedEvent event(key);
                    callbackData->OnEvent(event);
                    break;
                }
                default:
                {
                }
            }
        });
        glfwSetCharCallback(glfwWindow, [](GLFWwindow* glfwWindow, uint32_t keyCode) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            KeyTypedEvent event(keyCode);
            callbackData->OnEvent(event);
        });
    }

    void Window::SetGlfwMouseCallbacks() const
    {
        ST_LOG_DEBUG("Setting GLFW mouse callbacks");
        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* glfwWindow, int32_t button, int32_t action, int32_t mods) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    callbackData->OnEvent(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    callbackData->OnEvent(event);
                    break;
                }
                default:
                {
                }
            }
        });
        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double x, double y) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            MouseMovedEvent event((float) x, (float) y);
            callbackData->OnEvent(event);
        });
        glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            callbackData->OnEvent(event);
        });
    }

    void Window::DestroyGlfwWindow() const
    {
        ST_LOG_DEBUG("Destroying GLFW window");
        glfwDestroyWindow(glfwWindow);
        ST_LOG_INFO("Destroyed GLFW window");
    }

    void Window::TerminateGlfw() const
    {
        ST_LOG_DEBUG("Terminating GLFW");
        glfwTerminate();
        ST_LOG_INFO("Terminated GLFW");
    }

}