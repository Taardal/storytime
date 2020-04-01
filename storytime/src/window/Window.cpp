#include "system/Core.h"
#include "system/Log.h"
#include "Window.h"
#include "window/events/WindowEvent.h"
#include "window/events/KeyEvent.h"
#include "window/events/MouseEvent.h"

namespace storytime {

    float Window::Config::getAspectRatio() const {
        return (float) width / (float) height;
    }

    Window::Window(const Config& config, GraphicsContext* graphicsContext)
            : config(config) {
        ST_TRACE(ST_TAG, "Creating");
        initGlfw();
        setGlfwWindowHints(graphicsContext);
        glfwWindow = createGlfwWindow();
        graphicsContext->init(glfwWindow);
        setGlfwCallbacks();
        ST_TRACE(ST_TAG, "Created");
    }

    Window::~Window() {
        ST_TRACE(ST_TAG, "Destroying");
        destroyGlfwWindow();
        terminateGlfw();
        ST_TRACE(ST_TAG, "Destroyed");
    }

    const Window::Config& Window::getConfig() const {
        return config;
    }

    void Window::setOnEventListener(const std::function<void(const Event&)>& onEvent) {
        glfwCallbackData.onEvent = onEvent;
    }

    float Window::getTime() const {
        return (float) glfwGetTime();
    }

    void Window::onUpdate() const {
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow);
    }

    void Window::initGlfw() const {
        ST_DEBUG(ST_TAG, "Initializing GLFW");
        if (glfwInit()) {
            glfwSetErrorCallback(onGlfwError);
            ST_INFO(ST_TAG, "Initialized GLFW");
        } else {
            ST_CRITICAL(ST_TAG, "Could not initialize GLFW");
        }
    }

    void Window::onGlfwError(int error, const char* description) {
        ST_ERROR(ST_TAG_TYPE(Window), "GLFW error [{0}: {1}]", error, description);
    }

    void Window::setGlfwWindowHints(GraphicsContext* graphicsContext) const {
        int versionMajor = graphicsContext->getConfig().versionMajor;
        int versionMinor = graphicsContext->getConfig().versionMinor;
        ST_DEBUG(ST_TAG, "Setting GLFW context version [{0}.{1}]", versionMajor, versionMinor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef ST_PLATFORM_MACOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif
    }

    GLFWwindow* Window::createGlfwWindow() const {
        ST_DEBUG(ST_TAG, "Creating GLFW window [{0}, {1}x{2}]", config.title, config.width, config.height);
        GLFWwindow* glfwWindow = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);
        if (glfwWindow != nullptr) {
            ST_INFO(ST_TAG, "Created GLFW window");
        } else {
            ST_ERROR(ST_TAG, "Could not create GLFW window");
        }
        return glfwWindow;
    }

    void Window::setGlfwCallbacks() {
        ST_DEBUG(ST_TAG, "Setting GLFW window callback data");
        glfwSetWindowUserPointer(glfwWindow, &glfwCallbackData);
        setGlfwKeyCallbacks();
        setGlfwWindowCallbacks();
        setGlfwMouseCallbacks();
    }

    void Window::setGlfwWindowCallbacks() const {
        ST_DEBUG(ST_TAG, "Setting GLFW window callbacks");
        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* glfwWindow) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            WindowCloseEvent event;
            callbackData->onEvent(event);
        });
        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* glfwWindow, int width, int height) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            WindowResizeEvent event(width, height);
            callbackData->onEvent(event);
        });
    }

    void Window::setGlfwKeyCallbacks() const {
        ST_DEBUG(ST_TAG, "Setting GLFW key callbacks");
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* glfwWindow, int key, int scanCode, int action, int mods) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key);
                    callbackData->onEvent(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    callbackData->onEvent(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyRepeatedEvent event(key);
                    callbackData->onEvent(event);
                    break;
                }
                default: {
                }
            }
        });
        glfwSetCharCallback(glfwWindow, [](GLFWwindow* glfwWindow, unsigned int keyCode) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            KeyTypedEvent event(keyCode);
            callbackData->onEvent(event);
        });
    }

    void Window::setGlfwMouseCallbacks() const {
        ST_DEBUG(ST_TAG, "Setting GLFW mouse callbacks");
        glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* glfwWindow, int button, int action, int mods) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    callbackData->onEvent(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    callbackData->onEvent(event);
                    break;
                }
                default: {
                }
            }
        });
        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double x, double y) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            MouseMovedEvent event((float) x, (float) y);
            callbackData->onEvent(event);
        });
        glfwSetScrollCallback(glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            callbackData->onEvent(event);
        });
    }

    void Window::destroyGlfwWindow() const {
        ST_DEBUG(ST_TAG, "Destroying GLFW window");
        glfwDestroyWindow(glfwWindow);
        ST_INFO(ST_TAG, "Destroyed GLFW window");
    }

    void Window::terminateGlfw() const {
        ST_DEBUG(ST_TAG, "Terminating GLFW");
        glfwTerminate();
        ST_INFO(ST_TAG, "Terminated GLFW");
    }

}