#pragma once

#include "vertex_array.h"
#include "shader.h"
#include "texture.h"
#include "resource/resource_loader.h"

namespace Storytime {
    struct Quad {
        Shared<Texture> texture;
        glm::vec3 position;
        glm::vec2 size;
        glm::vec4 color;
        f32 rotation_in_degrees;
        f32 tiling_factor;

        Quad();
    };

    class Renderer {
    public:
        struct Statistics {
            u32 draw_calls;
            u32 quad_count;

            Statistics();

            u32 get_vertex_count() const;

            u32 get_index_count() const;
        };

    private:
        struct Vertex {
            glm::vec3 position;
            glm::vec4 color;
            glm::vec2 texture_coordinate;
            f32 texture_index;
            f32 tiling_factor;

            Vertex();
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
        Statistics statistics;

    public:
        explicit Renderer(const ResourceLoader* resource_loader);

        ~Renderer();

        Statistics get_statistics() const;

        void set_clear_color(const glm::vec3& color);

        void set_clear_color(const glm::vec4& color);

        void set_viewport(u32 width, u32 height, u32 x = 0, u32 y = 0) const;

        void begin_frame(const glm::mat4& view_projection);

        void submit_quad(Quad& quad);

        void end_frame();

    private:
        void reset();

        void draw_indexed();

        void flush();
    };
}
