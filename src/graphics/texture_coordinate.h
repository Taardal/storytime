#pragma once

namespace Storytime {
    struct TextureCoordinate {
        static constexpr u8 top_left = 0;
        static constexpr u8 top_right = 1;
        static constexpr u8 bottom_right = 2;
        static constexpr u8 bottom_left = 3;

        f32 x = 0.0f;
        f32 y = 0.0f;

        TextureCoordinate() = default;

        TextureCoordinate(f32 x, f32 y);

        TextureCoordinate(const glm::vec2& coordinate);

        operator glm::vec2() const;
    };

    struct QuadTextureCoordinates {
        static constexpr u8 top_left_index = 0;
        static constexpr u8 top_right_index = 1;
        static constexpr u8 bottom_right_index = 2;
        static constexpr u8 bottom_left_index = 3;

        TextureCoordinate top_left = {0.0f, 0.0f};
        TextureCoordinate top_right = {0.0f, 0.0f};
        TextureCoordinate bottom_right = {0.0f, 0.0f};
        TextureCoordinate bottom_left = {0.0f, 0.0f};

        QuadTextureCoordinates() = default;

        QuadTextureCoordinates(
            const TextureCoordinate& top_left,
            const TextureCoordinate& top_right,
            const TextureCoordinate& bottom_right,
            const TextureCoordinate& bottom_left
        );

        QuadTextureCoordinates(const std::array<TextureCoordinate, 4>& coordinates);

        operator std::array<TextureCoordinate, 4>() const;

        TextureCoordinate& operator [](u8 index);
    };
}
