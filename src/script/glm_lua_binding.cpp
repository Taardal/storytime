#include "script/glm_lua_binding.h"

namespace Storytime {
    const std::string GLMLuaBinding::table_name = "glm";
    const std::string GLMLuaBinding::metatable_name = table_name + "__meta";
    const std::string GLMLuaBinding::vec2_metatable_name = table_name + ".vec2__meta";
    const std::string GLMLuaBinding::vec3_metatable_name = table_name + ".vec3__meta";
    const std::string GLMLuaBinding::vec4_metatable_name = table_name + ".vec4__meta";
    const std::string GLMLuaBinding::mat2_metatable_name = table_name + ".mat2__meta";
    const std::string GLMLuaBinding::mat3_metatable_name = table_name + ".mat3__meta";
    const std::string GLMLuaBinding::mat4_metatable_name = table_name + ".mat4__meta";
    const std::string GLMLuaBinding::quat_metatable_name = table_name + ".quat__meta";

    i32 GLMLuaBinding::create(lua_State* L) {
        createVec2Metatable(L);
        createVec3Metatable(L);
        createVec4Metatable(L);
        createMat2Metatable(L);
        createMat3Metatable(L);
        createMat4Metatable(L);
        createQuatMetatable(L);

        lua_newtable(L);
        luaL_newmetatable(L, metatable_name.c_str());
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::index);
        lua_settable(L, -3);
        lua_setmetatable(L, -2);
        i32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
        ST_ASSERT(ref != 0, "Invalid Lua ref");
        return ref;
    }

    void GLMLuaBinding::createVec2Metatable(lua_State* L) {
        luaL_newmetatable(L, vec2_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::indexVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__len");
        lua_pushcfunction(L, GLMLuaBinding::lengthVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__unm");
        lua_pushcfunction(L, GLMLuaBinding::unaryMinusVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, GLMLuaBinding::toStringVec2);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    void GLMLuaBinding::createVec3Metatable(lua_State* L) {
        luaL_newmetatable(L, vec3_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::indexVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__len");
        lua_pushcfunction(L, GLMLuaBinding::lengthVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__unm");
        lua_pushcfunction(L, GLMLuaBinding::unaryMinusVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, GLMLuaBinding::toStringVec3);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    void GLMLuaBinding::createVec4Metatable(lua_State* L) {
        luaL_newmetatable(L, vec4_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::indexVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__len");
        lua_pushcfunction(L, GLMLuaBinding::lengthVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__unm");
        lua_pushcfunction(L, GLMLuaBinding::unaryMinusVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, GLMLuaBinding::toStringVec4);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    void GLMLuaBinding::createMat2Metatable(lua_State* L) {
        luaL_newmetatable(L, mat2_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addMat2);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideMat2);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyMat2);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractMat2);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    void GLMLuaBinding::createMat3Metatable(lua_State* L) {
        luaL_newmetatable(L, mat3_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::indexMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractMat3);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    void GLMLuaBinding::createMat4Metatable(lua_State* L) {
        luaL_newmetatable(L, mat4_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::indexMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractMat4);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    void GLMLuaBinding::createQuatMetatable(lua_State* L) {
        luaL_newmetatable(L, quat_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, GLMLuaBinding::addQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, GLMLuaBinding::divideQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, GLMLuaBinding::indexQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, GLMLuaBinding::multiplyQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, GLMLuaBinding::subtractQuat);
        lua_settable(L, -3);

        lua_pop(L, 1); // Pop the metatable off the stack
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] userdata  Binding
    int GLMLuaBinding::index(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "addMat2") {
            lua_pushcfunction(L, GLMLuaBinding::addMat2);
            return 1;
        }
        if (indexName == "addMat3") {
            lua_pushcfunction(L, GLMLuaBinding::addMat3);
            return 1;
        }
        if (indexName == "addMat4") {
            lua_pushcfunction(L, GLMLuaBinding::addMat4);
            return 1;
        }
        if (indexName == "addQuat") {
            lua_pushcfunction(L, GLMLuaBinding::addQuat);
            return 1;
        }
        if (indexName == "addVec2") {
            lua_pushcfunction(L, GLMLuaBinding::addVec2);
            return 1;
        }
        if (indexName == "addVec3") {
            lua_pushcfunction(L, GLMLuaBinding::addVec3);
            return 1;
        }
        if (indexName == "addVec4") {
            lua_pushcfunction(L, GLMLuaBinding::addVec4);
            return 1;
        }
        if (indexName == "angleAxis") {
            lua_pushcfunction(L, GLMLuaBinding::angleAxis);
            return 1;
        }
        if (indexName == "cross") {
            lua_pushcfunction(L, GLMLuaBinding::cross);
            return 1;
        }
        if (indexName == "degrees") {
            lua_pushcfunction(L, GLMLuaBinding::degrees);
            return 1;
        }
        if (indexName == "divideMat2") {
            lua_pushcfunction(L, GLMLuaBinding::divideMat2);
            return 1;
        }
        if (indexName == "divideMat3") {
            lua_pushcfunction(L, GLMLuaBinding::divideMat3);
            return 1;
        }
        if (indexName == "divideMat4") {
            lua_pushcfunction(L, GLMLuaBinding::divideMat4);
            return 1;
        }
        if (indexName == "divideQuat") {
            lua_pushcfunction(L, GLMLuaBinding::divideQuat);
            return 1;
        }
        if (indexName == "divideVec2") {
            lua_pushcfunction(L, GLMLuaBinding::divideVec2);
            return 1;
        }
        if (indexName == "divideVec3") {
            lua_pushcfunction(L, GLMLuaBinding::divideVec3);
            return 1;
        }
        if (indexName == "divideVec4") {
            lua_pushcfunction(L, GLMLuaBinding::divideVec4);
            return 1;
        }
        if (indexName == "dotVec2") {
            lua_pushcfunction(L, GLMLuaBinding::dotVec2);
            return 1;
        }
        if (indexName == "dotVec3") {
            lua_pushcfunction(L, GLMLuaBinding::dotVec3);
            return 1;
        }
        if (indexName == "dotVec4") {
            lua_pushcfunction(L, GLMLuaBinding::dotVec4);
            return 1;
        }
        if (indexName == "eulerAngles") {
            lua_pushcfunction(L, GLMLuaBinding::eulerAngles);
            return 1;
        }
        if (indexName == "inverseQuat") {
            lua_pushcfunction(L, GLMLuaBinding::inverseQuat);
            return 1;
        }
        if (indexName == "inverseMat2") {
            lua_pushcfunction(L, GLMLuaBinding::inverseMat2);
            return 1;
        }
        if (indexName == "inverseMat3") {
            lua_pushcfunction(L, GLMLuaBinding::inverseMat3);
            return 1;
        }
        if (indexName == "inverseMat4") {
            lua_pushcfunction(L, GLMLuaBinding::inverseMat4);
            return 1;
        }
        if (indexName == "lengthVec2") {
            lua_pushcfunction(L, GLMLuaBinding::lengthVec2);
            return 1;
        }
        if (indexName == "lengthVec3") {
            lua_pushcfunction(L, GLMLuaBinding::lengthVec3);
            return 1;
        }
        if (indexName == "lengthVec4") {
            lua_pushcfunction(L, GLMLuaBinding::lengthVec4);
            return 1;
        }
        if (indexName == "lerp") {
            lua_pushcfunction(L, GLMLuaBinding::lerp);
            return 1;
        }
        if (indexName == "lookAt") {
            lua_pushcfunction(L, GLMLuaBinding::lookAt);
            return 1;
        }
        if (indexName == "mat2") {
            lua_pushcfunction(L, GLMLuaBinding::mat2);
            return 1;
        }
        if (indexName == "mat3") {
            lua_pushcfunction(L, GLMLuaBinding::mat3);
            return 1;
        }
        if (indexName == "mat4") {
            lua_pushcfunction(L, GLMLuaBinding::mat4);
            return 1;
        }
        if (indexName == "mat3ToQuat") {
            lua_pushcfunction(L, GLMLuaBinding::mat3ToQuat);
            return 1;
        }
        if (indexName == "mat4ToQuat") {
            lua_pushcfunction(L, GLMLuaBinding::mat4ToQuat);
            return 1;
        }
        if (indexName == "multiplyMat2") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyMat2);
            return 1;
        }
        if (indexName == "multiplyMat3") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyMat3);
            return 1;
        }
        if (indexName == "multiplyMat4") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyMat4);
            return 1;
        }
        if (indexName == "multiplyQuat") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyQuat);
            return 1;
        }
        if (indexName == "multiplyVec2") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyVec2);
            return 1;
        }
        if (indexName == "multiplyVec3") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyVec3);
            return 1;
        }
        if (indexName == "multiplyVec4") {
            lua_pushcfunction(L, GLMLuaBinding::multiplyVec4);
            return 1;
        }
        if (indexName == "normalizeVec2") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeVec2);
            return 1;
        }
        if (indexName == "normalizeVec3") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeVec3);
            return 1;
        }
        if (indexName == "normalizeVec4") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeVec4);
            return 1;
        }
        if (indexName == "normalizeQuat") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeQuat);
            return 1;
        }
        if (indexName == "quat") {
            lua_pushcfunction(L, GLMLuaBinding::quat);
            return 1;
        }
        if (indexName == "quatLookAt") {
            lua_pushcfunction(L, GLMLuaBinding::quatLookAt);
            return 1;
        }
        if (indexName == "quatLookAtRH") {
            lua_pushcfunction(L, GLMLuaBinding::quatLookAtRH);
            return 1;
        }
        if (indexName == "quatLookAtLH") {
            lua_pushcfunction(L, GLMLuaBinding::quatLookAtLH);
            return 1;
        }
        if (indexName == "quatToMat4") {
            lua_pushcfunction(L, GLMLuaBinding::quatToMat4);
            return 1;
        }
        if (indexName == "radians") {
            lua_pushcfunction(L, GLMLuaBinding::radians);
            return 1;
        }
        if (indexName == "rotate") {
            lua_pushcfunction(L, GLMLuaBinding::rotate);
            return 1;
        }
        if (indexName == "rotateX") {
            lua_pushcfunction(L, GLMLuaBinding::rotateX);
            return 1;
        }
        if (indexName == "rotateY") {
            lua_pushcfunction(L, GLMLuaBinding::rotateY);
            return 1;
        }
        if (indexName == "rotateZ") {
            lua_pushcfunction(L, GLMLuaBinding::rotateZ);
            return 1;
        }
        if (indexName == "slerp") {
            lua_pushcfunction(L, GLMLuaBinding::slerp);
            return 1;
        }
        if (indexName == "subtractMat2") {
            lua_pushcfunction(L, GLMLuaBinding::subtractMat2);
            return 1;
        }
        if (indexName == "subtractMat3") {
            lua_pushcfunction(L, GLMLuaBinding::subtractMat3);
            return 1;
        }
        if (indexName == "subtractMat4") {
            lua_pushcfunction(L, GLMLuaBinding::subtractMat4);
            return 1;
        }
        if (indexName == "subtractQuat") {
            lua_pushcfunction(L, GLMLuaBinding::subtractQuat);
            return 1;
        }
        if (indexName == "subtractVec2") {
            lua_pushcfunction(L, GLMLuaBinding::subtractVec2);
            return 1;
        }
        if (indexName == "subtractVec3") {
            lua_pushcfunction(L, GLMLuaBinding::subtractVec3);
            return 1;
        }
        if (indexName == "subtractVec4") {
            lua_pushcfunction(L, GLMLuaBinding::subtractVec4);
            return 1;
        }
        if (indexName == "translate") {
            lua_pushcfunction(L, GLMLuaBinding::translate);
            return 1;
        }
        if (indexName == "vec2") {
            lua_pushcfunction(L, GLMLuaBinding::vec2);
            return 1;
        }
        if (indexName == "vec3") {
            lua_pushcfunction(L, GLMLuaBinding::vec3);
            return 1;
        }
        if (indexName == "vec4") {
            lua_pushcfunction(L, GLMLuaBinding::vec4);
            return 1;
        }
        ST_LOG_WARNING("Could not resolve index [{}]", indexName);
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Mat3 (self)
    int GLMLuaBinding::indexMat3(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "toQuat") {
            lua_pushcfunction(L, GLMLuaBinding::mat3ToQuat);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Mat4 (self)
    int GLMLuaBinding::indexMat4(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "toQuat") {
            lua_pushcfunction(L, GLMLuaBinding::mat4ToQuat);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Quaternion (self)
    int GLMLuaBinding::indexQuat(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "inverse") {
            lua_pushcfunction(L, GLMLuaBinding::inverseQuat);
            return 1;
        }
        if (indexName == "normalize") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeQuat);
            return 1;
        }
        if (indexName == "toMat4") {
            lua_pushcfunction(L, GLMLuaBinding::quatToMat4);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Vec2 (self)
    int GLMLuaBinding::indexVec2(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "normalize") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeVec2);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Vec3 (self)
    int GLMLuaBinding::indexVec3(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "normalize") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeVec3);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Vec4 (self)
    int GLMLuaBinding::indexVec4(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "normalize") {
            lua_pushcfunction(L, GLMLuaBinding::normalizeVec4);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int GLMLuaBinding::addVec2(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec2 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec2(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec2(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (aIsVector && bIsVector) {
            result = vectorA + vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA + scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA + vectorB;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int GLMLuaBinding::addVec3(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec3 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec3(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec3(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (aIsVector && bIsVector) {
            result = vectorA + vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA + scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA + vectorB;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int GLMLuaBinding::addVec4(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec4 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec4(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec4(L, -1);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (aIsVector && bIsVector) {
            result = vectorA + vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA + scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA + vectorB;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int GLMLuaBinding::addMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA + matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int GLMLuaBinding::addMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA + matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int GLMLuaBinding::addMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA + matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int GLMLuaBinding::addQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);
        glm::quat result = quatA + quatB;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Axis vector3
    // - [-2] number    Angle (radians)
    int GLMLuaBinding::angleAxis(lua_State* L) {
        glm::vec3 axis = lua_tovec3(L, -1);
        float angle = (float) lua_tonumber(L, -2);
        glm::quat result = glm::angleAxis(angle, axis);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vector B
    // - [-2] table     Vector A
    int GLMLuaBinding::cross(lua_State* L) {
        glm::vec3 vectorA = lua_tovec3(L, -1);
        glm::vec3 vectorB = lua_tovec3(L, -2);
        glm::vec3 result = glm::cross(vectorA, vectorB);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int GLMLuaBinding::degrees(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::vec3 result = glm::degrees(vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int GLMLuaBinding::divideVec2(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec2 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec2(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec2(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (aIsVector && bIsVector) {
            result = vectorA / vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA / scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA / vectorB;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int GLMLuaBinding::divideVec3(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec3 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec3(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec3(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (aIsVector && bIsVector) {
            result = vectorA / vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA / scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA / vectorB;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int GLMLuaBinding::divideVec4(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec4 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec4(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec4(L, -1);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (aIsVector && bIsVector) {
            result = vectorA / vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA / scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA / vectorB;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int GLMLuaBinding::divideMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA / matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int GLMLuaBinding::divideMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA / matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int GLMLuaBinding::divideMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA / matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int GLMLuaBinding::divideQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);

        // The division operation quatA / quatB is equivalent to multiplying quatA by the inverse of quatB
        glm::quat result = quatA * glm::inverse(quatB);

        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 B
    // - [-2] table     Vec2 A
    int GLMLuaBinding::dotVec2(lua_State* L) {
        glm::vec2 vectorB = lua_tovec2(L, -1);
        glm::vec2 vectorA = lua_tovec2(L, -2);
        float result = glm::dot(vectorA, vectorB);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 B
    // - [-2] table     Vec3 A
    int GLMLuaBinding::dotVec3(lua_State* L) {
        glm::vec3 vectorB = lua_tovec3(L, -1);
        glm::vec3 vectorA = lua_tovec3(L, -2);
        float result = glm::dot(vectorA, vectorB);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 B
    // - [-2] table     Vec4 A
    int GLMLuaBinding::dotVec4(lua_State* L) {
        glm::vec4 vectorB = lua_tovec4(L, -1);
        glm::vec4 vectorA = lua_tovec4(L, -2);
        float result = glm::dot(vectorA, vectorB);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int GLMLuaBinding::eulerAngles(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::vec3 result = glm::eulerAngles(quaternion);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int GLMLuaBinding::inverseQuat(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::quat result = glm::inverse(quaternion);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat2
    int GLMLuaBinding::inverseMat2(lua_State* L) {
        glm::mat2 matrix = lua_tomat2(L, -1);
        glm::mat2 result = glm::inverse(matrix);
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat3
    int GLMLuaBinding::inverseMat3(lua_State* L) {
        glm::mat3 matrix = lua_tomat3(L, -1);
        glm::mat3 result = glm::inverse(matrix);
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat4
    int GLMLuaBinding::inverseMat4(lua_State* L) {
        glm::mat4 matrix = lua_tomat4(L, -1);
        glm::mat4 result = glm::inverse(matrix);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2
    int GLMLuaBinding::lengthVec2(lua_State* L) {
        glm::vec2 vector = lua_tovec2(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int GLMLuaBinding::lengthVec3(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4
    int GLMLuaBinding::lengthVec4(lua_State* L) {
        glm::vec4 vector = lua_tovec4(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    timestep
    // - [-2] table     End position vec3
    // - [-3] table     Start position vec3
    int GLMLuaBinding::lerp(lua_State* L) {
        float timestep = (float) lua_tonumber(L, -1);
        glm::vec3 end = lua_tovec3(L, -2);
        glm::vec3 start = lua_tovec3(L, -3);
        glm::vec3 result = glm::mix(start, end, timestep);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vec3
    // - [-2] table     Center vec3
    // - [-3] table     Eye vec3
    int GLMLuaBinding::lookAt(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 center = lua_tovec3(L, -2);
        glm::vec3 eye = lua_tovec3(L, -3);
        glm::mat4 result = glm::lookAt(eye, center, upDirection);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int GLMLuaBinding::mat2(lua_State* L) {
        float scalar = 0.0f;
        bool missing = lua_isnil(L, -1);
        if (!missing) {
            scalar = (float) lua_tonumber(L, -1);
        }
        glm::mat4 result(scalar);
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int GLMLuaBinding::mat3(lua_State* L) {
        float scalar = 0.0f;
        bool missing = lua_isnil(L, -1);
        if (!missing) {
            scalar = (float) lua_tonumber(L, -1);
        }
        glm::mat4 result(scalar);
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int GLMLuaBinding::mat4(lua_State* L) {
        float scalar = 0.0f;
        bool missing = lua_isnil(L, -1);
        if (!missing) {
            scalar = (float) lua_tonumber(L, -1);
        }
        glm::mat4 result(scalar);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Matrix 3x3
    int GLMLuaBinding::mat3ToQuat(lua_State* L) {
        glm::mat3 matrix = lua_tomat3(L, -1);
        glm::quat result = glm::toQuat(matrix);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Matrix 4x4
    int GLMLuaBinding::mat4ToQuat(lua_State* L) {
        glm::mat4 matrix = lua_tomat4(L, -1);
        glm::quat result = glm::toQuat(matrix);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int GLMLuaBinding::multiplyVec2(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec2 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec2(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec2(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (aIsVector && bIsVector) {
            result = vectorA * vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA * scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA * vectorB;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int GLMLuaBinding::multiplyVec3(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec3 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec3(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec3(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (aIsVector && bIsVector) {
            result = vectorA * vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA * scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA * vectorB;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int GLMLuaBinding::multiplyVec4(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec4 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec4(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec4(L, -1);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (aIsVector && bIsVector) {
            result = vectorA * vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA * scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA * vectorB;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int GLMLuaBinding::multiplyMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA * matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int GLMLuaBinding::multiplyMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA * matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int GLMLuaBinding::multiplyMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA * matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int GLMLuaBinding::multiplyQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);
        glm::quat result = quatA * quatB;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quaternion
    int GLMLuaBinding::normalizeQuat(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::quat result = glm::normalize(quaternion);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2
    int GLMLuaBinding::normalizeVec2(lua_State* L) {
        glm::vec2 vector = lua_tovec2(L, -1);
        glm::vec2 result = glm::normalize(vector);
        lua_pushvec2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int GLMLuaBinding::normalizeVec3(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::vec3 result = glm::normalize(vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4
    int GLMLuaBinding::normalizeVec4(lua_State* L) {
        glm::vec4 vector = lua_tovec4(L, -1);
        glm::vec4 result = glm::normalize(vector);
        lua_pushvec4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number     Z
    // - [-2] number     Y
    // - [-3] number     X
    // - [-4] number     W
    int GLMLuaBinding::quat(lua_State* L) {
        glm::quat result{};
        result.z = (float) lua_tonumber(L, -1);
        result.y = (float) lua_tonumber(L, -2);
        result.x = (float) lua_tonumber(L, -3);
        result.w = (float) lua_tonumber(L, -4);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int GLMLuaBinding::quatLookAt(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 forwardDirection = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAt(forwardDirection, upDirection);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int GLMLuaBinding::quatLookAtRH(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 forwardDirection = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAtRH(forwardDirection, upDirection);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int GLMLuaBinding::quatLookAtLH(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 forwardDirection = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAtLH(forwardDirection, upDirection);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int GLMLuaBinding::quatToMat4(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::mat4 result = glm::toMat4(quaternion);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle (degrees)
    int GLMLuaBinding::radians(lua_State* L) {
        float angle = (float) lua_tonumber(L, -1);
        float result = glm::radians(angle);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     vector
    // - [-2] table     quaternion
    int GLMLuaBinding::rotate(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::quat quaternion = lua_toquat(L, -2);
        glm::vec3 result = glm::rotate(quaternion, vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int GLMLuaBinding::rotateX(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateX(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int GLMLuaBinding::rotateY(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateY(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int GLMLuaBinding::rotateZ(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateZ(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Timestep
    // - [-2] table     End quaternion
    // - [-3] table     Start quaternion
    int GLMLuaBinding::slerp(lua_State* L) {
        float timestep = (float) lua_tonumber(L, -1);
        glm::quat end = lua_toquat(L, -2);
        glm::quat start = lua_toquat(L, -3);
        glm::quat result = glm::slerp(start, end, timestep);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int GLMLuaBinding::subtractVec2(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec2 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec2(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec2 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec2(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec2 result{};
        if (aIsVector && bIsVector) {
            result = vectorA - vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA - scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA - vectorB;
        }
        lua_pushvec2(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector3 B or scalar B
    // - [-2] table or number    Vector3 A or scalar A
    int GLMLuaBinding::subtractVec3(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec3 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec3(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec3 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec3(L, -2);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec3 result{};
        if (aIsVector && bIsVector) {
            result = vectorA - vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA - scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA - vectorB;
        }
        lua_pushvec3(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector4 B or scalar B
    // - [-2] table or number    Vector4 A or scalar A
    int GLMLuaBinding::subtractVec4(lua_State* L) {
        bool bIsVector = lua_istable(L, -1);
        bool bIsScalar = lua_isnumber(L, -1);

        bool aIsVector = lua_istable(L, -2);
        bool aIsScalar = lua_isnumber(L, -2);

        ST_ASSERT(bIsVector || bIsScalar, "bIsVector || bIsScalar");
        ST_ASSERT(aIsVector || aIsScalar, "aIsVector || aIsScalar");
        ST_ASSERT(bIsVector || aIsVector, "bIsVector || aIsVector");

        glm::vec4 vectorB{};
        if (bIsVector) {
            vectorB = lua_tovec4(L, -1);
        }
        float scalarB = 0.0f;
        if (bIsScalar) {
            scalarB = (float) lua_tonumber(L, -1);
        }

        glm::vec4 vectorA{};
        if (aIsVector) {
            vectorA = lua_tovec4(L, -1);
        }
        float scalarA = 0.0f;
        if (aIsScalar) {
            scalarA = (float) lua_tonumber(L, -2);
        }

        glm::vec4 result;
        if (aIsVector && bIsVector) {
            result = vectorA - vectorB;
        }
        if (aIsVector && bIsScalar) {
            result = vectorA - scalarB;
        }
        if (aIsScalar && bIsVector) {
            result = scalarA - vectorB;
        }
        lua_pushvec4(L, result);

        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 (self)
    int GLMLuaBinding::toStringVec2(lua_State* L) {
        glm::vec2 self = lua_tovec2(L, -1);
        lua_pushfstring(L, "x: %d, y: %d", self.x, self.y);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 (self)
    int GLMLuaBinding::toStringVec3(lua_State* L) {
        glm::vec3 self = lua_tovec3(L, -1);
        lua_pushfstring(L, "x: %d, y: %d, z: %d", self.x, self.y, self.z);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 (self)
    int GLMLuaBinding::toStringVec4(lua_State* L) {
        glm::vec4 self = lua_tovec4(L, -1);
        lua_pushfstring(L, "x: %d, y: %d, z: %d, w: %d", self.x, self.y, self.z, self.w);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int GLMLuaBinding::subtractMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA - matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int GLMLuaBinding::subtractMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA - matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int GLMLuaBinding::subtractMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA - matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int GLMLuaBinding::subtractQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);
        glm::quat result = quatA - quatB;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Vec3
    // - [-2] table    Mat4
    int GLMLuaBinding::translate(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::mat4 matrix = lua_tomat4(L, -2);
        glm::mat4 result = glm::translate(matrix, vector);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 (self)
    int GLMLuaBinding::unaryMinusVec2(lua_State* L) {
        glm::vec2 self = lua_tovec2(L, -1);
        lua_pushvec2(L, -self);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 (self)
    int GLMLuaBinding::unaryMinusVec3(lua_State* L) {
        glm::vec3 self = lua_tovec3(L, -1);
        lua_pushvec3(L, -self);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 (self)
    int GLMLuaBinding::unaryMinusVec4(lua_State* L) {
        glm::vec4 self = lua_tovec4(L, -1);
        lua_pushvec4(L, -self);
        return 1;
    }

    // Lua stack
    //
    // glm.vec()
    // - Empty
    //
    // glm.vec({ x, y })
    // - [-1] table
    //
    // glm.vec(x, y)
    // - [-1] number    Y
    // - [-2] number    X
    int GLMLuaBinding::vec2(lua_State* L) {
        glm::vec2 vector{};
        uint32_t argumentCount = lua_gettop(L);
        if (argumentCount == 1) {
            vector = lua_tovec2(L, -1);
        }
        if (argumentCount == 2) {
            vector.y = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.x = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pushvec2(L, vector);
        return 1;
    }

    // Lua stack
    //
    // glm.vec()
    // - Empty
    //
    // glm.vec({ x, y, z })
    // - [-1] table
    //
    // glm.vec(x, y, z)
    // - [-1] number    Z
    // - [-2] number    Y
    // - [-3] number    X
    int GLMLuaBinding::vec3(lua_State* L) {
        glm::vec3 vector{0, 0, 0};
        uint32_t argumentCount = lua_gettop(L);
        if (argumentCount == 1) {
            vector = lua_tovec3(L, -1);
        }
        if (argumentCount == 3) {
            vector.z = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.y = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.x = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pushvec3(L, vector);
        return 1;
    }

    // Lua stack
    //
    // glm.vec()
    // - Empty
    //
    // glm.vec({ x, y, z, w })
    // - [-1] table
    //
    // glm.vec(x, y, z, w)
    // - [-1] number    W
    // - [-2] number    Z
    // - [-3] number    Y
    // - [-4] number    X
    int GLMLuaBinding::vec4(lua_State* L) {
        glm::vec4 vector{};
        uint32_t argumentCount = lua_gettop(L);
        if (argumentCount == 1) {
            vector = lua_tovec4(L, -1);
        }
        if (argumentCount == 4) {
            vector.w = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.z = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.y = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);

            vector.x = (float) lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pushvec4(L, vector);
        return 1;
    }
}

glm::vec2 lua_tovec2(lua_State* L, int index) {
    glm::vec2 vector{};

    lua_getfield(L, index, "x");
    vector.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vector.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vector;
}

void lua_pushvec2(lua_State* L, const glm::vec2& vector) {
    lua_newtable(L);

    lua_pushnumber(L, vector.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vector.y);
    lua_setfield(L, -2, "y");

    luaL_getmetatable(L, Storytime::GLMLuaBinding::vec2_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::vec3 lua_tovec3(lua_State* L, int index) {
    glm::vec3 vector{};

    lua_getfield(L, index, "x");
    vector.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vector.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    vector.z = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vector;
}

void lua_pushvec3(lua_State* L, const glm::vec3& vector) {
    lua_newtable(L);

    lua_pushnumber(L, vector.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vector.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, vector.z);
    lua_setfield(L, -2, "z");

    luaL_getmetatable(L, Storytime::GLMLuaBinding::vec3_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::vec4 lua_tovec4(lua_State* L, int index) {
    glm::vec4 vector{};

    lua_getfield(L, index, "x");
    vector.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    vector.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    vector.z = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "w");
    vector.w = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vector;
}

void lua_pushvec4(lua_State* L, const glm::vec4& vector) {
    lua_newtable(L);

    lua_pushnumber(L, vector.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, vector.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, vector.z);
    lua_setfield(L, -2, "z");

    lua_pushnumber(L, vector.w);
    lua_setfield(L, -2, "w");

    luaL_getmetatable(L, Storytime::GLMLuaBinding::vec4_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::mat2 lua_tomat2(lua_State* L, int index) {
    glm::mat2 matrix{};
    for (int i = 0; i < 2; ++i) {
        lua_geti(L, index, i + 1); // Lua uses 1-based indexing

        lua_getfield(L, -1, "x");
        matrix[i].x = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        matrix[i].y = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    return matrix;
}

void lua_pushmat2(lua_State* L, const glm::mat2& matrix) {
    lua_newtable(L);
    for (uint8_t i = 0; i < 2; ++i) {
        lua_newtable(L);
        lua_pushnumber(L, matrix[i].x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, matrix[i].y);
        lua_setfield(L, -2, "y");
        luaL_getmetatable(L, Storytime::GLMLuaBinding::vec2_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::GLMLuaBinding::mat2_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::mat3 lua_tomat3(lua_State* L, int index) {
    glm::mat3 matrix{};
    for (int i = 0; i < 3; ++i) {
        lua_geti(L, index, i + 1); // Lua uses 1-based indexing

        lua_getfield(L, -1, "x");
        matrix[i].x = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        matrix[i].y = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "z");
        matrix[i].z = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    return matrix;
}

void lua_pushmat3(lua_State* L, const glm::mat3& matrix) {
    lua_newtable(L);
    for (uint8_t i = 0; i < 3; ++i) {
        lua_newtable(L);
        lua_pushnumber(L, matrix[i].x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, matrix[i].y);
        lua_setfield(L, -2, "y");
        lua_pushnumber(L, matrix[i].z);
        lua_setfield(L, -2, "z");
        luaL_getmetatable(L, Storytime::GLMLuaBinding::vec3_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::GLMLuaBinding::mat3_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::mat4 lua_tomat4(lua_State* L, int index) {
    glm::mat4 matrix{};
    for (int i = 0; i < 4; ++i) {
        lua_geti(L, index, i + 1); // Lua uses 1-based indexing

        lua_getfield(L, -1, "x");
        matrix[i].x = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        matrix[i].y = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "z");
        matrix[i].z = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "w");
        matrix[i].w = (float) lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pop(L, 1);
    }
    return matrix;
}

void lua_pushmat4(lua_State* L, const glm::mat4& matrix) {
    lua_newtable(L);
    for (uint8_t i = 0; i < 4; ++i) {
        lua_newtable(L);
        lua_pushnumber(L, matrix[i].x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, matrix[i].y);
        lua_setfield(L, -2, "y");
        lua_pushnumber(L, matrix[i].z);
        lua_setfield(L, -2, "z");
        lua_pushnumber(L, matrix[i].w);
        lua_setfield(L, -2, "w");
        luaL_getmetatable(L, Storytime::GLMLuaBinding::vec4_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::GLMLuaBinding::mat4_metatable_name.c_str());
    lua_setmetatable(L, -2);
}

glm::quat lua_toquat(lua_State* L, int index) {
    glm::quat quaternion{};

    lua_getfield(L, index, "x");
    quaternion.x = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    quaternion.y = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    quaternion.z = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "w");
    quaternion.w = (float) lua_tonumber(L, -1);
    lua_pop(L, 1);

    return quaternion;
}

void lua_pushquat(lua_State* L, const glm::quat& quaternion) {
    lua_newtable(L);
    lua_pushnumber(L, quaternion.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, quaternion.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, quaternion.z);
    lua_setfield(L, -2, "z");
    lua_pushnumber(L, quaternion.w);
    lua_setfield(L, -2, "w");
    luaL_getmetatable(L, Storytime::GLMLuaBinding::quat_metatable_name.c_str());
    lua_setmetatable(L, -2);
}
