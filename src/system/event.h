#pragma once

#include <string>

namespace Storytime {
    typedef u32 EventType;

    struct Event {
        EventType type;
        std::string name;

        Event(EventType type, const std::string& name);

        virtual ~Event() = default;

        virtual std::string to_string() const;

        std::string get_name() const;

        EventType get_type() const;
    };
}
