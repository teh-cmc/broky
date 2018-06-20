compute_compile_commands()

add_executable(broky_server
    main.c
    listener.c
    dispatcher.c
    worker.c
    stream.c
    memory.c

    ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json
)

if(CMAKE_BUILD_TYPE MATCHES Release)
    set(libs logc uv flatccrt pthread)
else()
    set(libs logc uv flatccrt_d pthread)
endif()

target_link_libraries(broky_server ${libs})
