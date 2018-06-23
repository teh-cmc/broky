set(PARENT_PROJECT_NAME ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.5)

project(
    ${PARENT_PROJECT_NAME}_protocol
    LANGUAGES C
    VERSION ${${PARENT_PROJECT_NAME}_VERSION}
)

add_library(${PROJECT_NAME}
    protocol.c
)
add_dependencies(${PROJECT_NAME} compile_commands fbs_requests)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(libs logc flatccrt)
else()
    set(libs logc flatccrt_d)
endif()
target_link_libraries(${PROJECT_NAME} ${libs})

add_subdirectory(test)
