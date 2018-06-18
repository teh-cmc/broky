compute_compile_commands()

add_executable(test_fbs
    main.c

    ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(libs flatccrt pthread)
else()
    set(libs flatccrt_d pthread)
endif()

add_dependencies(test_fbs fbs_message)
target_link_libraries(test_fbs ${libs})
