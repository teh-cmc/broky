set(src ${CMAKE_CURRENT_LIST_DIR}/message.fbs)

set(cmd ${PROJECT_EXTERNAL_DIR_BIN}/flatcc_d)
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(cmd ${PROJECT_EXTERNAL_DIR_BIN}/flatcc)
endif()

add_custom_target(fbs_message ALL)
add_custom_command (
    TARGET fbs_message
    COMMAND cmake -E make_directory "${PROJECT_BINARY_DIR}/include/fbs"
    COMMAND ${cmd} -a --json -o "${PROJECT_BINARY_DIR}/include/fbs" ${src}
    DEPENDS project_flatcc ${src}
)
