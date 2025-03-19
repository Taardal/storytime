#pragma once

#include <msgpack.hpp>

template<class T>
std::string to_binary(T value) {
    std::stringstream ss;
    msgpack::pack(ss, value);
    return ss.str();
}

#define binary_serialize to_binary
#define serialize_binary to_binary
#define serialize_to_binary to_binary

template<class T>
void from_binary(const std::string& data, T* value) {
    ST_ASSERT(value != nullptr, "Value object pointer cannot be null");
    ST_ASSERT(data.size() > 0, "Data string cannot be empty");

    msgpack::object_handle object_handle = msgpack::unpack(data.data(), data.size());
    ST_ASSERT(!object_handle->is_nil(), "Value object msgpack handle cannot be null");

    const msgpack::object& object = object_handle.get();
    object.convert(*value);
}

template<class T>
T from_binary(const std::string& data) {
    T value;
    from_binary<T>(data, &value);
    return value;
}

#define binary_deserialize from_binary
#define deserialize_binary from_binary
#define deserialize_from_binary from_binary
