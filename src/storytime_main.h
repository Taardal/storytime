#pragma once

#include "system/command_line_arguments.h"
#include "system/service_locator.h"
#include "system/event_manager.h"

namespace Storytime {
    struct Config {
        CommandLineArguments command_line_arguments{};
        LogLevel log_level = LogLevel::info;
        u32 target_fps = 60;
        std::string window_title = "Storytime";
        i32 window_width = 1280;
        i32 window_height = 768;
        f32 window_aspect_ratio = 16.0f / 9.0f;
        bool window_maximized = false;
        bool window_resizable = true;
        u32 open_gl_version_major = 4;
        u32 open_gl_version_minor = 1;
        std::string glsl_version = "#version 410";
    };

    void run(const Config& config);

    void stop();

    void* get_service(std::type_index type);

    template<class T>
    T* get() {
        return static_cast<T*>(get_service(typeid(T)));
    }

    u32 subscribe(EventType event_type, const Subscription& subscription);
}

#define ST_SUBSCRIBE(E, captures, on_event)\
    Storytime::subscribe(E::type, captures(const st::Event& e) {\
        const auto& event = static_cast<const E&>(e);\
        on_event\
    })
