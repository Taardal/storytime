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
        uint32_t Size;
        uint32_t Length;
        uint32_t Offset;
        bool Normalized;

        VertexAttribute(GLSLType glslType, std::string_view name);

    private:
        uint32_t GetSize();

        uint32_t GetLength();
    };

}

