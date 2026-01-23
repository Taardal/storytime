#pragma once

#include "event/st_key_pressed_event.h"
#include "event/st_key_released_event.h"
#include "event/st_key_repeated_event.h"
#include "window/st_key.h"
#include "window/st_window.h"

namespace Storytime {
    typedef std::function<void(const KeyPressedEvent&)> OnPressedSubscription;
    typedef std::function<void(const KeyReleasedEvent&)> OnReleasedSubscription;
    typedef std::function<void(const KeyRepeatedEvent&)> OnRepeatedSubscription;

    struct KeyboardConfig {
        const Window& window;
        Dispatcher& dispatcher;
    };

    class Keyboard {
    private:
        KeyboardConfig config;
        std::vector<SubscriptionID> event_subscriptions;
        std::vector<OnPressedSubscription> on_pressed_subscriptions;
        std::vector<OnReleasedSubscription> on_released_subscriptions;
        std::vector<OnRepeatedSubscription> on_repeated_subscriptions;
        bool enabled = true;

    public:
        Keyboard(const KeyboardConfig& config);

        Keyboard(KeyboardConfig&& config);

        ~Keyboard();

        bool is_enabled() const;

        void set_enabled(bool enabled);

        bool is_pressed(KeyCode key_code) const;

        void on_pressed(const OnPressedSubscription& subscription);

        void on_released(const OnReleasedSubscription& subscription);

        void on_repeated(const OnRepeatedSubscription& subscription);

    private:
        void initialize();

        void terminate();
    };
}
