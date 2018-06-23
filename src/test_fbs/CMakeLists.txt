set(PARENT_PROJECT_NAME ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.5)

project(
    ${PARENT_PROJECT_NAME}_testfbs
    LANGUAGES C
    VERSION ${${PARENT_PROJECT_NAME}_VERSION}
)


add_executable(${PROJECT_NAME}
    main.c
)
add_dependencies(${PROJECT_NAME} compile_commands fbs_message)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(libs flatccrt pthread)
else()
    set(libs flatccrt_d pthread)
endif()

target_link_libraries(${PROJECT_NAME} ${libs})
