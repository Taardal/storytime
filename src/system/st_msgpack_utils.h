#pragma once

#include <msgpack.hpp>

#define MSGPACK_ADAPTOR_NAMESPACE(body)\
    namespace msgpack {\
        MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {\
            namespace adaptor\
                body\
        }\
    }

std::string get_msgpack_object_type_name(msgpack::type::object_type type);