#include "Window.h"
#include "Log.h"
#include <glad/glad.h>

namespace Darkle {

    Window::Window(const Config& config, GraphicsContext* graphicsContext) : glfwCallbackData({}) {
        LOG_TRACE(TAG, "Creating");
        initGlfw();
        setGlfwWindowHints(config);
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

    void Window::setOnEventListener(const std::function<void(bool)>& onEvent) {
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

    void Window::setGlfwWindowHints(const Config& config) const {
        LOG_DEBUG(TAG, "Setting GLFW context version [{0}.{1}]", config.contextMajorVersion, config.contextMinorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.contextMajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.contextMinorVersion);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }

    GLFWwindow* Window::createGlfwWindow(const Config& config) const {
        LOG_DEBUG(TAG, "Creating GLFW window [{0}, {1}x{2}]", config.title, config.width, config.height);
        GLFWwindow* glfwWindow = glfwCreateWindow(
                config.width,
                config.height,
                config.title,
                nullptr,
                nullptr
        );
        if (glfwWindow != nullptr) {
            LOG_INFO(TAG, "Created GLFW window");
        } else {
            LOG_ERROR(TAG, "Could not create GLFW window");
        }
        return glfwWindow;
    }

    void Window::setGlfwKeyCallbacks() const {
        glfwSetKeyCallback(glfwWindow, [](GLFWwindow* window, int key, int scanCode, int action, int mode) {
            bool shouldClose = key == GLFW_KEY_ESCAPE && action == GLFW_PRESS;
            auto* callbackData = (GlfwCallbackData*) glfwGetWindowUserPointer(window);
            callbackData->onEvent(shouldClose);
        });
    }

    void Window::terminateGlfw() const {
        LOG_DEBUG(TAG, "Terminating GLFW");
        glfwTerminate();
        LOG_INFO(TAG, "Terminated GLFW");
    }

}