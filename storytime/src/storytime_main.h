#pragma once

#include "storytime_config.h"
#include "storytime_app.h"
#include "system/service_locator.h"
#include "window/event_manager.h"

namespace Storytime {
    class Storytime {
    public:
        Config config{};

    private:
        ServiceLocator* service_locator = nullptr;
        EventManager* event_manager = nullptr;

    public:
        Storytime(const Config& config, ServiceLocator* service_locator, EventManager* event_manager)
            : config(config), service_locator(service_locator), event_manager(event_manager) {
        }

        CommandLineArguments args() const {
            return config.command_line_arguments;
        }

        template<class T>
        T* get() const {
            ST_ASSERT(service_locator != nullptr);
            return service_locator->get<T>();
        }

        SubscriptionID subscribe(EventType event_type, const Subscription& subscription) const {
            ST_ASSERT(event_manager != nullptr);
            return event_manager->subscribe(event_type, subscription);
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
