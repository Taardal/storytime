#######################################
# Pre-flight checks                   #
#######################################

if ("${RESOURCES_SOURCE_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable RESOURCES_SOURCE_DIR (model source code directory path)")
endif ()

if (NOT EXISTS ${RESOURCES_SOURCE_DIR})
    message(FATAL_ERROR "Could not find model source code directory [${RESOURCES_SOURCE_DIR}]")
endif ()

if ("${RESOURCES_OUTPUT_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable RESOURCES_OUTPUT_DIR (model compilation output directory path)")
endif ()

#######################################
# Copy skybox files                   #
#######################################

file(REMOVE_RECURSE ${RESOURCES_OUTPUT_DIR})
file(MAKE_DIRECTORY ${RESOURCES_OUTPUT_DIR})

file(GLOB RESOURCE_FILES "${RESOURCES_SOURCE_DIR}/*")
foreach (RESOURCE_FILE ${RESOURCE_FILES})
    file(COPY ${RESOURCE_FILE} DESTINATION ${RESOURCES_OUTPUT_DIR})
    message("Copied model file [${RESOURCE_FILE}] to [${RESOURCES_OUTPUT_DIR}]")
endforeach ()
