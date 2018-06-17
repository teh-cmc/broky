include(ExternalProject)

## -----------------------------------------------------------------------------

macro(assert var)
    if(NOT ${var})
        message(FATAL_ERROR "${var} must be defined.")
    endif()
endmacro()

assert(PROJECT_EXTERNAL_DIR)
assert(PROJECT_EXTERNAL_DIR_INCLUDE)
assert(PROJECT_EXTERNAL_DIR_LIB)
assert(PROJECT_EXTERNAL_DIR_BIN)

## -----------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

include(logc)
include(libuv)
include(flatcc)
