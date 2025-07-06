#######################################
# Pre-flight checks                   #
#######################################

if ("${LUA_SOURCE_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable LUA_SOURCE_DIR (Lua source code directory path)")
endif ()

if (NOT EXISTS ${LUA_SOURCE_DIR})
    message(FATAL_ERROR "Could not find Lua source code directory [${LUA_SOURCE_DIR}]")
endif ()

if ("${LUA_OUTPUT_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable LUA_OUTPUT_DIR (Lua compilation output directory path)")
endif ()

find_program(LUAC luac)
if (NOT LUAC)
    message(FATAL_ERROR "Could not find Lua compiler (luac)")
endif ()

#######################################
# Install Lua dependencies            #
#######################################

find_program(LUAROCKS luarocks)
if (NOT LUAROCKS)
    message(FATAL_ERROR "Could not find luarocks executable")
endif ()

file(GLOB LUA_ROCKSPEC_FILES "${LUA_SOURCE_DIR}/*.rockspec")
foreach (LUA_ROCKSPEC_FILE ${LUA_ROCKSPEC_FILES})
    message("Installing Lua rocks from [${LUA_ROCKSPEC_FILE}]")
    execute_process(
            COMMAND ${LUAROCKS} install --tree=${LUA_SOURCE_DIR} --only-deps ${LUA_ROCKSPEC_FILE}
            RESULT_VARIABLE result
    )
    if (NOT result EQUAL 0)
        message(FATAL_ERROR "Could not install Lua rocks from [${LUA_ROCKSPEC_FILE}]: ${result}")
    endif ()
endforeach ()

#######################################
# Compile lua files                   #
#######################################

function(compile_lua_file LUA_SOURCE_FILE LUA_OUTPUT_FILE)
    execute_process(
            COMMAND ${LUAC} -o ${LUA_OUTPUT_FILE} ${LUA_SOURCE_FILE}
            RESULT_VARIABLE result
    )
    if (result EQUAL 0)
        message("Compiled Lua file [${LUA_SOURCE_FILE}] to [${LUA_OUTPUT_FILE}]")
    else ()
        message(FATAL_ERROR "Could not compile Lua file [${LUA_SOURCE_FILE}]")
    endif ()
endfunction()

file(MAKE_DIRECTORY ${LUA_OUTPUT_DIR})
file(GLOB_RECURSE LUA_SOURCE_FILES "${LUA_SOURCE_DIR}**/*.lua")
foreach (LUA_SOURCE_FILE ${LUA_SOURCE_FILES})

    # Determine source file relative path
    #
    # Given...
    #   [LUA_SOURCE_DIR] /foo/bar/project/lua
    #   [LUA_SOURCE_FILE] /foo/bar/project/lua/scenes/sandbox.lua
    #
    # Source file relative path should be...
    #   [LUA_SOURCE_FILE_RELATIVE_PATH] scenes/sandbox.lua
    #
    string(REPLACE "${LUA_SOURCE_DIR}/" "" LUA_SOURCE_FILE_RELATIVE_PATH "${LUA_SOURCE_FILE}")

    # Skip LuaRocks metadata directory (rockspec files, docs, unit tests etc. for installed rocks)
    if ("${LUA_SOURCE_FILE_RELATIVE_PATH}" MATCHES "lib/luarocks/*")
        continue()
    endif ()

    # Determine output file absolute and relative path
    #
    # Given...
    #   [LUA_OUTPUT_DIR] /foo/bar/project/bin/lua
    #   [LUA_SOURCE_FILE_RELATIVE_PATH] scenes/sandbox.lua
    #
    # Output file absolute and relative paths should be...
    #   [LUA_OUTPUT_FILE] /foo/bar/project/bin/lua/scenes/sandbox.out
    #   [LUA_OUTPUT_FILE_RELATIVE_PATH] scenes/sandbox.out
    #
    set(LUA_OUTPUT_FILE "${LUA_OUTPUT_DIR}/${LUA_SOURCE_FILE_RELATIVE_PATH}")
    string(REPLACE ".lua" ".out" LUA_OUTPUT_FILE "${LUA_OUTPUT_FILE}")
    string(REPLACE "${LUA_OUTPUT_DIR}/" "" LUA_OUTPUT_FILE_RELATIVE_PATH "${LUA_OUTPUT_FILE}")

    # Create any directory in the output path that does not exist before compiling the Lua file
    #
    # 1. Check if the output relative file path contains slashes
    # 2. If it does, determine the relative directory path by removing the file name at the end
    # 3. Create the directory path
    string(FIND "${LUA_OUTPUT_FILE_RELATIVE_PATH}" "/" LUA_OUTPUT_FILE_RELATIVE_PATH_SLASH_INDEX)
    if (NOT ${LUA_OUTPUT_FILE_RELATIVE_PATH_SLASH_INDEX} EQUAL -1)

        # Determine output directory absolute path
        #
        # Given...
        #   [LUA_OUTPUT_FILE] /foo/bar/project/bin/lua/scenes/sandbox.out
        #
        # Output directory absolute path should be...
        #   [LUA_OUTPUT_FILE_NAME] sandbox.out
        #   [LUA_OUTPUT_FILE_DIR] /foo/bar/project/bin/lua/scenes
        #
        get_filename_component(LUA_OUTPUT_FILE_NAME ${LUA_OUTPUT_FILE} NAME)
        string(REPLACE "${LUA_OUTPUT_FILE_NAME}" "" LUA_OUTPUT_FILE_DIR "${LUA_OUTPUT_FILE}")

        # Create the output directory path
        file(MAKE_DIRECTORY "${LUA_OUTPUT_FILE_DIR}")
    endif ()

    compile_lua_file("${LUA_SOURCE_FILE}" "${LUA_OUTPUT_FILE}")
endforeach ()

