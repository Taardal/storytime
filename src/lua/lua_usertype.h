#pragma once

//
// Expose C++ types to Lua
//
// Problem:
// Given a class or struct defined in C++, we want to expose it to Lua as a table. We also want to be able to read and
// write any field from Lua code. We want to be able to regster any class/struct with any number of fields of any type
// using the same function.
//
// Solution:
// Create a Lua metatable to represent the class/struct with a `new` function to create new instances of the
// class/struct as userdatums. The metatable will have `__index` and `__newindex` functions capable of reading and
// writing any field of the userdatum.
//
// We use member pointers to facilitate this by storing each member pointer as an upvalue to the `__index` and
// `__newindex` functions of the metatable, and then invoke them on the userdatum. However, this requires knowledge of
// the type of the class/struct as well as the type of each member pointer. We solve this by storing the member
// pointers in a templated struct. This struct extends a non-templated struct which allows us to use type-erasure to
// store the struct instances in the same map while preserving the type-information.
//

namespace Storytime {
    // Non-templated type for upvalues
    struct Upvalue {
        virtual ~Upvalue() = default;

        virtual void push_to_lua(lua_State* L, void* userdata) const = 0;

        virtual void parse_from_lua(lua_State* L, void* userdata) const = 0;
    };

    // Templated type for upvalues to store member pointers with their type information
    template<class T, class E>
    struct Upvalue_T final : Upvalue {
        E T::* value;

        explicit Upvalue_T(E T::* value) : value(value) {
        }

        // Read the field of the userdatum object using the member pointer, and push the value to the Lua stack as its
        // expected type
        void push_to_lua(lua_State* L, void* userdata) const override {
            assert(userdata != nullptr);
            T* userdata_T = static_cast<T*>(userdata);
            lua_push<E>(L, userdata_T->*value);
        }

        // Write the field of the userdatum object using the member pointer, and parse the value from the Lua stack as its
        // expected type
        void parse_from_lua(lua_State* L, void* userdata) const override {
            assert(userdata != nullptr);
            T* userdata_T = static_cast<T*>(userdata);
            userdata_T->*value = lua_to<E>(L, -1);
        }
    };

    using UpvalueMap = std::map<std::string, Unique<Upvalue>>;

    class LuaUsertype {
    private:
        lua_State* L;
        std::string name;
        UpvalueMap upvalues;

    public:
        LuaUsertype(lua_State* L) : L(L) {
        }

        // Create a metatable to represent the global type
        template<class T>
        LuaUsertype& register_global(const std::string& name) {
            this->name = name;
            luaL_newmetatable(L, name.c_str());
            {
                // Add a function to create new instances of the type
                lua_pushstring(L, name.c_str());
                constexpr int upvalue_count = 1;
                lua_pushcclosure(L, lua_new<T>, upvalue_count);
                lua_setfield(L, -2, "new");
            }
            {
                // Add a function to read fields of the type
                lua_pushlightuserdata(L, &upvalues);
                constexpr int upvalue_count = 1;
                lua_pushcclosure(L, lua_index<T>, upvalue_count);
                lua_setfield(L, -2, "__index");
            }
            {
                // Add a function to write fields of the type
                lua_pushlightuserdata(L, &upvalues);
                constexpr int upvalue_count = 1;
                lua_pushcclosure(L, lua_newindex<T>, upvalue_count);
                lua_setfield(L, -2, "__newindex");
            }
            lua_setglobal(L, name.c_str());
            return *this;
        }

        template<class T, class E>
        LuaUsertype& with_field(const std::string& key, E T::* value) {
            upvalues.emplace(key, std::make_unique<Upvalue_T<T, E>>(value));
            return *this;
        }

    private:
        template<class T>
        static int lua_new(lua_State* L) {
            // Get the upvalue for the name of the type
            auto name = lua_tostring(L, lua_upvalueindex(1));
            assert(name != nullptr);

            // Create the userdatum to represent the instance of the type
            auto userdata = static_cast<T*>(lua_newuserdata(L, sizeof(T)));

            // Attach the global metatable for the type to the userdatum to facilitate the `__index` and `__newindex`
            // functions. This also makes it so that you can call `new` on the instance which is not ideal, but acceptable.
            luaL_getmetatable(L, name);
            lua_setmetatable(L, -2);

            // Return the userdatum to Lua
            assert(lua_gettop(L) == 1);
            assert(lua_isuserdata(L, -1));
            return 1;
        }

        template<class T>
        static int lua_index(lua_State* L) {
            // Get the name of the field to read from
            auto key = lua_tostring(L, -1);
            assert(key != nullptr);

            // Get the userdatum to read the field from as the expected C++ class/struct
            auto userdata = static_cast<T*>(lua_touserdata(L, -2));
            assert(userdata != nullptr);

            // Get the map of upvalues containing the member pointers of the C++ class/struct
            auto upvalues = static_cast<UpvalueMap*>(lua_touserdata(L, lua_upvalueindex(1)));
            assert(upvalues != nullptr);

            // Find the correct upvalue containing the member pointer of the C++ class/struct
            auto it = upvalues->find(key);
            assert(it != upvalues->end());

            // Get the raw pointer to the upvalue (it's stored as a unique pointer)
            Upvalue* up = it->second.get();
            assert(up != nullptr);

            // Invoke the upvalue object to read the field from the userdatum using the member pointer and push the value
            // to the Lua stack
            up->push_to_lua(L, userdata);
            return 1;
        }

        template<class T>
        static int lua_newindex(lua_State* L) {
            // Get the name of the field to write to
            const char* key = lua_tostring(L, -2);
            assert(key != nullptr);

            // Get the userdatum to write the field to as the expected C++ class/struct
            auto userdata = static_cast<T*>(lua_touserdata(L, -3));
            assert(userdata != nullptr);

            // Get the map of upvalues containing the member pointers of the C++ class/struct
            auto upvalues = static_cast<UpvalueMap*>(lua_touserdata(L, lua_upvalueindex(1)));
            assert(upvalues != nullptr);

            // Find the correct upvalue containing the member pointer of the C++ class/struct
            auto it = upvalues->find(key);
            assert(it != upvalues->end());

            // Get the raw pointer to the upvalue (it's stored as a unique pointer)
            Upvalue* up = it->second.get();
            assert(up != nullptr);

            // Invoke the upvalue object to parse the value from the Lua stack and write it to the field to the userdatum
            // using the member pointer
            up->parse_from_lua(L, userdata);
            return 0;
        }
    };
}
