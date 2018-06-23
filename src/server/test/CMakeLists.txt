set(PROJECT_NAME_TEST ${PROJECT_NAME}_test)

add_executable(${PROJECT_NAME_TEST}
    main.c
)
add_dependencies(${PROJECT_NAME_TEST} compile_commands)

#target_link_libraries(${PROJECT_NAME_TEST})

add_test(test_all valgrind ${VALGRIND_FLAGS} ./${PROJECT_NAME_TEST})
