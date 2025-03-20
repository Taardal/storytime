#include "st_texture_coordinate.h"

namespace Storytime {
    TextureCoordinate::TextureCoordinate(f32 x, f32 y) : x(x), y(y) {
    }

    TextureCoordinate::TextureCoordinate(const glm::vec2& coordinate) : x(coordinate.x), y(coordinate.y) {
    }

    TextureCoordinate::operator glm::vec<2, float>() const {
        return {x, y};
    }

    QuadTextureCoordinates::QuadTextureCoordinates(
        const TextureCoordinate& top_left,
        const TextureCoordinate& top_right,
        const TextureCoordinate& bottom_right,
        const TextureCoordinate& bottom_left
    ) : top_left(top_left),
        top_right(top_right),
        bottom_right(bottom_right),
        bottom_left(bottom_left) {
    }

    QuadTextureCoordinates::QuadTextureCoordinates(const std::array<TextureCoordinate, 4>& coordinates)
        : top_left(coordinates[top_left_index]),
          top_right(coordinates[top_right_index]),
          bottom_right(coordinates[bottom_right_index]),
          bottom_left(coordinates[bottom_left_index]) {
    }

    QuadTextureCoordinates::operator std::array<TextureCoordinate, 4>() const {
        std::array<TextureCoordinate, 4> texture_coordinates;
        texture_coordinates[top_left_index] = top_left;
        texture_coordinates[top_right_index] = top_right;
        texture_coordinates[bottom_right_index] = bottom_right;
        texture_coordinates[bottom_left_index] = bottom_left;
        return texture_coordinates;
    }

    TextureCoordinate& QuadTextureCoordinates::operator[](u8 index) {
        ST_ASSERT(index < 4, "Texture coordinate index out of bounds");
        if (index == 3) {
            return bottom_left;
        }
        if (index == 2) {
            return bottom_right;
        }
        if (index == 1) {
            return top_right;
        }
        return top_left;
    }
}
