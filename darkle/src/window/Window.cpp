#include "Log.h"
#include "Window.h"
#include "window/events/WindowEvent.h"
#include "window/events/KeyEvent.h"
#include "window/events/MouseEvent.h"

namespace Darkle {

    Window::Window(const Config& config, GraphicsContext* graphicsContext) : glfwCallbackData({}) {
        LOG_TRACE(TAG, "Creating");
        initGlfw();
        setGlfwWindowHints(graphicsContext);
        glfwWindow = createGlfwWindow(config);
        graphicsContext->init(glfwWindow);
        setGlfwCallbacks();
        LOG_TRACE(TAG, "Created");
    }

    Window::~Window() {
        LOG_TRACE(TAG, "Destroying");
        destroyGlfwWindow();
        terminateGlfw();
        LOG_TRACE(TAG, "Destroyed");
    }

    void Window::setOnEventListener(const std::function<void(const Event&)>& onEvent) {
        glfwCallbackData.onEvent = onEvent;
    }

    void Window::onUpdate() const {
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow);
    }

    void Window::initGlfw() const {
        LOG_DEBUG(TAG, "Initializing GLFW");
        if (glfwInit()) {
            LOG_INFO(TAG, "Initialized GLFW");
        } else {
            LOG_CRITICAL(TAG, "Could not initialize GLFW");
        }
    }

    void Window::setGlfwWindowHints(GraphicsContext* graphicsContext) const {
        int versionMajor = graphicsContext->getConfig().openGLVersionMajor;
        int versionMinor = graphicsContext->getConfig().openGLVersionMinor;
        LOG_DEBUG(TAG, "Setting GLFW context version [{0}.{1}]", versionMajor, versionMinor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef APPLE
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    GLFWwindow* Window::createGlfwWindow(const Config& config) const {
        LOG_DEBUG(TAG, "Creating GLFW window [{0}, {1}x{2}]", config.title, config.width, config.height);
        GLFWwindow* glfwWindow = glfwCreateWindow(config.width, config.height, config.title, nullptr, nullptr);
        if (glfwWindow != nullptr) {
            LOG_INFO(TAG, "Created GLFW window");
        } else {
            LOG_ERROR(TAG, "Could not create GLFW window");
        }
        return glfwWindow;
    }

    void Window::setGlfwCallbacks() {
        LOG_DEBUG(TAG, "Setting GLFW window callback data");
        glfwSetWindowUserPointer(glfwWindow, &glfwCallbackData);
        setGlfwKeyCallbacks();
        setGlfwWindowCallbacks();
        setGlfwMouseCallbacks();
    }

    void Window::setGlfwWindowCallbacks() const {
        LOG_DEBUG(TAG, "Setting GLFW window callbacks");
        glfwSetWindowCloseCallback(glfwWindow, [](GLFWwindow* glfwWindow) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            WindowCloseEvent event;
            callbackData->onEvent(event);
        });
        glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* glfwWindow, int width, int height) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            callbackData->width = width;
            callbackData->height = height;
            WindowResizeEvent event(width, height);
            callbackData->onEvent(event);
        });
    }

    void Window::setGlfwKeyCallbacks() const {
        LOG_DEBUG(TAG, "Setting GLFW key callbacks");
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
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
                default: {}
            }
        });
        glfwSetCharCallback(glfwWindow, [](GLFWwindow* glfwWindow, unsigned int keycode) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            KeyTypedEvent event(keycode);
            callbackData->onEvent(event);
        });
    }

    void Window::setGlfwMouseCallbacks() const {
        LOG_DEBUG(TAG, "Setting GLFW mouse callbacks");
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
                default: {}
            }
        });
        glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double xpos, double ypos) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            MouseMovedEvent event((float) xpos, (float) ypos);
            callbackData->onEvent(event);
        });
    }

    void Window::destroyGlfwWindow() const {
        LOG_DEBUG(TAG, "Destroying GLFW window");
        glfwDestroyWindow(glfwWindow);
        LOG_INFO(TAG, "Destroyed GLFW window");
    }

    void Window::terminateGlfw() const {
        LOG_DEBUG(TAG, "Terminating GLFW");
        glfwTerminate();
        LOG_INFO(TAG, "Terminated GLFW");
    }

}