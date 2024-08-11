#pragma once

#include "window/event_manager.h"

#include <GLFW/glfw3.h>

namespace Storytime {
    struct WindowSize {
        i32 width;
        i32 height;
    };

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
        EventManager* event_manager;
        GLFWwindow* glfw_window = nullptr;

    public:
        explicit NewWindow(const NewWindowConfig& config, EventManager* event_manager);

        void update() const;

        WindowSize get_size_in_pixels() const;

        void get_size_in_pixels(i32* width, i32* height) const;

        f32 get_aspect_ratio() const;

    private:
        static void on_glfw_error(i32 error, const char* description);

        static void on_framebuffer_size_change(GLFWwindow* glfw_window, i32 width, i32 height);

        static void on_key_change(GLFWwindow* glfw_window, i32 key, i32 scanCode, i32 action, i32 mods);

        static void on_mouse_button_change(GLFWwindow* glfw_window, i32 button, i32 action, i32 mods);

        static void on_mouse_scroll_change(GLFWwindow* glfw_window, f64 xoffset, f64 yoffset);

        static void on_window_close_change(GLFWwindow* glfw_window);

        static void on_window_iconify_change(GLFWwindow* glfw_window, i32 iconified);

        static void on_event(GLFWwindow* glfw_window, EventType event_type, const Event& event);

    };
}
