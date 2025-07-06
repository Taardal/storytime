#include "st_window.h"

#include "event/st_events.h"

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

        ST_ASSERT(config.width > 0, "Window width must be greater than zero");
        ST_ASSERT(config.height > 0 || config.aspect_ratio > 0.0f, "Window height or aspect ratio must be greater than zero");
        i32 height = config.aspect_ratio > 0.0f ? (f32) config.width / config.aspect_ratio : config.height;

        ST_LOG_TRACE("Creating GLFW window");
        GLFWmonitor* fullscreen_monitor = nullptr;
        GLFWwindow* shared_window = nullptr;
        glfw_window = glfwCreateWindow(config.width, height, config.title.c_str(), fullscreen_monitor, shared_window);
        if (glfw_window == nullptr) {
            ST_THROW("Could not create GLFW window");
        }
        ST_LOG_DEBUG("Created GLFW window [{0}, {1}x{2}]", config.title, config.width, config.height);

        glfwMakeContextCurrent(glfw_window);
        glfwSetWindowUserPointer(glfw_window, this);

        glfwSetCursorPosCallback(glfw_window, on_cursor_position_change);
        glfwSetFramebufferSizeCallback(glfw_window, on_framebuffer_size_change);
        glfwSetKeyCallback(glfw_window, on_key_change);
        glfwSetMouseButtonCallback(glfw_window, on_mouse_button_change);
        glfwSetScrollCallback(glfw_window, on_mouse_scroll_change);
        glfwSetWindowCloseCallback(glfw_window, on_window_close_change);
        glfwSetWindowIconifyCallback(glfw_window, on_window_iconify_change);

        ST_LOG_INFO("Created window [{0}, {1}x{2}]", config.title, config.width, config.height);

        config.event_manager->mute(MouseMovedEvent::type);
    }

    Window::~Window() {
        glfwDestroyWindow(glfw_window);
        ST_LOG_DEBUG("Destroyed GLFW window");
        glfwTerminate();
        ST_LOG_DEBUG("Terminated GLFW");
    }

    Window::operator GLFWwindow*() const {
        ST_ASSERT(glfw_window != nullptr, "Cannot access GLFW window object that has not been created or is already destroyed");
        return glfw_window;
    }

    void Window::process_events() {
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

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        MouseMovedEvent event;
        event.x = x;
        event.y = y;

        event_manager->trigger_event(MouseMovedEvent::type, event);
    }

    void Window::on_framebuffer_size_change(GLFWwindow* glfw_window, i32 width, i32 height) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        WindowResizedEvent event;
        event.width = width;
        event.height = height;

        event_manager->trigger_event(WindowResizedEvent::type, event);
    }

    void Window::on_key_change(GLFWwindow* glfw_window, i32 key, i32 scanCode, i32 action, i32 mods) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->keyboard_events_enabled) {
            return;
        }

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        if (action == GLFW_PRESS) {
            KeyPressedEvent event;
            event.key_code = key;
            event.mods = mods;
            event.scan_code = scanCode;
            event_manager->trigger_event(KeyPressedEvent::type, event);
        } else if (action == GLFW_RELEASE) {
            KeyReleasedEvent event;
            event.key_code = key;
            event.mods = mods;
            event.scan_code = scanCode;
            event_manager->trigger_event(KeyReleasedEvent::type, event);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatedEvent event;
            event.key_code = key;
            event.mods = mods;
            event.scan_code = scanCode;
            event_manager->trigger_event(KeyRepeatedEvent::type, event);
        }
    }

    void Window::on_mouse_button_change(GLFWwindow* glfw_window, i32 button, i32 action, i32 mods) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->mouse_events_enabled) {
            return;
        }

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        if (action == GLFW_PRESS) {
            MouseButtonPressedEvent event;
            event.button = button;
            event_manager->trigger_event(MouseButtonPressedEvent::type, event);
        }
        if (action == GLFW_RELEASE) {
            MouseButtonReleasedEvent event;
            event.button = button;
            event_manager->trigger_event(MouseButtonReleasedEvent::type, event);
        }
    }

    void Window::on_mouse_scroll_change(GLFWwindow* glfw_window, f64 xoffset, f64 yoffset) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        if (!window->mouse_events_enabled) {
            return;
        }

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        MouseScrollEvent event;
        event.x_offset = xoffset;
        event.y_offset = yoffset;

        event_manager->trigger_event(MouseScrollEvent::type, event);
    }

    void Window::on_window_close_change(GLFWwindow* glfw_window) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        WindowClosedEvent event{};

        event_manager->trigger_event(WindowClosedEvent::type, event);
    }

    void Window::on_window_iconify_change(GLFWwindow* glfw_window, i32 iconified) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        ST_ASSERT(window != nullptr, "Window must exist as a GLFW user pointer");

        auto event_manager = window->config.event_manager;
        ST_ASSERT(event_manager != nullptr, "Event manager must exist on Window GLFW user pointer");

        WindowMinimizedEvent event;
        event.minimized = iconified == 1;

        event_manager->trigger_event(WindowMinimizedEvent::type, event);
    }
}
