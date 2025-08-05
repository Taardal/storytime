#pragma once

namespace Storytime {
    template<typename T>
    static std::string serialize_value(const T& value) {
        std::stringstream ss;
        msgpack::pack(ss, value);
        return ss.str();
    }

    template<typename T>
    static void deserialize_value(T* value, const std::string& data) {
        ST_ASSERT(value != nullptr, "Value object pointer cannot be null");
        ST_ASSERT(data.size() > 0, "Data string cannot be empty");

        msgpack::object_handle object_handle = msgpack::unpack(data.data(), data.size());
        ST_ASSERT(!object_handle->is_nil(), "Value object msgpack handle cannot be null");

        const msgpack::object& object = object_handle.get();
        object.convert(*value);
    }
}
