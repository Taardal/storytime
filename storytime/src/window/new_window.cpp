#include "new_window.h"
#include "window/events/KeyEvent.h"
#include "window/events/MouseEvent.h"
#include "window/events/WindowEvent.h"

namespace Storytime {
    NewWindow::NewWindow(const NewWindowConfig& config): config(config) {
        ST_LOG_T("Initializing GLFW");
        ST_ASSERT_THROW(glfwInit());
        ST_LOG_D("Initialized GLFW");

        glfwSetErrorCallback(OnGlfwError);

        ST_LOG_T("Setting GLFW window hints");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.context_version_major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.context_version_minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_MAXIMIZED, config.maximized);
#ifdef ST_PLATFORM_MACOS
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif
        ST_LOG_D("GLFW context version [{0}.{1}]", config.context_version_major, config.context_version_minor);

        ST_LOG_T("Creating GLFW window");
        GLFWmonitor* fullscreenMonitor = nullptr;
        GLFWwindow* sharedWindow = nullptr;
        glfwWindow = glfwCreateWindow(config.width, config.height, config.title.c_str(), fullscreenMonitor, sharedWindow);
        ST_ASSERT_THROW(glfwWindow != nullptr);
        ST_LOG_D("Created GLFW window [{0}, {1}x{2}]", config.title, config.width, config.height);

        glfwMakeContextCurrent(glfwWindow);
        glfwSetWindowUserPointer(glfwWindow, this);

        glfwSetKeyCallback(glfwWindow, on_key_change);
        glfwSetWindowCloseCallback(glfwWindow, on_window_close_change);
        // glfwSetMouseButtonCallback(glfwWindow, onMouseButtonChange);
        // glfwSetWindowIconifyCallback(glfwWindow, onWindowIconifyChange);
        // glfwSetFramebufferSizeCallback(glfwWindow, onFramebufferSizeChange);

    }

    void NewWindow::update() const {
        glfwPollEvents();
        glfwSwapBuffers(glfwWindow);
    }

    SubscriptionID NewWindow::subscribe_to_event(const EventType event_type, const Subscription& subscription) {
        return event_manager.subscribe(event_type, subscription);
    }

    void NewWindow::OnGlfwError(int32_t error, const char* description) {
        ST_LOG_E("GLFW error [{0}: {1}]", error, description);
    }

    void NewWindow::on_key_change(GLFWwindow* glfwWindow, int32_t key, int32_t scanCode, int32_t action, int32_t mods) {
        if (action == GLFW_PRESS) {
            KeyPressedEvent event(key, mods, scanCode);
            send_event(glfwWindow, EventType::KeyPressed, event);
        } else if (action == GLFW_RELEASE) {
            KeyReleasedEvent event(key, mods, scanCode);
            send_event(glfwWindow, EventType::KeyReleased, event);
        } else if (action == GLFW_REPEAT) {
            KeyRepeatedEvent event(key, mods, scanCode);
            send_event(glfwWindow, EventType::KeyRepeated, event);
        }
    }

    void NewWindow::onMouseButtonChange(GLFWwindow* glfwWindow, int32_t button, int32_t action, int32_t mods) {
        // if (action == GLFW_PRESS) {
        //     MouseButtonPressedEvent event(button);
        //     sendEvent(MouseButtonPressedEvent::type, event, glfwWindow);
        // }
        // if (action == GLFW_RELEASE) {
        //     MouseButtonReleasedEvent event(button);
        //     sendEvent(MouseButtonReleasedEvent::type, event, glfwWindow);
        // }
    }

    void NewWindow::on_window_close_change(GLFWwindow* glfwWindow) {
        WindowCloseEvent event{};
        send_event(glfwWindow, EventType::WindowClose, event);
    }

    void NewWindow::onFramebufferSizeChange(GLFWwindow* glfwWindow, int width, int height) {
        // WindowResizeEvent event(width, height);
        // sendEvent(WindowResizeEvent::type, event, glfwWindow);
    }

    void NewWindow::onWindowIconifyChange(GLFWwindow* glfwWindow, int iconified) {
        // bool minimized = iconified == 1;
        // WindowMinimizeEvent event(minimized);
        // sendEvent(event, glfwWindow);
    }

    void NewWindow::send_event(GLFWwindow* glfwWindow, EventType event_type, const Event& event) {
        ST_LOG_T(event.ToString());
        auto window = (NewWindow*) glfwGetWindowUserPointer(glfwWindow);
        ST_ASSERT(window != nullptr);
        window->event_manager.trigger_event(event_type, event);
    }

}
