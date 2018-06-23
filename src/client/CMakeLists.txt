set(PARENT_PROJECT_NAME ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.5)

project(
    ${PARENT_PROJECT_NAME}_client
    LANGUAGES C
    VERSION ${${PARENT_PROJECT_NAME}_VERSION}
)

add_executable(${PROJECT_NAME}
    main.c
    tty.c
    memory.c
)
add_dependencies(${PROJECT_NAME} compile_commands)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(libs logc uv flatccrt pthread)
else()
    set(libs logc uv flatccrt_d pthread)
endif()

target_link_libraries(${PROJECT_NAME} ${PARENT_PROJECT_NAME}_protocol ${libs})
