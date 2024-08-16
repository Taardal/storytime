#pragma once

namespace Storytime {
    class Shader {
    private:
        u32 id;

    public:
        Shader(const char* vertex_source, const char* fragment_source);

        ~Shader();

        void bind() const;

        void unbind() const;

        void set_int1(const char* key, u32 value) const;

        void set_mat4(const char* key, glm::mat4 value) const;

        void set_float4(const char* key, glm::vec4 value) const;

        void set_int_array(const char* key, const int32_t* values, u32 count) const;

    private:
        u32 create_shader(const char* source, u32 type) const;

        void set_shader_source(const char* source, u32 shader_id) const;

        bool compile_shader(u32 shader_id) const;

        std::string get_shader_log(u32 shader_id) const;

        u32 create_program(u32 vertex_shader_id, u32 fragment_shader_id) const;

        bool link_program(u32 id) const;

        std::string get_program_log(u32 program_id) const;
    };
}
