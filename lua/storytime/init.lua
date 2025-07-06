---@module Storytime

local class = require("storytime.class")
local msgpack = require("storytime.msgpack")

---@type Storytime
local M = {}

---@field class function Define a class in Lua.
M.class = class

---@field from_binary function Deserialize binary data into a Lua value.
M.from_binary = msgpack.from_binary

---@field to_binary function Serialize a Lua value into binary data.
M.to_binary = msgpack.to_binary

return M