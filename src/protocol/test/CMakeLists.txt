set(PARENT_PROJECT_NAME ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.5)

project(
    ${PARENT_PROJECT_NAME}_test
    LANGUAGES C
    VERSION ${${PARENT_PROJECT_NAME}_VERSION}
)

add_executable(${PROJECT_NAME}
    main.c
)
add_dependencies(${PROJECT_NAME} compile_commands)

target_link_libraries(${PROJECT_NAME} ${PARENT_PROJECT_NAME})

add_test(${PROJECT_NAME}_all valgrind ${VALGRIND_FLAGS} ./${PROJECT_NAME})
