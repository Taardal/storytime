#pragma once

#include "storytime_config.h"
#include "storytime_app.h"
#include "audio/audio_module.h"
#include "graphics/graphics_module.h"
#include "resource/resource_module.h"
#include "system/system_module.h"
#include "window/window_module.h"

namespace Storytime {
    class Storytime {
    private:
        Config config{};
        SystemModule* system_module;
        WindowModule* window_module;
        AudioModule* audio_module;
        ResourceModule* resource_module;
        GraphicsModule* graphics_module;

    public:
        Storytime(
            const Config& config,
            SystemModule* system_module,
            WindowModule* window_module,
            AudioModule* audio_module,
            ResourceModule* resource_module,
            GraphicsModule* graphics_module
        ) : config(config),
            system_module(system_module),
            window_module(window_module),
            audio_module(audio_module),
            resource_module(resource_module),
            graphics_module(graphics_module) {
        }

        const Config& cfg() const {
            return config;
        }

        const CommandLineArguments& args() const {
            return config.command_line_arguments;
        }

        template<class T>
        T* get() const {
            if (T* pointer = system_module->get<T>(); pointer != nullptr) {
                return pointer;
            }
            if (T* pointer = window_module->get<T>(); pointer != nullptr) {
                return pointer;
            }
            if (T* pointer = audio_module->get<T>(); pointer != nullptr) {
                return pointer;
            }
            if (T* pointer = resource_module->get<T>(); pointer != nullptr) {
                return pointer;
            }
            if (T* pointer = graphics_module->get<T>(); pointer != nullptr) {
                return pointer;
            }
            return nullptr;
        }
    };

    void run(const Config& config, const std::function<App*(const Storytime&)>& create_app);

    template<typename T>
    void run(const Config& config) {
        run(config, [](const Storytime& storytime) {
            return ST_NEW T(storytime);
        });
    }
}
