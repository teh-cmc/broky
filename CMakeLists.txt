cmake_minimum_required(VERSION 3.5)

project(broky LANGUAGES C VERSION 0.1.0)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   message(STATUS "CMAKE_INSTALL_PREFIX is not set, defaulting to '${CMAKE_BINARY_DIR}/install'")
   set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING "" FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "CMAKE_BUILD_TYPE is not set, defaulting to 'Debug'")
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

## -----------------------------------------------------------------------------

enable_testing()

set(VALGRIND_FLAGS
        --quiet
        --leak-check=full
        --show-leak-kinds=all
        --track-origins=yes
        --error-exitcode=1
)

## -----------------------------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(compile_commands)
add_custom_command(
   TARGET compile_commands
   DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
   COMMAND cp ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}
   VERBATIM
)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fno-omit-frame-pointer -DDEBUG")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Werror -Os")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Werror -O3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -O2 -g -fno-omit-frame-pointer")

## -----------------------------------------------------------------------------

set(PROJECT_EXTERNAL_DIR "${PROJECT_BINARY_DIR}/external")
set(PROJECT_EXTERNAL_DIR_INCLUDE "${PROJECT_EXTERNAL_DIR}/include")
set(PROJECT_EXTERNAL_DIR_LIB "${PROJECT_EXTERNAL_DIR}/lib")
set(PROJECT_EXTERNAL_DIR_BIN "${PROJECT_EXTERNAL_DIR}/bin")

add_subdirectory(external)

include_directories("${PROJECT_EXTERNAL_DIR_INCLUDE}")
link_directories("${PROJECT_EXTERNAL_DIR_LIB}")

## -----------------------------------------------------------------------------

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_BINARY_DIR}/include")

add_subdirectory(fbs)
add_subdirectory(src)
