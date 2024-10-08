#include "window.h"

#include "system/assert.h"
#include "key_event.h"
#include "mouse_event.h"
#include "window/window_event.h"

namespace Storytime {
    Window::Window(const WindowConfig& config) : config(config) {
        ST_LOG_TRACE("Initializing GLFW");
        if (!glfwInit()) {
            ST_THROW("Could not initialize GLFW");
        }
        ST_LOG_DEBUG("Initialized GLFW");
        glfwSetErrorCallback(on_glfw_error);

        ST_LOG_TRACE("Setting GLFW window hints");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.context_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.context_version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, config.maximized);
        glfwWindowHint(GLFW_RESIZABLE, config.resizable);
#ifdef ST_PLATFORM_MACOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif
        ST_LOG_DEBUG("GLFW context version [{0}.{1}]", config.context_version_major, config.context_version_minor);

        ST_LOG_TRACE("Creating GLFW window");
        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        glfw_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), fullscreenMonitor, sharedWindow);
        if (glfw_window == nullptr) {
            ST_THROW("Could not create GLFW window");
        }
        ST_LOG_DEBUG("Created GLFW window [{0}, {1}x{2}]", config.title, config.width, config.height);

        glfwMakeContextCurrent(glfw_window);
        glfwSetWindowUserPointer(glfw_window, this);

        glfwSetFramebufferSizeCallback(glfw_window, on_framebuffer_size_change);
        glfwSetKeyCallback(glfw_window, on_key_change);
        glfwSetMouseButtonCallback(glfw_window, on_mouse_button_change);
        glfwSetScrollCallback(glfw_window, on_mouse_scroll_change);
        glfwSetWindowCloseCallback(glfw_window, on_window_close_change);
        glfwSetWindowIconifyCallback(glfw_window, on_window_iconify_change);

        ST_LOG_INFO("Created window [{0}, {1}x{2}]", config.title, config.width, config.height);
    }

    Window::~Window() {
        glfwDestroyWindow(glfw_window);
        ST_LOG_DEBUG("Destroyed GLFW window");
        glfwTerminate();
        ST_LOG_DEBUG("Terminated GLFW");
    }

    Window::operator GLFWwindow*() const {
        ST_ASSERT(glfw_window != nullptr);
        return glfw_window;
    }

    void Window::update() const {
        glfwPollEvents();
        glfwSwapBuffers(glfw_window);
    }

    void Window::set_title(const char* title) const {
        glfwSetWindowTitle(glfw_window, title);
    }

    WindowSize Window::get_size_in_pixels() const  {
        i32 width = 0;
        i32 height = 0;
        get_size_in_pixels(&width, &height);
        return { width, height };
    }

    void Window::get_size_in_pixels(i32* width, i32* height) const {
        glfwGetFramebufferSize(glfw_window, width, height);
    }

    f32 Window::get_aspect_ratio() const {
        auto [width, height] = get_size_in_pixels();
        return static_cast<f32>(width) / static_cast<f32>(height);
    }

    f64 Window::get_time() const {
        return glfwGetTime(); // Seconds since initialization
    }

    void Window::on_glfw_error(i32 error, const char* description) {
        ST_LOG_ERROR("GLFW error [{0}: {1}]", error, description);
    }

    void Window::on_framebuffer_size_change(GLFWwindow* glfw_window, i32 width, i32 height) {
        WindowResizeEvent event(width, height);
        on_event(glfw_window, EventType::WindowResize, event);
    }

    void Window::on_key_change(GLFWwindow* glfw_window, i32 key, i32 scanCode, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            KeyPressedEvent event(key, mods, scanCode);
            on_event(glfw_window, EventType::KeyPressed, event);
        } else if (action == GLFW_RELEASE) {
            KeyReleasedEvent event(key, mods, scanCode);
            on_event(glfw_window, EventType::KeyReleased, event);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatedEvent event(key, mods, scanCode);
            on_event(glfw_window, EventType::KeyRepeated, event);
        }
    }

    void Window::on_mouse_button_change(GLFWwindow* glfw_window, i32 button, i32 action, i32 mods) {
        if (action == GLFW_PRESS) {
            MouseButtonPressedEvent event(button);
            on_event(glfw_window, EventType::MouseButtonPressed, event);
        }
        if (action == GLFW_RELEASE) {
            MouseButtonReleasedEvent event(button);
            on_event(glfw_window, EventType::MouseButtonReleased, event);
        }
    }

    void Window::on_mouse_scroll_change(GLFWwindow* glfw_window, f64 xoffset, f64 yoffset) {
        MouseScrollEvent event(xoffset, yoffset);
        on_event(glfw_window, EventType::MouseScroll, event);
    }

    void Window::on_window_close_change(GLFWwindow* glfw_window) {
        WindowCloseEvent event{};
        on_event(glfw_window, EventType::WindowClose, event);
    }

    void Window::on_window_iconify_change(GLFWwindow* glfw_window, i32 iconified) {
        bool minimized = iconified == 1;
        WindowMinimizeEvent event(minimized);
        on_event(glfw_window, EventType::WindowMinimize, event);
    }

    void Window::on_event(GLFWwindow* glfw_window, EventType event_type, const Event& event) {
        ST_LOG_TRACE(event.to_string());
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr);
        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr);
        event_manager->trigger_event(event_type, event);
    }
}
