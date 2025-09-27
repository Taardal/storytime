---Define a Lua class.
---
---A Lua class emulates object-oriented programming and enables inheritance hierarchies.
---It also provides the ability for Lua "classes" to extend C++ classes.
---
---This function is an implementation of the function described in the book "Game Coding Complete: Fourth Edition":
---https://www.amazon.com/Game-Coding-Complete-Fourth-McShaffry/dp/1133776574
---
---@param parent_class table|nil The parent class table to inherit from. Expected to also be created with this function. If the class doesn't inherit from any parent class, this should be nil.
---@param class_body table|nil The class body table containing the members and methods of the class. Defaults to an empty table if nil.
---@return table A new class table with members and optional inheritance.
local function class(parent_class, class_body)
    local clazz = class_body or {}

    -- Set up the inheritance chain: class --> parent class
    if (parent_class ~= nil) then
        setmetatable(clazz, clazz)
        clazz.__index = parent_class
    end

    -- Add function to create new instance tables from the class table.
    clazz.new = function(class_self, construction_data)
        local instance

        -- If the class table has a parent class, let the parent class create the instance table so that the parent class may add
        -- any fields, functions and custom operators as needed.
        if (parent_class ~= nil) then

            -- Pass in the class table in case the parent class is a C++ class.
            -- C++ parent classes, that binds Lua together with C++ (a.k.a. "bindings"), needs to obtain Lua references to the
            -- fields and functions defined in the Lua class table so that C++ code can invoke them.
            instance = parent_class:new(construction_data, class_self);
        else
            instance = construction_data or {};
        end

        -- Bind the instance to the class inheritance chain: instance --> class --> parent class.
        setmetatable(instance, instance)
        instance.__index = class_self

        -- Copy metafields and metafunctions from the class table to the instance metatable.
        --
        -- In C++, you can define custom operators on a class. For example, you can override the `+` operator on a class so that you
        -- can add two instances of that class together in the same you would add two numbers together.
        --
        -- Lua tables can also define custom operators. When we are mimicing OOP like this, we want to define these custom operators
        -- on the class tables, and have the instances be able to use them automatically.
        --
        -- Because the instance metatable and the class table are two different tables, we have to manually copy all custom operators
        -- from the class table to the instance metatable so that the instance can use them.
        --
        if (class_self.__operators ~= nil) then
            for key, val in pairs(class_self.__operators) do
                instance_metatable[key] = val
            end
        end

        -- Return the final instance table, complete with inheritance and custom operators from the class table.
        return instance
    end

    -- Return the final class table with inheritance and the ability to create instances.
    return clazz
end

return class