#include "VertexAttribute.h"

namespace Darkle {

    VertexAttribute::VertexAttribute(GLSLType glslType, std::string_view name)
            : glslType(glslType), name(name), size(getSize()), length(getLength()), offset(0), normalized(false) {}

    unsigned int VertexAttribute::getSize() {
        switch (glslType) {
            case GLSLType::Bool:
                return sizeof(bool);
            case GLSLType::Int:
                return sizeof(int);
            case GLSLType::Float:
                return sizeof(float);
            case GLSLType::Vec2:
                return sizeof(float) * 2;
            case GLSLType::Vec3:
                return sizeof(float) * 3;
            case GLSLType::Vec4:
                return sizeof(float) * 4;
            case GLSLType::Mat2:
                return sizeof(float) * 2 * 2;
            case GLSLType::Mat3:
                return sizeof(float) * 3 * 3;
            case GLSLType::Mat4:
                return sizeof(float) * 4 * 4;
            default:
                return 0;
        }
    }

    unsigned int VertexAttribute::getLength() {
        switch (glslType) {
            case GLSLType::Bool:
                return 1;
            case GLSLType::Int:
                return 1;
            case GLSLType::Float:
                return 1;
            case GLSLType::Vec2:
                return 2;
            case GLSLType::Vec3:
                return 3;
            case GLSLType::Vec4:
                return 4;
            case GLSLType::Mat2:
                return 2 * 2;
            case GLSLType::Mat3:
                return 3 * 3;
            case GLSLType::Mat4:
                return 4 * 4;
            default:
                return 0;
        }
    }

}
