#pragma once

#include "vertex_array.h"
#include "shader.h"
#include "texture.h"

namespace Storytime {

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

    struct RendererStatistics {
        u32 draw_calls = 0;
        u32 quad_count = 0;
        u32 vertex_count = 0;
        u32 index_count = 0;
    };

    class Renderer {
    private:
        struct Vertex {
            glm::vec3 position = {0.0f, 0.0f, 0.0f};
            glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec2 texture_coordinate = {0.0f, 0.0f};
            f32 texture_index = 0.0f;
            f32 tiling_factor = 1.0f;
        };

    private:
        static const u32 VERTICES_PER_QUAD;
        static const u32 INDICES_PER_QUAD;
        static const u32 QUADS_PER_BATCH;
        static const u32 VERTICES_PER_BATCH;
        static const u32 INDICES_PER_BATCH;
        static const u32 MAX_TEXTURE_SLOTS;

    private:
        glm::vec4 clear_color = {1.0f, 0.0f, 1.0f, 1};
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

    public:
        explicit Renderer(const ResourceLoader* resource_loader);

        ~Renderer();

        RendererStatistics get_statistics() const;

        void set_clear_color(const glm::vec3& color);

        void set_clear_color(const glm::vec4& color);

        void set_viewport(u32 width, u32 height, u32 x = 0, u32 y = 0) const;

        void begin_frame(const glm::mat4& view_projection);

        void submit_quad(const Quad& quad);

        void submit_quad(const Quad& quad, const std::vector<glm::vec2>& texture_coordinates);

        void end_frame();

    private:
        void reset();

        void draw_indexed();

        void flush();
    };
}
