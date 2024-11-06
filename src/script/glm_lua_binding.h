#pragma once

namespace Storytime {
    class GLMLuaBinding {
    public:
        static const std::string table_name;
        static const std::string metatable_name;
        static const std::string vec2_metatable_name;
        static const std::string vec3_metatable_name;
        static const std::string vec4_metatable_name;
        static const std::string mat2_metatable_name;
        static const std::string mat3_metatable_name;
        static const std::string mat4_metatable_name;
        static const std::string quat_metatable_name;

    public:
        static i32 create(lua_State* L);

    private:
        static void createVec2Metatable(lua_State* L);

        static void createVec3Metatable(lua_State* L);

        static void createVec4Metatable(lua_State* L);

        static void createMat2Metatable(lua_State* L);

        static void createMat3Metatable(lua_State* L);

        static void createMat4Metatable(lua_State* L);

        static void createQuatMetatable(lua_State* L);

        static int index(lua_State* L);

        static int indexMat3(lua_State* L);

        static int indexMat4(lua_State* L);

        static int indexQuat(lua_State* L);

        static int indexVec2(lua_State* L);

        static int indexVec3(lua_State* L);

        static int indexVec4(lua_State* L);

        static int addMat2(lua_State* L);

        static int addMat3(lua_State* L);

        static int addMat4(lua_State* L);

        static int addQuat(lua_State* L);

        static int addVec2(lua_State* L);

        static int addVec3(lua_State* L);

        static int addVec4(lua_State* L);

        static int angleAxis(lua_State* L);

        static int cross(lua_State* L);

        static int degrees(lua_State* L);

        static int divideMat2(lua_State* L);

        static int divideMat3(lua_State* L);

        static int divideMat4(lua_State* L);

        static int divideQuat(lua_State* L);

        static int divideVec2(lua_State* L);

        static int divideVec3(lua_State* L);

        static int divideVec4(lua_State* L);

        static int dotVec2(lua_State* L);

        static int dotVec3(lua_State* L);

        static int dotVec4(lua_State* L);

        static int eulerAngles(lua_State* L);

        static int inverseQuat(lua_State* L);

        static int inverseMat2(lua_State* L);

        static int inverseMat3(lua_State* L);

        static int inverseMat4(lua_State* L);

        static int lengthVec2(lua_State* L);

        static int lengthVec3(lua_State* L);

        static int lengthVec4(lua_State* L);

        static int lerp(lua_State* L);

        static int lookAt(lua_State* L);

        static int mat2(lua_State* L);

        static int mat3(lua_State* L);

        static int mat4(lua_State* L);

        static int mat3ToQuat(lua_State* L);

        static int mat4ToQuat(lua_State* L);

        static int multiplyMat2(lua_State* L);

        static int multiplyMat3(lua_State* L);

        static int multiplyMat4(lua_State* L);

        static int multiplyQuat(lua_State* L);

        static int multiplyVec2(lua_State* L);

        static int multiplyVec3(lua_State* L);

        static int multiplyVec4(lua_State* L);

        static int normalizeVec2(lua_State* L);

        static int normalizeVec3(lua_State* L);

        static int normalizeVec4(lua_State* L);

        static int normalizeQuat(lua_State* L);

        static int quat(lua_State* L);

        static int quatLookAt(lua_State* L);

        static int quatLookAtRH(lua_State* L);

        static int quatLookAtLH(lua_State* L);

        static int quatToMat4(lua_State* L);

        static int radians(lua_State* L);

        static int rotate(lua_State* L);

        static int rotateX(lua_State* L);

        static int rotateY(lua_State* L);

        static int rotateZ(lua_State* L);

        static int slerp(lua_State* L);

        static int subtractMat2(lua_State* L);

        static int subtractMat3(lua_State* L);

        static int subtractMat4(lua_State* L);

        static int subtractQuat(lua_State* L);

        static int subtractVec2(lua_State* L);

        static int subtractVec3(lua_State* L);

        static int subtractVec4(lua_State* L);

        static int toStringVec2(lua_State* L);

        static int toStringVec3(lua_State* L);

        static int toStringVec4(lua_State* L);

        static int translate(lua_State* L);

        static int unaryMinusVec2(lua_State* L);

        static int unaryMinusVec3(lua_State* L);

        static int unaryMinusVec4(lua_State* L);

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
