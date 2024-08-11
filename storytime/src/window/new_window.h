#pragma once

#include "window/event_manager.h"

#include <GLFW/glfw3.h>

namespace Storytime {
    struct NewWindowConfig {
        std::string title;
        i32 width;
        i32 height;
        bool maximized;
        bool resizable;
        u32 context_version_major;
        u32 context_version_minor;
    };

    class NewWindow {
    private:
        NewWindowConfig config;
        EventManager event_manager;
        GLFWwindow* glfwWindow = nullptr;

    public:
        explicit NewWindow(const NewWindowConfig& config);

        void update() const;

        SubscriptionID subscribe_to_event(EventType event_type, const Subscription& subscription);

    private:
        static void OnGlfwError(i32 error, const char* description);

        static void on_key_change(GLFWwindow* glfwWindow, i32 key, i32 scanCode, i32 action, i32 mods);

        static void onMouseButtonChange(GLFWwindow* glfwWindow, i32 button, i32 action, i32 mods);

        static void on_window_close_change(GLFWwindow* glfwWindow);

        static void onWindowIconifyChange(GLFWwindow* glfwWindow, int iconified);

        static void onFramebufferSizeChange(GLFWwindow* glfwWindow, int width, int height);

        static void send_event(GLFWwindow* glfwWindow, EventType event_type, const Event& event);

    };
}
