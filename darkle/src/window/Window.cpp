#include "Log.h"
#include "Window.h"
#include "window/events/KeyEvent.h"

namespace Darkle {

    Window::Window(const Config& config, GraphicsContext* graphicsContext) : glfwCallbackData({}) {
        LOG_TRACE(TAG, "Creating");
        initGlfw();
        setGlfwWindowHints(graphicsContext);
        glfwWindow = createGlfwWindow(config);
        graphicsContext->init(glfwWindow);
        glfwSetWindowUserPointer(glfwWindow, &glfwCallbackData);
        setGlfwKeyCallbacks();
        LOG_TRACE(TAG, "Created");
    }

    Window::~Window() {
        LOG_TRACE(TAG, "Destroying");
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

    void Window::setGlfwKeyCallbacks() const {
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
                default: {
                    break;
                }
            }
        });
        glfwSetCharCallback(glfwWindow, [](GLFWwindow* glfwWindow, unsigned int keycode) {
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(glfwWindow);
            KeyTypedEvent event(keycode);
            callbackData->onEvent(event);
        });
    }

    void Window::terminateGlfw() const {
        LOG_DEBUG(TAG, "Terminating GLFW");
        glfwTerminate();
        LOG_INFO(TAG, "Terminated GLFW");
    }

}