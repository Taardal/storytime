#include "st_window.h"

#include "event/st_key_pressed_event.h"
#include "event/st_key_released_event.h"
#include "event/st_key_repeated_event.h"
#include "event/st_mouse_button_pressed_event.h"
#include "event/st_mouse_button_released_event.h"
#include "event/st_mouse_moved_event.h"
#include "event/st_mouse_scroll_event.h"
#include "event/st_window_closed_event.h"
#include "event/st_window_focused_event.h"
#include "event/st_window_minimized_event.h"
#include "event/st_window_resized_event.h"

namespace Storytime {
    Window::Window(const WindowConfig& config) : config(config) {
        ST_ASSERT(config.width > 0, "Window width must be greater than zero");
        i32 width = config.width;

        ST_ASSERT(config.height > 0 || config.aspect_ratio > 0.0f, "Window height or aspect ratio must be greater than zero");
        i32 height = config.aspect_ratio > 0.0f ? (f32) config.width / config.aspect_ratio : config.height;

        ST_LOG_TRACE("Initializing GLFW");
        if (!glfwInit()) {
            ST_THROW("Could not initialize GLFW");
        }
        ST_LOG_DEBUG("Initialized GLFW");

        glfwSetErrorCallback(on_glfw_error);

        ST_LOG_TRACE("Setting GLFW window hints");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_MAXIMIZED, config.maximized);
        glfwWindowHint(GLFW_RESIZABLE, config.resizable);

#ifdef ST_PLATFORM_MACOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif
        ST_LOG_DEBUG("GLFW context version [{0}.{1}]", config.context_version_major, config.context_version_minor);

        GLFWmonitor* fullscreen_monitor = nullptr;
        if (config.fullscreen) {
            fullscreen_monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* video_mode = glfwGetVideoMode(fullscreen_monitor);
            glfwWindowHint(GLFW_RED_BITS, video_mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, video_mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, video_mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, video_mode->refreshRate);
        }

        GLFWwindow* shared_window = nullptr;

        ST_LOG_TRACE("Creating GLFW window");
        glfw_window = glfwCreateWindow(width, height, config.title.c_str(), fullscreen_monitor, shared_window);
        if (glfw_window == nullptr) {
            ST_THROW("Could not create GLFW window");
        }
        ST_LOG_DEBUG("Created GLFW window [{0}, {1}x{2}]", config.title, width, height);

        glfwSetWindowUserPointer(glfw_window, this);
        glfwSetCursorPosCallback(glfw_window, on_cursor_position_change);
        glfwSetFramebufferSizeCallback(glfw_window, on_framebuffer_size_change);
        glfwSetKeyCallback(glfw_window, on_key_change);
        glfwSetMouseButtonCallback(glfw_window, on_mouse_button_change);
        glfwSetScrollCallback(glfw_window, on_mouse_scroll_change);
        glfwSetWindowCloseCallback(glfw_window, on_window_close_change);
        glfwSetWindowFocusCallback(glfw_window, on_window_focus_change);
        glfwSetWindowIconifyCallback(glfw_window, on_window_iconify_change);

        ST_LOG_INFO("Created window [{0}, {1}x{2}]", config.title, width, height);
    }

    Window::~Window() {
        glfwDestroyWindow(glfw_window);
        ST_LOG_DEBUG("Destroyed GLFW window");
        glfwTerminate();
        ST_LOG_DEBUG("Terminated GLFW");
    }

    Window::operator GLFWwindow*() const {
        return glfw_window;
    }

    void Window::poll_events() {
        glfwPollEvents();
    }

    void Window::swap_buffers() const {
        glfwSwapBuffers(glfw_window);
    }

    void Window::set_title(const char* title) const {
        glfwSetWindowTitle(glfw_window, title);
    }

    WindowSize Window::get_size_in_pixels() const  {
        i32 width = 0;
        i32 height = 0;
        glfwGetFramebufferSize(glfw_window, &width, &height);
        return { width, height };
    }

    WindowSize Window::get_size_in_screen_coordinates() const {
        i32 width = 0;
        i32 height = 0;
        glfwGetWindowSize(glfw_window, &width, &height);
        return { width, height };
    }

    f32 Window::get_aspect_ratio() const {
        auto [width, height] = get_size_in_screen_coordinates();
        return (f32) width / (f32) height;
    }

    // Returns the time elapsed, in seconds, since GLFW was initialized, or zero if an error occurred.
    // The resolution is system dependent.
    f64 Window::get_time() {
        return glfwGetTime();
    }

    bool Window::is_iconified() const {
        return glfwGetWindowAttrib(glfw_window, GLFW_ICONIFIED) == 1;
    }

    void Window::wait_until_not_minimized() const {
        WindowSize size_px = get_size_in_pixels();
        bool iconified = is_iconified();
        while (size_px.width == 0 || size_px.height == 0 || iconified) {
            size_px = get_size_in_pixels();
            iconified = is_iconified();
            glfwWaitEvents();
        }
    }

    void Window::set_mouse_events_enabled(bool mouse_events_enabled) {
        this->mouse_events_enabled = mouse_events_enabled;
    }

    void Window::set_keyboard_events_enabled(bool keyboard_events_enabled) {
        this->keyboard_events_enabled = keyboard_events_enabled;
    }

    void Window::on_glfw_error(i32 error, const char* description) {
        ST_LOG_ERROR("GLFW error [{0}: {1}]", error, description);
    }

    void Window::on_cursor_position_change(GLFWwindow* glfw_window, f64 x, f64 y) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->mouse_events_enabled) {
            return;
        }

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        MouseMovedEvent event;
        event.x = x;
        event.y = y;

        dispatcher->trigger<MouseMovedEvent>(std::move(event));
    }

    void Window::on_framebuffer_size_change(GLFWwindow* glfw_window, i32 width, i32 height) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        WindowResizedEvent event;
        event.width = width;
        event.height = height;

        dispatcher->trigger<WindowResizedEvent>(std::move(event));
    }

    void Window::on_key_change(GLFWwindow* glfw_window, i32 key, i32 scanCode, i32 action, i32 mods) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->keyboard_events_enabled) {
            return;
        }

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        if (action == GLFW_PRESS) {
            KeyPressedEvent event;
            event.key_code = key;
            event.mods = mods;
            event.scan_code = scanCode;
            dispatcher->trigger<KeyPressedEvent>(std::move(event));
        } else if (action == GLFW_RELEASE) {
            KeyReleasedEvent event;
            event.key_code = key;
            event.mods = mods;
            event.scan_code = scanCode;
            dispatcher->trigger<KeyReleasedEvent>(std::move(event));
        } else if (action == GLFW_REPEAT) {
            KeyRepeatedEvent event;
            event.key_code = key;
            event.mods = mods;
            event.scan_code = scanCode;
            dispatcher->trigger<KeyRepeatedEvent>(std::move(event));
        }
    }

    void Window::on_mouse_button_change(GLFWwindow* glfw_window, i32 button, i32 action, i32 mods) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->mouse_events_enabled) {
            return;
        }

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        if (action == GLFW_PRESS) {
            MouseButtonPressedEvent event;
            event.button = button;
            dispatcher->trigger<MouseButtonPressedEvent>(std::move(event));
        }
        if (action == GLFW_RELEASE) {
            MouseButtonReleasedEvent event;
            event.button = button;
            dispatcher->trigger<MouseButtonReleasedEvent>(std::move(event));
        }
    }

    void Window::on_mouse_scroll_change(GLFWwindow* glfw_window, f64 xoffset, f64 yoffset) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->mouse_events_enabled) {
            return;
        }

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        MouseScrollEvent event;
        event.x_offset = xoffset;
        event.y_offset = yoffset;

        dispatcher->trigger<MouseScrollEvent>(std::move(event));
    }

    void Window::on_window_close_change(GLFWwindow* glfw_window) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        WindowClosedEvent event{};

        dispatcher->trigger<WindowClosedEvent>(std::move(event));
    }

    void Window::on_window_focus_change(GLFWwindow* glfw_window, i32 focused) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        WindowFocusedEvent event;
        event.focused = focused == 1;

        dispatcher->trigger<WindowFocusedEvent>(std::move(event));
    }

    void Window::on_window_iconify_change(GLFWwindow* glfw_window, i32 iconified) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto dispatcher = window->config.dispatcher;
        ST_ASSERT(dispatcher != nullptr, "Dispatcher must exist on Window GLFW user pointer");

        WindowMinimizedEvent event;
        event.minimized = iconified == 1;

        dispatcher->trigger<WindowMinimizedEvent>(std::move(event));
    }
}
