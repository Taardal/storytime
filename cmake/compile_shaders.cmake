if ("${SHADERS_SOURCE_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable SHADERS_SOURCE_DIR (shader source code directory path)")
endif ()

if (NOT EXISTS ${SHADERS_SOURCE_DIR})
    message(FATAL_ERROR "Could not find shader source code directory [${SHADERS_SOURCE_DIR}]")
endif ()

if ("${SHADERS_OUTPUT_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable SHADERS_OUTPUT_DIR (shader compilation output directory path)")
endif ()

find_program(GLSLC glslc)
if (NOT GLSLC)
    message(FATAL_ERROR "Could not find shader compiler (glslc)")
endif ()

function(compile_shaders SHADER_FILE_TYPE_WILDCARD)
    file(GLOB SHADER_FILES "${SHADERS_SOURCE_DIR}/${SHADER_FILE_TYPE_WILDCARD}")
    foreach (SHADER_FILE ${SHADER_FILES})
        get_filename_component(FILENAME ${SHADER_FILE} NAME)
        set(OUTPUT_FILE "${SHADERS_OUTPUT_DIR}/${FILENAME}.spv")
        execute_process(
                COMMAND ${GLSLC} -o ${OUTPUT_FILE} ${SHADER_FILE}
                RESULT_VARIABLE result
        )
        if (result EQUAL 0)
            message("Compiled shader file [${SHADER_FILE}] to [${OUTPUT_FILE}]")
        else ()
            message(FATAL_ERROR "Could not compile shader file [${SHADER_FILE}]")
        endif ()
    endforeach ()
endfunction()

file(REMOVE_RECURSE ${SHADERS_OUTPUT_DIR})
file(MAKE_DIRECTORY ${SHADERS_OUTPUT_DIR})

compile_shaders(*.vert)
compile_shaders(*.frag)