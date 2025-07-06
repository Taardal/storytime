if ("${SOURCE_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable SOURCE_DIR")
endif ()

if (NOT EXISTS ${SOURCE_DIR})
    message(FATAL_ERROR "Could not find source directory [${SOURCE_DIR}]")
endif ()

if ("${OUTPUT_DIR}" STREQUAL "")
    message(FATAL_ERROR "Missing required variable OUTPUT_DIR")
endif ()

set(
    EXCLUDED_FILES
    ".DS_Store"     # macOS
    "Thumbs.db"     # Windows
)

file(GLOB FILES "${SOURCE_DIR}/*")

foreach (FILE ${FILES})
    get_filename_component(FILE_NAME ${FILE} NAME)
    list(FIND EXCLUDED_FILES ${FILE_NAME} EXCLUDE_INDEX)
    if (EXCLUDE_INDEX EQUAL -1)
        file(COPY ${FILE} DESTINATION ${OUTPUT_DIR})
        message("Copied file [${FILE}] to [${OUTPUT_DIR}]")
    endif ()
endforeach ()