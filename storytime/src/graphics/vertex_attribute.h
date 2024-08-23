#pragma once

namespace Storytime {
    enum class GLSLType {
        None = 0,
        Bool,
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat2,
        Mat3,
        Mat4
    };

    struct VertexAttribute {
        GLSLType glsl_type;
        std::string_view name;
        u32 size;
        u32 length;
        u32 offset;
        bool normalized;

        VertexAttribute(GLSLType glsl_type, std::string_view name);

    private:
        u32 get_size() const;

        u32 get_length() const;
    };
}
