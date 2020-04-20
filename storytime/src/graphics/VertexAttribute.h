#pragma once

#include "string_view"

namespace storytime
{
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
        GLSLType GlslType;
        std::string_view Name;
        unsigned int Size;
        unsigned int Length;
        unsigned int Offset;
        bool Normalized;

        VertexAttribute(GLSLType glslType, std::string_view name);

    private:
        unsigned int GetSize();

        unsigned int GetLength();
    };

}

