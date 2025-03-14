#include "msgpack_utils.h"

std::string get_msgpack_object_type_name(msgpack::type::object_type type) {
    switch (type) {
        case msgpack::type::object_type::NIL:
            return "NIL";
        case msgpack::type::object_type::BOOLEAN:
            return "BOOLEAN";
        case msgpack::type::object_type::POSITIVE_INTEGER:
            return "POSITIVE_INTEGER";
        case msgpack::type::object_type::NEGATIVE_INTEGER:
            return "NEGATIVE_INTEGER";
        case msgpack::type::object_type::FLOAT32:
            return "FLOAT32";
        case msgpack::type::object_type::FLOAT64:
            return "FLOAT64";
        case msgpack::type::object_type::STR:
            return "STR";
        case msgpack::type::object_type::BIN:
            return "BIN";
        case msgpack::type::object_type::ARRAY:
            return "ARRAY";
        case msgpack::type::object_type::MAP:
            return "MAP";
        case msgpack::type::object_type::EXT:
            return "EXT";
        default:
            return "";
    }
}