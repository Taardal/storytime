if ("${RESOURCES_SOURCE_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable RESOURCES_SOURCE_DIR")
endif ()

if (NOT EXISTS ${RESOURCES_SOURCE_DIR})
    message(FATAL_ERROR "Could not find resource directory [${RESOURCES_SOURCE_DIR}]")
endif ()

if ("${RESOURCES_OUTPUT_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable RESOURCES_OUTPUT_DIR")
endif ()

file(REMOVE_RECURSE ${RESOURCES_OUTPUT_DIR})
file(MAKE_DIRECTORY ${RESOURCES_OUTPUT_DIR})

file(GLOB RESOURCE_FILES "${RESOURCES_SOURCE_DIR}/*")
foreach (RESOURCE_FILE ${RESOURCE_FILES})
    file(COPY ${RESOURCE_FILE} DESTINATION ${RESOURCES_OUTPUT_DIR})
    message("Copied resource [${RESOURCE_FILE}] to [${RESOURCES_OUTPUT_DIR}]")
endforeach ()
