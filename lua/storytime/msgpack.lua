local MessagePack = require("MessagePack")

local function to_binary(value)
    assert(value ~= nil, "Cannot pack nil value")

    local data = MessagePack.pack(value)
    assert(data ~= nil, "Packed data cannot be nil")
    assert(data:len() > 0, "Packed data cannot be empty")

    return data
end

local function from_binary(data)
    assert(data ~= nil, "Cannot unpack nil data")

    local value = MessagePack.unpack(data)
    assert(value ~= nil, "Unpacked value cannot be nil")

    return value
end

return {
    from_binary = from_binary,
    to_binary = to_binary,
}