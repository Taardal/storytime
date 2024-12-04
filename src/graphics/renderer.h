#pragma once

#include "vertex_array.h"
#include "shader.h"
#include "texture.h"
#include "view_projection.h"
#include "texture_coordinate.h"

namespace Storytime {

    typedef glm::vec4 QuadOffset;

    // Forward declaration
    class ResourceLoader;

    struct Quad {
        Shared<Texture> texture = nullptr;
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec2 size = {0.0f, 0.0f};
        glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        f32 rotation_in_degrees = 0.0f;
        f32 tiling_factor = 1.0f;
    };

    struct Viewport {
        i32 width = 0;
        i32 height = 0;
        i32 x = 0;
        i32 y = 0;
    };

    struct RendererStatistics {
        u32 draw_calls = 0;
        u32 quad_count = 0;
        u32 vertex_count = 0;
        u32 index_count = 0;
    };

    struct RendererConfig {
        ResourceLoader* resource_loader = nullptr;
        glm::vec4 clear_color = {0.1f, 0.1f, 0.1f, 1.0f};
        Viewport viewport{};
    };

    class Renderer {
    private:
        struct Vertex {
            glm::vec3 position = {0.0f, 0.0f, 0.0f};
            glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
            TextureCoordinate texture_coordinate = glm::vec2({0.0f, 0.0f});
            f32 texture_index = 0.0f;
            f32 tiling_factor = 1.0f;
        };

    private:
        static constexpr u32 VERTICES_PER_QUAD = 4;
        static constexpr u32 INDICES_PER_QUAD = 6;
        static constexpr u32 QUADS_PER_BATCH = 1000;
        static constexpr u32 MAX_TEXTURE_SLOTS = 16;
        static const u32 VERTICES_PER_BATCH;
        static const u32 INDICES_PER_BATCH;

    private:
        RendererConfig config;
        Shared<VertexArray> vertex_array;
        Shared<VertexBuffer> vertex_buffer;
        Shared<Shader> shader;
        Shared<Texture>* textures;
        Shared<Texture> white_texture;
        Vertex* vertices;
        u32* indices;
        u32 vertex_count;
        u32 index_count;
        u32 texture_count;
        u32 reserved_textures_count;
        RendererStatistics statistics;
        std::array<TextureCoordinate, 4> default_texture_coordinates;
        std::array<glm::vec4, 4> origin_quad_offsets;

    public:
        explicit Renderer(const RendererConfig& config);

        ~Renderer();

        RendererStatistics get_statistics() const;

        static void set_clear_color(const glm::vec4& clear_color);

        static void set_viewport(const Viewport& viewport);

        void set_view_projection(const ViewProjection& view_projection) const;

        void begin_frame();

        void render_quad(const Quad& quad);

        void render_quad(const Quad& quad, const std::array<TextureCoordinate, 4>& texture_coordinates);

        void end_frame();

    private:
        void reset();

        void draw_indexed();

        void flush();
    };
}
