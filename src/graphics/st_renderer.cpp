#include "st_renderer.h"
#include "system/st_log.h"
#include "system/st_environment.h"
#include "graphics/st_open_gl.h"
#include "resource/st_resource_loader.h"

namespace Storytime {
    constexpr u32 Renderer::VERTICES_PER_BATCH = QUADS_PER_BATCH * VERTICES_PER_QUAD;
    constexpr u32 Renderer::INDICES_PER_BATCH = QUADS_PER_BATCH * INDICES_PER_QUAD;

    Renderer::Renderer(const RendererConfig& config)
        : vertex_array(shared<VertexArray>()),
          vertex_buffer(shared<VertexBuffer>(sizeof(Vertex) * VERTICES_PER_BATCH)),
          shader(config.resource_loader->load_shader("res/shaders/texture.vertex.glsl", "res/shaders/texture.fragment.glsl")),
          textures(new Shared<Texture>[MAX_TEXTURE_SLOTS]),
          white_texture(shared<Texture>(1, 1)),
          vertices(new Vertex[VERTICES_PER_BATCH]),
          indices(new u32[INDICES_PER_BATCH]),
          vertex_count(0),
          index_count(0),
          texture_count(0),
          reserved_textures_count(0),
          statistics()
    {
        ST_GL_CALL(glEnable(GL_BLEND));
        ST_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        ST_GL_CALL(glClearColor(config.clear_color.r, config.clear_color.g, config.clear_color.b, config.clear_color.a));
        ST_GL_CALL(glViewport(config.viewport.x, config.viewport.y, config.viewport.width, config.viewport.height));

        vertex_buffer->set_attribute_layout({
            {GLSLType::Vec3, "position"},
            {GLSLType::Vec4, "color"},
            {GLSLType::Vec2, "textureCoordinate"},
            {GLSLType::Float, "textureIndex"},
            {GLSLType::Float, "tilingFactor"}
        });

        u32 offset = 0;
        for (u32 i = 0; i < INDICES_PER_BATCH; i += INDICES_PER_QUAD) {
            indices[i + 0] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;
            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;
            offset += VERTICES_PER_QUAD;
        }
        auto index_buffer = shared<IndexBuffer>(indices, INDICES_PER_BATCH);

        vertex_array->add_vertex_buffer(vertex_buffer);
        vertex_array->set_index_buffer(index_buffer);
        vertex_array->bind();

        u32 white_texture_pixels = 0xffffffff;
        white_texture->set_pixels(&white_texture_pixels);
        textures[0] = white_texture;
        texture_count++;
        reserved_textures_count++;

        i32 samplers[MAX_TEXTURE_SLOTS];
        for (u32 i = 0; i < MAX_TEXTURE_SLOTS; i++) {
            samplers[i] = i;
        }
        shader->bind();
        shader->set_int_array("textureSamplers", samplers, MAX_TEXTURE_SLOTS);

        default_texture_coordinates[0] = {0.0f, 0.0f}; // Top left
        default_texture_coordinates[1] = {1.0f, 0.0f}; // Top right
        default_texture_coordinates[2] = {1.0f, 1.0f}; // Bottom right
        default_texture_coordinates[3] = {0.0f, 1.0f}; // Bottom left

        origin_quad_offsets[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Top left
        origin_quad_offsets[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Top right
        origin_quad_offsets[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // Bottom right
        origin_quad_offsets[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Bottom left
    }

    Renderer::~Renderer() {
        shader->unbind();
        vertex_array->unbind();
        delete[] textures;
        delete[] indices;
        delete[] vertices;
    }

    RendererStatistics Renderer::get_statistics() const {
        return statistics;
    }

    void Renderer::set_clear_color(const glm::vec4& clear_color) {
        ST_GL_CALL(glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a));
    }

    void Renderer::set_viewport(const Viewport& viewport) {
        ST_GL_CALL(glViewport(viewport.x, viewport.y, viewport.width, viewport.height));
    }

    void Renderer::set_view_projection(const ViewProjection& view_projection) const {
        shader->set_mat4("viewProjection", view_projection);
    }

    void Renderer::begin_frame() {
        reset();
        ST_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void Renderer::end_frame() {
        flush();
        statistics.vertex_count = statistics.quad_count * VERTICES_PER_QUAD;
        statistics.index_count = statistics.quad_count * INDICES_PER_QUAD;
    }

    void Renderer::render_quad(const Quad& quad) {
        render_quad(quad, default_texture_coordinates);
    }

    void Renderer::render_quad(const Quad& quad, const std::array<TextureCoordinate, 4>& texture_coordinates) {
        if (index_count >= INDICES_PER_BATCH) {
            flush();
            reset();
        }

        Shared<Texture> texture = quad.texture != nullptr ? quad.texture : white_texture;
        f32 texture_index = -1.0f;
        for (u32 i = 0; i < texture_count; i++) {
            if (textures[i] == texture) {
                texture_index = (f32) i;
                break;
            }
        }
        if (texture_index == -1.0f) {
            texture_index = (f32) texture_count;
            textures[texture_count] = texture;
            texture_count++;
        }

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), quad.position);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation_in_degrees), {0.0f, 0.0f, 1.0f});
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), {quad.size.x, quad.size.y, 1.0f});
        glm::mat4 model = translation * rotation * scale;

        for (int i = 0; i < VERTICES_PER_QUAD; ++i) {
            vertices[vertex_count].position = model * origin_quad_offsets[i];
            vertices[vertex_count].texture_coordinate = texture_coordinates[i];
            vertices[vertex_count].texture_index = texture_index;
            vertices[vertex_count].tiling_factor = quad.tiling_factor;
            vertices[vertex_count].color = quad.color;
            vertex_count++;
        }
        index_count += INDICES_PER_QUAD;
        statistics.quad_count++;
    }

    void Renderer::reset() {
        vertex_count = 0;
        index_count = 0;
        texture_count = reserved_textures_count;
        memset(&statistics, 0, sizeof(RendererStatistics));
    }

    void Renderer::flush() {
        vertex_buffer->set_vertices(vertices, vertex_count * sizeof(Vertex));
        for (u32 i = 0; i < texture_count; i++) {
            textures[i]->bind(i);
        }
        draw_indexed();
    }

    void Renderer::draw_indexed() {
        void* offset = nullptr;
        ST_GL_CALL(glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, offset));
        statistics.draw_calls++;
    }
}
