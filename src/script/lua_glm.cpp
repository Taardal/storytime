#include "script/lua_glm.h"

namespace Storytime {
    const std::string LuaGLM::table_name = "glm";
    const std::string LuaGLM::metatable_name = table_name + ".meta";
    const std::string LuaGLM::vec2_metatable_name = table_name + ".vec2.meta";
    const std::string LuaGLM::vec3_metatable_name = table_name + ".vec3.meta";
    const std::string LuaGLM::vec4_metatable_name = table_name + ".vec4.meta";
    const std::string LuaGLM::mat2_metatable_name = table_name + ".mat2.meta";
    const std::string LuaGLM::mat3_metatable_name = table_name + ".mat3.meta";
    const std::string LuaGLM::mat4_metatable_name = table_name + ".mat4.meta";
    const std::string LuaGLM::quat_metatable_name = table_name + ".quat.meta";

    int LuaGLM::create_metatable(lua_State* L) {
        createVec2Metatable(L);
        lua_pop(L, 1);
        createVec3Metatable(L);
        lua_pop(L, 1);
        createVec4Metatable(L);
        lua_pop(L, 1);
        createMat2Metatable(L);
        lua_pop(L, 1);
        createMat3Metatable(L);
        lua_pop(L, 1);
        createMat4Metatable(L);
        lua_pop(L, 1);
        createQuatMetatable(L);
        lua_pop(L, 1);

        luaL_newmetatable(L, metatable_name.c_str());
        lua_pushstring(L, "__index");
        lua_pushcfunction(L, index);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::create(lua_State* L) {
        lua_newtable(L);

        luaL_getmetatable(L, metatable_name.c_str());
        if (lua_isnil(L, -1)) {
            lua_pop(L, 1);
            ST_LUA_ASSERT_CREATED(create_metatable(L), LUA_TTABLE);
        }
        lua_setmetatable(L, -2);

        return 1;
    }

    int LuaGLM::createVec2Metatable(lua_State* L) {
        luaL_newmetatable(L, vec2_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, LuaGLM::indexVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__len");
        lua_pushcfunction(L, LuaGLM::lengthVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__unm");
        lua_pushcfunction(L, LuaGLM::unaryMinusVec2);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, LuaGLM::toStringVec2);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::createVec3Metatable(lua_State* L) {
        luaL_newmetatable(L, vec3_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, LuaGLM::indexVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__len");
        lua_pushcfunction(L, LuaGLM::lengthVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__unm");
        lua_pushcfunction(L, LuaGLM::unaryMinusVec3);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, LuaGLM::toStringVec3);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::createVec4Metatable(lua_State* L) {
        luaL_newmetatable(L, vec4_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, LuaGLM::indexVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__len");
        lua_pushcfunction(L, LuaGLM::lengthVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__unm");
        lua_pushcfunction(L, LuaGLM::unaryMinusVec4);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcfunction(L, LuaGLM::toStringVec4);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::createMat2Metatable(lua_State* L) {
        luaL_newmetatable(L, mat2_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addMat2);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideMat2);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyMat2);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractMat2);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::createMat3Metatable(lua_State* L) {
        luaL_newmetatable(L, mat3_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, LuaGLM::indexMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyMat3);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractMat3);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::createMat4Metatable(lua_State* L) {
        luaL_newmetatable(L, mat4_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, LuaGLM::indexMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyMat4);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractMat4);
        lua_settable(L, -3);

        return 1;
    }

    int LuaGLM::createQuatMetatable(lua_State* L) {
        luaL_newmetatable(L, quat_metatable_name.c_str());

        lua_pushstring(L, "__add");
        lua_pushcfunction(L, LuaGLM::addQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__div");
        lua_pushcfunction(L, LuaGLM::divideQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__index");
        lua_pushcfunction(L, LuaGLM::indexQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__mul");
        lua_pushcfunction(L, LuaGLM::multiplyQuat);
        lua_settable(L, -3);

        lua_pushstring(L, "__sub");
        lua_pushcfunction(L, LuaGLM::subtractQuat);
        lua_settable(L, -3);

        return 1;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] userdata  Binding
    int LuaGLM::index(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "addMat2") {
            lua_pushcfunction(L, LuaGLM::addMat2);
            return 1;
        }
        if (indexName == "addMat3") {
            lua_pushcfunction(L, LuaGLM::addMat3);
            return 1;
        }
        if (indexName == "addMat4") {
            lua_pushcfunction(L, LuaGLM::addMat4);
            return 1;
        }
        if (indexName == "addQuat") {
            lua_pushcfunction(L, LuaGLM::addQuat);
            return 1;
        }
        if (indexName == "addVec2") {
            lua_pushcfunction(L, LuaGLM::addVec2);
            return 1;
        }
        if (indexName == "addVec3") {
            lua_pushcfunction(L, LuaGLM::addVec3);
            return 1;
        }
        if (indexName == "addVec4") {
            lua_pushcfunction(L, LuaGLM::addVec4);
            return 1;
        }
        if (indexName == "angleAxis") {
            lua_pushcfunction(L, LuaGLM::angleAxis);
            return 1;
        }
        if (indexName == "cross") {
            lua_pushcfunction(L, LuaGLM::cross);
            return 1;
        }
        if (indexName == "degrees") {
            lua_pushcfunction(L, LuaGLM::degrees);
            return 1;
        }
        if (indexName == "divideMat2") {
            lua_pushcfunction(L, LuaGLM::divideMat2);
            return 1;
        }
        if (indexName == "divideMat3") {
            lua_pushcfunction(L, LuaGLM::divideMat3);
            return 1;
        }
        if (indexName == "divideMat4") {
            lua_pushcfunction(L, LuaGLM::divideMat4);
            return 1;
        }
        if (indexName == "divideQuat") {
            lua_pushcfunction(L, LuaGLM::divideQuat);
            return 1;
        }
        if (indexName == "divideVec2") {
            lua_pushcfunction(L, LuaGLM::divideVec2);
            return 1;
        }
        if (indexName == "divideVec3") {
            lua_pushcfunction(L, LuaGLM::divideVec3);
            return 1;
        }
        if (indexName == "divideVec4") {
            lua_pushcfunction(L, LuaGLM::divideVec4);
            return 1;
        }
        if (indexName == "dotVec2") {
            lua_pushcfunction(L, LuaGLM::dotVec2);
            return 1;
        }
        if (indexName == "dotVec3") {
            lua_pushcfunction(L, LuaGLM::dotVec3);
            return 1;
        }
        if (indexName == "dotVec4") {
            lua_pushcfunction(L, LuaGLM::dotVec4);
            return 1;
        }
        if (indexName == "eulerAngles") {
            lua_pushcfunction(L, LuaGLM::eulerAngles);
            return 1;
        }
        if (indexName == "inverseQuat") {
            lua_pushcfunction(L, LuaGLM::inverseQuat);
            return 1;
        }
        if (indexName == "inverseMat2") {
            lua_pushcfunction(L, LuaGLM::inverseMat2);
            return 1;
        }
        if (indexName == "inverseMat3") {
            lua_pushcfunction(L, LuaGLM::inverseMat3);
            return 1;
        }
        if (indexName == "inverseMat4") {
            lua_pushcfunction(L, LuaGLM::inverseMat4);
            return 1;
        }
        if (indexName == "lengthVec2") {
            lua_pushcfunction(L, LuaGLM::lengthVec2);
            return 1;
        }
        if (indexName == "lengthVec3") {
            lua_pushcfunction(L, LuaGLM::lengthVec3);
            return 1;
        }
        if (indexName == "lengthVec4") {
            lua_pushcfunction(L, LuaGLM::lengthVec4);
            return 1;
        }
        if (indexName == "lerp") {
            lua_pushcfunction(L, LuaGLM::lerp);
            return 1;
        }
        if (indexName == "lookAt") {
            lua_pushcfunction(L, LuaGLM::lookAt);
            return 1;
        }
        if (indexName == "mat2") {
            lua_pushcfunction(L, LuaGLM::mat2);
            return 1;
        }
        if (indexName == "mat3") {
            lua_pushcfunction(L, LuaGLM::mat3);
            return 1;
        }
        if (indexName == "mat4") {
            lua_pushcfunction(L, LuaGLM::mat4);
            return 1;
        }
        if (indexName == "mat3ToQuat") {
            lua_pushcfunction(L, LuaGLM::mat3ToQuat);
            return 1;
        }
        if (indexName == "mat4ToQuat") {
            lua_pushcfunction(L, LuaGLM::mat4ToQuat);
            return 1;
        }
        if (indexName == "multiplyMat2") {
            lua_pushcfunction(L, LuaGLM::multiplyMat2);
            return 1;
        }
        if (indexName == "multiplyMat3") {
            lua_pushcfunction(L, LuaGLM::multiplyMat3);
            return 1;
        }
        if (indexName == "multiplyMat4") {
            lua_pushcfunction(L, LuaGLM::multiplyMat4);
            return 1;
        }
        if (indexName == "multiplyQuat") {
            lua_pushcfunction(L, LuaGLM::multiplyQuat);
            return 1;
        }
        if (indexName == "multiplyVec2") {
            lua_pushcfunction(L, LuaGLM::multiplyVec2);
            return 1;
        }
        if (indexName == "multiplyVec3") {
            lua_pushcfunction(L, LuaGLM::multiplyVec3);
            return 1;
        }
        if (indexName == "multiplyVec4") {
            lua_pushcfunction(L, LuaGLM::multiplyVec4);
            return 1;
        }
        if (indexName == "normalizeVec2") {
            lua_pushcfunction(L, LuaGLM::normalizeVec2);
            return 1;
        }
        if (indexName == "normalizeVec3") {
            lua_pushcfunction(L, LuaGLM::normalizeVec3);
            return 1;
        }
        if (indexName == "normalizeVec4") {
            lua_pushcfunction(L, LuaGLM::normalizeVec4);
            return 1;
        }
        if (indexName == "normalizeQuat") {
            lua_pushcfunction(L, LuaGLM::normalizeQuat);
            return 1;
        }
        if (indexName == "quat") {
            lua_pushcfunction(L, LuaGLM::quat);
            return 1;
        }
        if (indexName == "quatLookAt") {
            lua_pushcfunction(L, LuaGLM::quatLookAt);
            return 1;
        }
        if (indexName == "quatLookAtRH") {
            lua_pushcfunction(L, LuaGLM::quatLookAtRH);
            return 1;
        }
        if (indexName == "quatLookAtLH") {
            lua_pushcfunction(L, LuaGLM::quatLookAtLH);
            return 1;
        }
        if (indexName == "quatToMat4") {
            lua_pushcfunction(L, LuaGLM::quatToMat4);
            return 1;
        }
        if (indexName == "radians") {
            lua_pushcfunction(L, LuaGLM::radians);
            return 1;
        }
        if (indexName == "rotate") {
            lua_pushcfunction(L, LuaGLM::rotate);
            return 1;
        }
        if (indexName == "rotateX") {
            lua_pushcfunction(L, LuaGLM::rotateX);
            return 1;
        }
        if (indexName == "rotateY") {
            lua_pushcfunction(L, LuaGLM::rotateY);
            return 1;
        }
        if (indexName == "rotateZ") {
            lua_pushcfunction(L, LuaGLM::rotateZ);
            return 1;
        }
        if (indexName == "slerp") {
            lua_pushcfunction(L, LuaGLM::slerp);
            return 1;
        }
        if (indexName == "subtractMat2") {
            lua_pushcfunction(L, LuaGLM::subtractMat2);
            return 1;
        }
        if (indexName == "subtractMat3") {
            lua_pushcfunction(L, LuaGLM::subtractMat3);
            return 1;
        }
        if (indexName == "subtractMat4") {
            lua_pushcfunction(L, LuaGLM::subtractMat4);
            return 1;
        }
        if (indexName == "subtractQuat") {
            lua_pushcfunction(L, LuaGLM::subtractQuat);
            return 1;
        }
        if (indexName == "subtractVec2") {
            lua_pushcfunction(L, LuaGLM::subtractVec2);
            return 1;
        }
        if (indexName == "subtractVec3") {
            lua_pushcfunction(L, LuaGLM::subtractVec3);
            return 1;
        }
        if (indexName == "subtractVec4") {
            lua_pushcfunction(L, LuaGLM::subtractVec4);
            return 1;
        }
        if (indexName == "translate") {
            lua_pushcfunction(L, LuaGLM::translate);
            return 1;
        }
        if (indexName == "vec2") {
            lua_pushcfunction(L, LuaGLM::vec2);
            return 1;
        }
        if (indexName == "vec3") {
            lua_pushcfunction(L, LuaGLM::vec3);
            return 1;
        }
        if (indexName == "vec4") {
            lua_pushcfunction(L, LuaGLM::vec4);
            return 1;
        }
        ST_LOG_WARNING("Could not resolve index [{}]", indexName);
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Mat3 (self)
    int LuaGLM::indexMat3(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "toQuat") {
            lua_pushcfunction(L, LuaGLM::mat3ToQuat);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Mat4 (self)
    int LuaGLM::indexMat4(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "toQuat") {
            lua_pushcfunction(L, LuaGLM::mat4ToQuat);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Quaternion (self)
    int LuaGLM::indexQuat(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "inverse") {
            lua_pushcfunction(L, LuaGLM::inverseQuat);
            return 1;
        }
        if (indexName == "normalize") {
            lua_pushcfunction(L, LuaGLM::normalizeQuat);
            return 1;
        }
        if (indexName == "toMat4") {
            lua_pushcfunction(L, LuaGLM::quatToMat4);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Vec2 (self)
    int LuaGLM::indexVec2(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "normalize") {
            lua_pushcfunction(L, LuaGLM::normalizeVec2);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Vec3 (self)
    int LuaGLM::indexVec3(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "normalize") {
            lua_pushcfunction(L, LuaGLM::normalizeVec3);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] string    Name of the index being accessed
    // - [-2] table     Vec4 (self)
    int LuaGLM::indexVec4(lua_State* L) {
        std::string indexName = lua_tostring(L, -1);
        if (indexName == "normalize") {
            lua_pushcfunction(L, LuaGLM::normalizeVec4);
            return 1;
        }
        return 0;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLM::addVec2(lua_State* L) {
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
    int LuaGLM::addVec3(lua_State* L) {
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
    int LuaGLM::addVec4(lua_State* L) {
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
    int LuaGLM::addMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA + matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLM::addMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA + matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLM::addMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA + matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLM::addQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);
        glm::quat result = quatA + quatB;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Axis vector3
    // - [-2] number    Angle (radians)
    int LuaGLM::angleAxis(lua_State* L) {
        glm::vec3 axis = lua_tovec3(L, -1);
        float angle = (float) lua_tonumber(L, -2);
        glm::quat result = glm::angleAxis(angle, axis);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vector B
    // - [-2] table     Vector A
    int LuaGLM::cross(lua_State* L) {
        glm::vec3 vectorA = lua_tovec3(L, -1);
        glm::vec3 vectorB = lua_tovec3(L, -2);
        glm::vec3 result = glm::cross(vectorA, vectorB);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int LuaGLM::degrees(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::vec3 result = glm::degrees(vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLM::divideVec2(lua_State* L) {
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
    int LuaGLM::divideVec3(lua_State* L) {
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
    int LuaGLM::divideVec4(lua_State* L) {
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
    int LuaGLM::divideMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA / matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLM::divideMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA / matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLM::divideMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA / matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLM::divideQuat(lua_State* L) {
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
    int LuaGLM::dotVec2(lua_State* L) {
        glm::vec2 vectorB = lua_tovec2(L, -1);
        glm::vec2 vectorA = lua_tovec2(L, -2);
        float result = glm::dot(vectorA, vectorB);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 B
    // - [-2] table     Vec3 A
    int LuaGLM::dotVec3(lua_State* L) {
        glm::vec3 vectorB = lua_tovec3(L, -1);
        glm::vec3 vectorA = lua_tovec3(L, -2);
        float result = glm::dot(vectorA, vectorB);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 B
    // - [-2] table     Vec4 A
    int LuaGLM::dotVec4(lua_State* L) {
        glm::vec4 vectorB = lua_tovec4(L, -1);
        glm::vec4 vectorA = lua_tovec4(L, -2);
        float result = glm::dot(vectorA, vectorB);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int LuaGLM::eulerAngles(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::vec3 result = glm::eulerAngles(quaternion);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int LuaGLM::inverseQuat(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::quat result = glm::inverse(quaternion);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat2
    int LuaGLM::inverseMat2(lua_State* L) {
        glm::mat2 matrix = lua_tomat2(L, -1);
        glm::mat2 result = glm::inverse(matrix);
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat3
    int LuaGLM::inverseMat3(lua_State* L) {
        glm::mat3 matrix = lua_tomat3(L, -1);
        glm::mat3 result = glm::inverse(matrix);
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Mat4
    int LuaGLM::inverseMat4(lua_State* L) {
        glm::mat4 matrix = lua_tomat4(L, -1);
        glm::mat4 result = glm::inverse(matrix);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2
    int LuaGLM::lengthVec2(lua_State* L) {
        glm::vec2 vector = lua_tovec2(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int LuaGLM::lengthVec3(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4
    int LuaGLM::lengthVec4(lua_State* L) {
        glm::vec4 vector = lua_tovec4(L, -1);
        float result = glm::length(vector);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    timestep
    // - [-2] table     End position vec3
    // - [-3] table     Start position vec3
    int LuaGLM::lerp(lua_State* L) {
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
    int LuaGLM::lookAt(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 center = lua_tovec3(L, -2);
        glm::vec3 eye = lua_tovec3(L, -3);
        glm::mat4 result = glm::lookAt(eye, center, upDirection);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number   Scalar
    int LuaGLM::mat2(lua_State* L) {
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
    int LuaGLM::mat3(lua_State* L) {
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
    int LuaGLM::mat4(lua_State* L) {
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
    int LuaGLM::mat3ToQuat(lua_State* L) {
        glm::mat3 matrix = lua_tomat3(L, -1);
        glm::quat result = glm::toQuat(matrix);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Matrix 4x4
    int LuaGLM::mat4ToQuat(lua_State* L) {
        glm::mat4 matrix = lua_tomat4(L, -1);
        glm::quat result = glm::toQuat(matrix);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table or number    Vector2 B or scalar B
    // - [-2] table or number    Vector2 A or scalar A
    int LuaGLM::multiplyVec2(lua_State* L) {
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
    int LuaGLM::multiplyVec3(lua_State* L) {
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
    int LuaGLM::multiplyVec4(lua_State* L) {
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
    int LuaGLM::multiplyMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA * matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLM::multiplyMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA * matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLM::multiplyMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA * matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLM::multiplyQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);
        glm::quat result = quatA * quatB;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quaternion
    int LuaGLM::normalizeQuat(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::quat result = glm::normalize(quaternion);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2
    int LuaGLM::normalizeVec2(lua_State* L) {
        glm::vec2 vector = lua_tovec2(L, -1);
        glm::vec2 result = glm::normalize(vector);
        lua_pushvec2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3
    int LuaGLM::normalizeVec3(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::vec3 result = glm::normalize(vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4
    int LuaGLM::normalizeVec4(lua_State* L) {
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
    int LuaGLM::quat(lua_State* L) {
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
    int LuaGLM::quatLookAt(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 forwardDirection = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAt(forwardDirection, upDirection);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int LuaGLM::quatLookAtRH(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 forwardDirection = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAtRH(forwardDirection, upDirection);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Up direction vector3
    // - [-2] table     Forward direction vector3
    int LuaGLM::quatLookAtLH(lua_State* L) {
        glm::vec3 upDirection = lua_tovec3(L, -1);
        glm::vec3 forwardDirection = lua_tovec3(L, -2);
        glm::quat result = glm::quatLookAtLH(forwardDirection, upDirection);
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Quaternion
    int LuaGLM::quatToMat4(lua_State* L) {
        glm::quat quaternion = lua_toquat(L, -1);
        glm::mat4 result = glm::toMat4(quaternion);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle (degrees)
    int LuaGLM::radians(lua_State* L) {
        float angle = (float) lua_tonumber(L, -1);
        float result = glm::radians(angle);
        lua_pushnumber(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     vector
    // - [-2] table     quaternion
    int LuaGLM::rotate(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::quat quaternion = lua_toquat(L, -2);
        glm::vec3 result = glm::rotate(quaternion, vector);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int LuaGLM::rotateX(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateX(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int LuaGLM::rotateY(lua_State* L) {
        auto angle = (float) lua_tonumber(L, -1);
        glm::vec3 vector = lua_tovec3(L, -2);
        glm::vec3 result = glm::rotateY(vector, angle);
        lua_pushvec3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] number    Angle
    // - [-2] table     Vector3
    int LuaGLM::rotateZ(lua_State* L) {
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
    int LuaGLM::slerp(lua_State* L) {
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
    int LuaGLM::subtractVec2(lua_State* L) {
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
    int LuaGLM::subtractVec3(lua_State* L) {
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
    int LuaGLM::subtractVec4(lua_State* L) {
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
    int LuaGLM::toStringVec2(lua_State* L) {
        glm::vec2 self = lua_tovec2(L, -1);
        lua_pushfstring(L, "x: %d, y: %d", self.x, self.y);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 (self)
    int LuaGLM::toStringVec3(lua_State* L) {
        glm::vec3 self = lua_tovec3(L, -1);
        lua_pushfstring(L, "x: %d, y: %d, z: %d", self.x, self.y, self.z);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 (self)
    int LuaGLM::toStringVec4(lua_State* L) {
        glm::vec4 self = lua_tovec4(L, -1);
        lua_pushfstring(L, "x: %d, y: %d, z: %d, w: %d", self.x, self.y, self.z, self.w);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat2 B
    // - [-2] table     Mat2 A
    int LuaGLM::subtractMat2(lua_State* L) {
        glm::mat2 matrixB = lua_tomat2(L, -1);
        glm::mat2 matrixA = lua_tomat2(L, -2);
        glm::mat2 result = matrixA - matrixB;
        lua_pushmat2(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat3 B
    // - [-2] table     Mat3 A
    int LuaGLM::subtractMat3(lua_State* L) {
        glm::mat3 matrixB = lua_tomat3(L, -1);
        glm::mat3 matrixA = lua_tomat3(L, -2);
        glm::mat3 result = matrixA - matrixB;
        lua_pushmat3(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Mat4 B
    // - [-2] table     Mat4 A
    int LuaGLM::subtractMat4(lua_State* L) {
        glm::mat4 matrixB = lua_tomat4(L, -1);
        glm::mat4 matrixA = lua_tomat4(L, -2);
        glm::mat4 result = matrixA - matrixB;
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Quat B
    // - [-2] table     Quat A
    int LuaGLM::subtractQuat(lua_State* L) {
        glm::quat quatB = lua_toquat(L, -1);
        glm::quat quatA = lua_toquat(L, -2);
        glm::quat result = quatA - quatB;
        lua_pushquat(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table    Vec3
    // - [-2] table    Mat4
    int LuaGLM::translate(lua_State* L) {
        glm::vec3 vector = lua_tovec3(L, -1);
        glm::mat4 matrix = lua_tomat4(L, -2);
        glm::mat4 result = glm::translate(matrix, vector);
        lua_pushmat4(L, result);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec2 (self)
    int LuaGLM::unaryMinusVec2(lua_State* L) {
        glm::vec2 self = lua_tovec2(L, -1);
        lua_pushvec2(L, -self);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec3 (self)
    int LuaGLM::unaryMinusVec3(lua_State* L) {
        glm::vec3 self = lua_tovec3(L, -1);
        lua_pushvec3(L, -self);
        return 1;
    }

    // Lua stack
    // - [-1] table     Vec4 (self)
    int LuaGLM::unaryMinusVec4(lua_State* L) {
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
    int LuaGLM::vec2(lua_State* L) {
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
    int LuaGLM::vec3(lua_State* L) {
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
    int LuaGLM::vec4(lua_State* L) {
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

    luaL_getmetatable(L, Storytime::LuaGLM::vec2_metatable_name.c_str());
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

    luaL_getmetatable(L, Storytime::LuaGLM::vec3_metatable_name.c_str());
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

    luaL_getmetatable(L, Storytime::LuaGLM::vec4_metatable_name.c_str());
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
        luaL_getmetatable(L, Storytime::LuaGLM::vec2_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::LuaGLM::mat2_metatable_name.c_str());
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
        luaL_getmetatable(L, Storytime::LuaGLM::vec3_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::LuaGLM::mat3_metatable_name.c_str());
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
        luaL_getmetatable(L, Storytime::LuaGLM::vec4_metatable_name.c_str());
        lua_setmetatable(L, -2);
        lua_seti(L, -2, i + 1); // Lua uses 1-based indexing
    }
    luaL_getmetatable(L, Storytime::LuaGLM::mat4_metatable_name.c_str());
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
    luaL_getmetatable(L, Storytime::LuaGLM::quat_metatable_name.c_str());
    lua_setmetatable(L, -2);
}
