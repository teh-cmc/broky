cmake_minimum_required(VERSION 3.4)

project(Broky)

## -----------------------------------------------------------------------------

set(PROJECT_SOURCES
    src/main.c
    src/listener.c
    src/dispatcher.c
    src/worker.c
    src/client.c
    src/memory.c
    src/log.c
)

set(PROJECT_THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/third_party")
set(CMAKE_CURRENT_DEPS_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps")

## -----------------------------------------------------------------------------

## TODO(cmc): put external projects somewhere else.
include(ExternalProject)

set(CMAKE_CURRENT_DEPS_DIR_LIBUV "${CMAKE_CURRENT_DEPS_DIR}/libuv")
externalproject_add(
    project_libuv
    GIT_REPOSITORY https://github.com/libuv/libuv.git
    GIT_TAG v1.20.3
    DOWNLOAD_DIR "${CMAKE_CURRENT_DEPS_DIR_LIBUV}"
    SOURCE_DIR "${CMAKE_CURRENT_DEPS_DIR_LIBUV}"
    UPDATE_DISCONNECTED 1
    UPDATE_COMMAND ""
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND sh <SOURCE_DIR>/autogen.sh &&
                      ./configure --prefix "${PROJECT_THIRDPARTY_DIR}/libuv"
    BUILD_COMMAND make -j8
    INSTALL_COMMAND make install
    TEST_COMMAND ""
)
add_library(libuv STATIC IMPORTED)
set_property(TARGET libuv PROPERTY
    IMPORTED_LOCATION ${PROJECT_THIRDPARTY_DIR}/libuv/lib/libuv.a)

## TODO(cmc): add log.c

add_executable(broky ${PROJECT_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_THIRDPARTY_DIR}")
target_link_libraries(broky libuv pthread)

## -----------------------------------------------------------------------------

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_command(
    OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copying compile_commands.json to root directory..."
    VERBATIM
)

## -----------------------------------------------------------------------------

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_COMPILER /usr/bin/clang)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} -Werror -Os")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Werror -O4")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -O2 -g -fno-omit-frame-pointer")
