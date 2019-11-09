#pragma once

#include "string_view"

namespace storytime {

    enum class GLSLType
    {
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

    struct VertexAttribute
    {
        GLSLType glslType;
        std::string_view name;
        unsigned int size;
        unsigned int length;
        unsigned int offset;
        bool normalized;

        VertexAttribute(GLSLType glslType, std::string_view name);

    private:
        unsigned int getSize();
        unsigned int getLength();
    };

}

