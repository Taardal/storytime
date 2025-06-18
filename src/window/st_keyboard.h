#pragma once

#include "window/st_key.h"
#include "event/st_events.h"
#include "window/st_window.h"

namespace Storytime {
    typedef std::function<void(const KeyPressedEvent&)> OnPressedFn;
    typedef std::function<void(const KeyReleasedEvent&)> OnReleasedFn;
    typedef std::function<void(const KeyRepeatedEvent&)> OnRepeatedFn;

    struct KeyboardConfig {
        Window* window = nullptr;
        EventManager* event_manager = nullptr;
    };

    class Keyboard {
    private:
        KeyboardConfig config;
        std::vector<SubscriptionID> event_subscriptions;
        std::vector<OnPressedFn> on_pressed_fns;
        std::vector<OnReleasedFn> on_released_fns;
        std::vector<OnRepeatedFn> on_repeated_fns;
        bool enabled = true;

    public:
        Keyboard(const KeyboardConfig& config);

        Keyboard(KeyboardConfig&& config);

        ~Keyboard();

        bool is_enabled() const;

        void set_enabled(bool enabled);

        bool is_pressed(KeyCode key_code) const;

        void on_pressed(const OnPressedFn& on_pressed);

        void on_released(const OnReleasedFn& on_released);

        void on_repeated(const OnRepeatedFn& on_repeated);

    private:
        void initialize();

        void terminate();
    };
}
