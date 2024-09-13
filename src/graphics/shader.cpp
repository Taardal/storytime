#include "system/log.h"
#include "system/environment.h"
#include "shader.h"
#include "open_gl.h"

#include <glm/gtc/type_ptr.hpp>

namespace Storytime {
    Shader::Shader(const char* vertex_source, const char* fragment_source) : id(0) {
        u32 vertexShaderId = create_shader(vertex_source, GL_VERTEX_SHADER);
        u32 fragmentShaderId = create_shader(fragment_source, GL_FRAGMENT_SHADER);
        id = create_program(vertexShaderId, fragmentShaderId);
    }

    Shader::~Shader() {
        ST_GL_CALL(glDeleteProgram(id));
    }

    void Shader::bind() const {
        ST_GL_CALL(glUseProgram(id));
    }

    void Shader::unbind() const {
        ST_GL_CALL(glUseProgram(0));
    }

    void Shader::set_int1(const char* key, u32 value) const {
        ST_GL_CALL(i32 location = glGetUniformLocation(id, key));
        ST_GL_CALL(glUniform1i(location, value));
    }

    void Shader::set_mat4(const char* key, glm::mat4 value) const {
        ST_GL_CALL(i32 location = glGetUniformLocation(id, key));
        i32 count = 1;
        bool transpose = GL_FALSE;
        ST_GL_CALL(glUniformMatrix4fv(location, count, transpose, glm::value_ptr(value)));
    }

    void Shader::set_float4(const char* key, glm::vec4 value) const {
        ST_GL_CALL(i32 location = glGetUniformLocation(id, key));
        ST_GL_CALL(glUniform4f(location, value.x, value.y, value.z, value.w));
    }

    void Shader::set_int_array(const char* key, const i32* values, u32 count) const {
        ST_GL_CALL(i32 location = glGetUniformLocation(id, key));
        ST_GL_CALL(glUniform1iv(location, count, values));
    }

    u32 Shader::create_shader(const char* source, u32 type) const {
        const char* type_string = type == GL_VERTEX_SHADER ? ST_STRING(GL_VERTEX_SHADER) : ST_STRING(GL_FRAGMENT_SHADER);
        ST_LOG_DEBUG("Creating [{0}] shader", type_string);
        ST_GL_CALL(u32 shader_id = glCreateShader(type));
        ST_LOG_TRACE("Created shader with id [{0}]", shader_id);
        set_shader_source(source, shader_id);
        bool compiled = compile_shader(shader_id);
        if (compiled) {
            ST_LOG_DEBUG("Shader created successfully");
            return shader_id;
        }
        ST_LOG_TRACE("Deleting shader with id [{0}]", shader_id);
        ST_GL_CALL(glDeleteShader(shader_id));
        return 0;
    }

    void Shader::set_shader_source(const char* source, u32 shader_id) const {
        ST_LOG_TRACE("Setting source on shader [{0}] \n{1}", shader_id, source);
        i32 count = 1;
        i32* length = nullptr;
        ST_GL_CALL(glShaderSource(shader_id, count, &source, length));
        ST_LOG_TRACE("Set source on shader [{0}]", shader_id);
    }

    bool Shader::compile_shader(u32 shader_id) const {
        ST_LOG_TRACE("Compiling shader with id [{0}]", shader_id);
        ST_GL_CALL(glCompileShader(shader_id));
        i32 status = 0;
        ST_GL_CALL(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status));
        bool compiled = status != 0;
        if (!compiled) {
            ST_LOG_ERROR("Could not compile shader with id [{0}]: [{1}]", shader_id, get_shader_log(shader_id));
        }
        return compiled;
    }

    std::string Shader::get_shader_log(u32 shader_id) const {
        i32 length = 0;
        ST_GL_CALL(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> log(length);
        ST_GL_CALL(glGetShaderInfoLog(shader_id, length, &length, &log[0]));
        return log.data();
    }

    u32 Shader::create_program(u32 vertex_shader_id, u32 fragment_shader_id) const {
        ST_LOG_TRACE(
            "Creating program using vertex shader with id [{0}] and fragment shader with id [{1}]",
            vertex_shader_id,
            fragment_shader_id
        );
        u32 program_id = glCreateProgram();
        ST_LOG_TRACE("Attaching vertex shader with id [{0}]", vertex_shader_id);
        ST_GL_CALL(glAttachShader(program_id, vertex_shader_id));
        ST_LOG_TRACE("Attaching fragment shader with id [{0}]", fragment_shader_id);
        ST_GL_CALL(glAttachShader(program_id, fragment_shader_id));
        bool linked = link_program(program_id);
        if (linked) {
            ST_LOG_TRACE("Detaching vertex shader with id [{0}]", vertex_shader_id);
            ST_GL_CALL(glDetachShader(program_id, vertex_shader_id));
            ST_LOG_TRACE("Detaching fragment shader with id [{0}]", fragment_shader_id);
            ST_GL_CALL(glDetachShader(program_id, fragment_shader_id));
            ST_LOG_DEBUG("Shader program created successfully", program_id);
            return program_id;
        }
        ST_LOG_TRACE("Deleting shader program with id [{0}]", program_id);
        ST_GL_CALL(glDeleteProgram(program_id));
        ST_LOG_TRACE("Deleting vertex shader with id [{0}]", vertex_shader_id);
        ST_GL_CALL(glDeleteShader(vertex_shader_id));
        ST_LOG_TRACE("Deleting fragment shader with id [{0}]", fragment_shader_id);
        ST_GL_CALL(glDeleteShader(fragment_shader_id));
        return 0;
    }

    bool Shader::link_program(u32 id) const {
        ST_LOG_TRACE("Linking shader program with id [{0}]", id);
        ST_GL_CALL(glLinkProgram(id));
        i32 status = 0;
        ST_GL_CALL(glGetProgramiv(id, GL_LINK_STATUS, &status));
        bool linked = status != 0;
        if (!linked) {
            ST_LOG_ERROR("Could not link shader program with id [{0}]: [{1}]", id, get_program_log(id));
        }
        return linked;
    }

    std::string Shader::get_program_log(u32 program_id) const {
        i32 log_length = 0;
        ST_GL_CALL(glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length));
        std::vector<char> log(log_length);
        ST_GL_CALL(glGetProgramInfoLog(program_id, log_length, &log_length, &log[0]));
        return log.data();
    }
}
