add_subdirectory(test_fbs)
add_subdirectory(common)

option(BROKY_BUILD_SERVER "build Broky server" ON)
option(BROKY_BUILD_CLIENT "build Broky client" ON)

if(BROKY_BUILD_SERVER)
    add_subdirectory(server)
endif()

if(BROKY_BUILD_CLIENT)
    add_subdirectory(client)
endif()
