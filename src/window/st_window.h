#pragma once

#include "event/st_event_manager.h"

#include <GLFW/glfw3.h>

namespace Storytime {
    struct WindowSize {
        i32 width;
        i32 height;

        f32 get_aspect_ratio() const {
            return (f32) width / (f32) height;
        }
    };

    struct WindowConfig {
        EventManager* event_manager = nullptr;
        std::string title;
        i32 width;
        i32 height;
        f32 aspect_ratio;
        bool maximized;
        bool resizable;
        u32 context_version_major;
        u32 context_version_minor;
    };

    class Window {
    private:
        WindowConfig config;
        GLFWwindow* glfw_window = nullptr;
        bool mouse_events_enabled = true;
        bool keyboard_events_enabled = true;

    public:
        explicit Window(const WindowConfig& config);

        ~Window();

        operator GLFWwindow*() const;

        static void process_events();

        void swap_buffers() const;

        void set_title(const char* title) const;

        WindowSize get_size_in_pixels() const;

        WindowSize get_size_in_screen_coordinates() const;

        f32 get_aspect_ratio() const;

        static f64 get_time();

        void set_mouse_events_enabled(bool mouse_events_enabled);

        void set_keyboard_events_enabled(bool keyboard_events_enabled);

    private:
        static void on_glfw_error(i32 error, const char* description);

        static void on_cursor_position_change(GLFWwindow* glfw_window, f64 x, f64 y);

        static void on_framebuffer_size_change(GLFWwindow* glfw_window, i32 width, i32 height);

        static void on_key_change(GLFWwindow* glfw_window, i32 key, i32 scanCode, i32 action, i32 mods);

        static void on_mouse_button_change(GLFWwindow* glfw_window, i32 button, i32 action, i32 mods);

        static void on_mouse_scroll_change(GLFWwindow* glfw_window, f64 xoffset, f64 yoffset);

        static void on_window_close_change(GLFWwindow* glfw_window);

        static void on_window_iconify_change(GLFWwindow* glfw_window, i32 iconified);
    };
}
