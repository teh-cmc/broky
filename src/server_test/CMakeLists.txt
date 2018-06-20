compute_compile_commands()

add_executable(broky_server_test
    main.c

    ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
)

add_test(test_all valgrind ${VALGRIND_FLAGS} ./broky_server_test)
