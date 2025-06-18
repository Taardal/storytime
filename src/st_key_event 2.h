#pragma once

#include "st_event.h"
#include <msgpack.hpp>

namespace Storytime {
    struct KeyPressedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyPressedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyPressedEvent& event);
}

namespace Storytime {
    struct KeyReleasedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyReleasedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyReleasedEvent& event);
}

namespace Storytime {
    struct KeyRepeatedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyRepeatedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyRepeatedEvent& event);
}

namespace Storytime {
    struct KeyTypedEvent : Event {
        static const EventType type;
        static const std::string type_name;

        i32 key_code = 0;
        i32 mods = 0;
        i32 scan_code = 0;

        KeyTypedEvent();

        std::string to_string() const override;
    };

    std::ostream& operator<<(std::ostream& os, const KeyTypedEvent& event);
}
