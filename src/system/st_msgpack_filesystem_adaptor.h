#pragma once

#include "st_msgpack_utils.h"
#include <msgpack.hpp>

namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {

            template<>
            struct pack<std::filesystem::path> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const std::filesystem::path& v) const {
                    const std::string& str = v.string();
                    o.pack_str(str.size());
                    o.pack_str_body(str.c_str(), str.size());
                    return o;
                }
            };


            template<>
            struct convert<std::filesystem::path> {
                const msgpack::object& operator()(const msgpack::object& o, std::filesystem::path& v) const {
                    ST_ASSERT(
                        o.type == msgpack::type::STR,
                        "msgpack::object for std::filesystem::path cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.str.size > 0,
                        "msgpack::object for std::filesystem::path cannot be empty"
                    );
                    v = o.as<std::filesystem::path>();
                    return o;
                }
            };
        }
    }
}
