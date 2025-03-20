#pragma once

#include "st_msgpack_utils.h"
#include <msgpack.hpp>
#include <glm/glm.hpp>

namespace msgpack {
    MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
        namespace adaptor {

            // ---------------------------------------------------------------------
            // glm::vec2
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::vec2> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::vec2& v) const {
                    o.pack_map(3);
                    o.pack("x");
                    o.pack_float(v.x);
                    o.pack("y");
                    o.pack_float(v.y);
                    return o;
                }
            };


            template<>
            struct convert<glm::vec2> {
                const msgpack::object& operator()(const msgpack::object& o, glm::vec2& v) const {
                    ST_ASSERT(
                        o.type == msgpack::type::MAP,
                        "msgpack::object for glm::vec2 cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.map.size == 2,
                        "msgpack::object for glm::vec2 cannot be size [" << o.via.map.size << "]"
                    );
                    for (uint32_t i = 0; i < o.via.map.size; ++i) {
                        const std::string& key = o.via.map.ptr[i].key.as<std::string>();
                        const msgpack::object& val = o.via.map.ptr[i].val;
                        if (key == "x") {
                            v.x = val.as<float>();
                        } else if (key == "y") {
                            v.y = val.as<float>();
                        }
                    }
                    return o;
                }
            };

            // ---------------------------------------------------------------------
            // glm::vec3
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::vec3> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::vec3& v) const {
                    o.pack_map(3);
                    o.pack("x");
                    o.pack_float(v.x);
                    o.pack("y");
                    o.pack_float(v.y);
                    o.pack("z");
                    o.pack_float(v.z);
                    return o;
                }
            };

            template<>
            struct convert<glm::vec3> {
                const msgpack::object& operator()(const msgpack::object& o, glm::vec3& v) const {
                    ST_ASSERT(
                        o.type == msgpack::type::MAP,
                        "msgpack::object for glm::vec3 cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.map.size == 3,
                        "msgpack::object for glm::vec3 cannot be size [" << o.via.map.size << "]"
                    );
                    for (uint32_t i = 0; i < o.via.map.size; ++i) {
                        const std::string& key = o.via.map.ptr[i].key.as<std::string>();
                        const msgpack::object& val = o.via.map.ptr[i].val;
                        if (key == "x") {
                            v.x = val.as<float>();
                        } else if (key == "y") {
                            v.y = val.as<float>();
                        } else if (key == "z") {
                            v.z = val.as<float>();
                        }
                    }
                    return o;
                }
            };

            // ---------------------------------------------------------------------
            // glm::vec4
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::vec4> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::vec4& v) const {
                    o.pack_map(3);
                    o.pack("x");
                    o.pack_float(v.x);
                    o.pack("y");
                    o.pack_float(v.y);
                    o.pack("z");
                    o.pack_float(v.z);
                    o.pack("w");
                    o.pack_float(v.w);
                    return o;
                }
            };

            template<>
            struct convert<glm::vec4> {
                const msgpack::object& operator()(const msgpack::object& o, glm::vec4& v) const {
                    ST_ASSERT(
                        o.type == msgpack::type::MAP,
                        "msgpack::object for glm::vec4 cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.map.size == 4,
                        "msgpack::object for glm::vec4 cannot be size [" << o.via.map.size << "]"
                    );
                    for (uint32_t i = 0; i < o.via.map.size; ++i) {
                        const std::string& key = o.via.map.ptr[i].key.as<std::string>();
                        const msgpack::object& val = o.via.map.ptr[i].val;
                        if (key == "x") {
                            v.x = val.as<float>();
                        } else if (key == "y") {
                            v.y = val.as<float>();
                        } else if (key == "z") {
                            v.z = val.as<float>();
                        } else if (key == "w") {
                            v.w = val.as<float>();
                        }
                    }
                    return o;
                }
            };

            // ---------------------------------------------------------------------
            // glm::mat2
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::mat2> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::mat2& m) const {
                    o.pack_array(2);
                    for (int i = 0; i < 2; ++i) {
                        o.pack_array(2);
                        for (int j = 0; j < 2; ++j) {
                            o.pack_float(m[i][j]);
                        }
                    }
                    return o;
                }
            };

            template<>
            struct convert<glm::mat2> {
                const msgpack::object& operator()(const msgpack::object& o, glm::mat2& m) const {
                    ST_ASSERT(
                        o.type == msgpack::type::ARRAY,
                        "msgpack::object for glm::mat2 cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.array.size == 2,
                        "msgpack::object for glm::mat2 cannot be size [" << o.via.map.size << "]"
                    );
                    for (int i = 0; i < o.via.array.size; ++i) {
                        const msgpack::object& row = o.via.array.ptr[i];
                        ST_ASSERT(
                            row.type == msgpack::type::ARRAY,
                            "msgpack::object for glm::mat2 cannot be type [" << get_msgpack_object_type_name(row.type) << "]"
                        );
                        ST_ASSERT(
                            row.via.array.size == 2,
                            "msgpack::object for glm::mat2 cannot be size [" << row.via.map.size << "]"
                        );
                        for (int j = 0; j < row.via.array.size; ++j) {
                            m[i][j] = row.via.array.ptr[j].as<float>();
                        }
                    }
                    return o;
                }
            };

            // ---------------------------------------------------------------------
            // glm::mat3
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::mat3> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::mat3& m) const {
                    o.pack_array(3);
                    for (int i = 0; i < 3; ++i) {
                        o.pack_array(3);
                        for (int j = 0; j < 3; ++j) {
                            o.pack_float(m[i][j]);
                        }
                    }
                    return o;
                }
            };

            template<>
            struct convert<glm::mat3> {
                const msgpack::object& operator()(const msgpack::object& o, glm::mat3& m) const {
                    ST_ASSERT(
                        o.type == msgpack::type::ARRAY,
                        "msgpack::object for glm::mat3 cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.array.size == 3,
                        "msgpack::object for glm::mat3 cannot be size [" << o.via.map.size << "]"
                    );
                    for (int i = 0; i < o.via.array.size; ++i) {
                        const msgpack::object& row = o.via.array.ptr[i];
                        ST_ASSERT(
                            row.type == msgpack::type::ARRAY,
                            "msgpack::object for glm::mat3 cannot be type [" << get_msgpack_object_type_name(row.type) << "]"
                        );
                        ST_ASSERT(
                            row.via.array.size == 3,
                            "msgpack::object for glm::mat3 cannot be size [" << row.via.map.size << "]"
                        );
                        for (int j = 0; j < row.via.array.size; ++j) {
                            m[i][j] = row.via.array.ptr[j].as<float>();
                        }
                    }
                    return o;
                }
            };

            // ---------------------------------------------------------------------
            // glm::mat4
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::mat4> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::mat4& m) const {
                    o.pack_array(4);
                    for (int i = 0; i < 4; ++i) {
                        o.pack_array(4);
                        for (int j = 0; j < 4; ++j) {
                            o.pack_float(m[i][j]);
                        }
                    }
                    return o;
                }
            };

            template<>
            struct convert<glm::mat4> {
                const msgpack::object& operator()(const msgpack::object& o, glm::mat4& m) const {
                    ST_ASSERT(
                        o.type == msgpack::type::ARRAY,
                        "msgpack::object for glm::mat4 cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.array.size == 4,
                        "msgpack::object for glm::mat4 cannot be size [" << o.via.map.size << "]"
                    );
                    for (int i = 0; i < o.via.array.size; ++i) {
                        const msgpack::object& row = o.via.array.ptr[i];
                        ST_ASSERT(
                            row.type == msgpack::type::ARRAY,
                            "msgpack::object for glm::mat4 cannot be type [" << get_msgpack_object_type_name(row.type) << "]"
                        );
                        ST_ASSERT(
                            row.via.array.size == 4,
                            "msgpack::object for glm::mat4 cannot be size [" << row.via.map.size << "]"
                        );
                        for (int j = 0; j < row.via.array.size; ++j) {
                            m[i][j] = row.via.array.ptr[j].as<float>();
                        }
                    }
                    return o;
                }
            };

            // ---------------------------------------------------------------------
            // glm::quat
            // ---------------------------------------------------------------------

            template<>
            struct pack<glm::quat> {
                template<typename Stream>
                msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const glm::quat& v) const {
                    o.pack_map(3);
                    o.pack("x");
                    o.pack_float(v.x);
                    o.pack("y");
                    o.pack_float(v.y);
                    o.pack("z");
                    o.pack_float(v.z);
                    o.pack("w");
                    o.pack_float(v.w);
                    return o;
                }
            };

            template<>
            struct convert<glm::quat> {
                const msgpack::object& operator()(const msgpack::object& o, glm::quat& v) const {
                    ST_ASSERT(
                        o.type == msgpack::type::MAP,
                        "msgpack::object for glm::quat cannot be type [" << get_msgpack_object_type_name(o.type) << "]"
                    );
                    ST_ASSERT(
                        o.via.map.size == 4,
                        "msgpack::object for glm::quat cannot be size [" << o.via.map.size << "]"
                    );
                    for (uint32_t i = 0; i < o.via.map.size; ++i) {
                        const std::string& key = o.via.map.ptr[i].key.as<std::string>();
                        const msgpack::object& val = o.via.map.ptr[i].val;
                        if (key == "x") {
                            v.x = val.as<float>();
                        } else if (key == "y") {
                            v.y = val.as<float>();
                        } else if (key == "z") {
                            v.z = val.as<float>();
                        } else if (key == "w") {
                            v.w = val.as<float>();
                        }
                    }
                    return o;
                }
            };
        }
    }
}
