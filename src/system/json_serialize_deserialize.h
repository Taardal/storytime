#pragma once

#include <nlohmann/json.hpp>

template<typename T>
void from_json(const nlohmann::json&, T&) {
    static_assert(sizeof(T) == 0, "Unimplemented JSON deserialization");
}

#define json_to from_json
#define json_deserialize from_json