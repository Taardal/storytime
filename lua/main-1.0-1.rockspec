package = "main"
version = "1.0-1"
source = {
  url = "",
}
build = {
  type = "builtin",
  modules = {
    main = "main.lua"
  }
}
dependencies = {
  "lua-messagepack = 0.5.4-1"
}