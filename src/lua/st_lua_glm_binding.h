#pragma once

namespace Storytime {
    class LuaGLMBinding {
    public:
        static const std::string metatable_name;
        static const std::string vec2_metatable_name;
        static const std::string vec3_metatable_name;
        static const std::string vec4_metatable_name;
        static const std::string mat2_metatable_name;
        static const std::string mat3_metatable_name;
        static const std::string mat4_metatable_name;
        static const std::string quat_metatable_name;

    public:
        static int create_metatable(lua_State* L);

        static int create(lua_State* L);

    private:
        static int create_vec2_metatable(lua_State* L);

        static int create_vec3_metatable(lua_State* L);

        static int create_vec4_metatable(lua_State* L);

        static int create_mat2_metatable(lua_State* L);

        static int create_mat3_metatable(lua_State* L);

        static int create_mat4_metatable(lua_State* L);

        static int create_quat_metatable(lua_State* L);

        static int index(lua_State* L);

        static int index_mat3(lua_State* L);

        static int index_mat4(lua_State* L);

        static int index_quat(lua_State* L);

        static int index_vec2(lua_State* L);

        static int index_vec3(lua_State* L);

        static int index_vec4(lua_State* L);

        static int add_mat2(lua_State* L);

        static int add_mat3(lua_State* L);

        static int add_mat4(lua_State* L);

        static int add_quat(lua_State* L);

        static int add_vec2(lua_State* L);

        static int add_vec3(lua_State* L);

        static int add_vec4(lua_State* L);

        static int angle_axis(lua_State* L);

        static int concat_vec2(lua_State* L);

        static int concat_vec3(lua_State* L);

        static int concat_vec4(lua_State* L);

        static int cross(lua_State* L);

        static int degrees(lua_State* L);

        static int divide_mat2(lua_State* L);

        static int divide_mat3(lua_State* L);

        static int divide_mat4(lua_State* L);

        static int divide_quat(lua_State* L);

        static int divide_vec2(lua_State* L);

        static int divide_vec3(lua_State* L);

        static int divide_vec4(lua_State* L);

        static int dot_vec2(lua_State* L);

        static int dot_vec3(lua_State* L);

        static int dot_vec4(lua_State* L);

        static int euler_angles(lua_State* L);

        static int inverse_mat2(lua_State* L);

        static int inverse_mat3(lua_State* L);

        static int inverse_mat4(lua_State* L);

        static int inverse_quat(lua_State* L);

        static int length_vec2(lua_State* L);

        static int length_vec3(lua_State* L);

        static int length_vec4(lua_State* L);

        static int lerp(lua_State* L);

        static int look_at(lua_State* L);

        static int mat2(lua_State* L);

        static int mat3(lua_State* L);

        static int mat4(lua_State* L);

        static int mat3_to_quat(lua_State* L);

        static int mat4_to_quat(lua_State* L);

        static int multiply_mat2(lua_State* L);

        static int multiply_mat3(lua_State* L);

        static int multiply_mat4(lua_State* L);

        static int multiply_quat(lua_State* L);

        static int multiply_vec2(lua_State* L);

        static int multiply_vec3(lua_State* L);

        static int multiply_vec4(lua_State* L);

        static int normalize_quat(lua_State* L);

        static int normalize_vec2(lua_State* L);

        static int normalize_vec3(lua_State* L);

        static int normalize_vec4(lua_State* L);

        static int quat(lua_State* L);

        static int quat_look_at(lua_State* L);

        static int quat_look_at_rh(lua_State* L);

        static int quat_look_at_lh(lua_State* L);

        static int quat_to_mat4(lua_State* L);

        static int radians(lua_State* L);

        static int rotate(lua_State* L);

        static int rotate_x(lua_State* L);

        static int rotate_y(lua_State* L);

        static int rotate_z(lua_State* L);

        static int slerp(lua_State* L);

        static int subtract_mat2(lua_State* L);

        static int subtract_mat3(lua_State* L);

        static int subtract_mat4(lua_State* L);

        static int subtract_quat(lua_State* L);

        static int subtract_vec2(lua_State* L);

        static int subtract_vec3(lua_State* L);

        static int subtract_vec4(lua_State* L);

        static int to_string_vec2(lua_State* L);

        static int to_string_vec3(lua_State* L);

        static int to_string_vec4(lua_State* L);

        static int translate(lua_State* L);

        static int unary_minus_vec2(lua_State* L);

        static int unary_minus_vec3(lua_State* L);

        static int unary_minus_vec4(lua_State* L);

        static int vec2(lua_State* L);

        static int vec3(lua_State* L);

        static int vec4(lua_State* L);
    };
}

glm::vec2 lua_tovec2(lua_State* L, int index);

void lua_pushvec2(lua_State* L, const glm::vec2& vector);

glm::vec3 lua_tovec3(lua_State* L, int index);

void lua_pushvec3(lua_State* L, const glm::vec3& vector);

glm::vec4 lua_tovec4(lua_State* L, int index);

void lua_pushvec4(lua_State* L, const glm::vec4& vector);

glm::mat2 lua_tomat2(lua_State* L, int index);

void lua_pushmat2(lua_State* L, const glm::mat2& matrix);

glm::mat3 lua_tomat3(lua_State* L, int index);

void lua_pushmat3(lua_State* L, const glm::mat3& matrix);

glm::mat4 lua_tomat4(lua_State* L, int index);

void lua_pushmat4(lua_State* L, const glm::mat4& matrix);

glm::quat lua_toquat(lua_State* L, int index);

void lua_pushquat(lua_State* L, const glm::quat& quaternion);
