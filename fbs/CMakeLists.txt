set(cmd ${PROJECT_EXTERNAL_DIR_BIN}/flatcc_d)
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(cmd ${PROJECT_EXTERNAL_DIR_BIN}/flatcc)
endif()

file(GLOB files ${CMAKE_CURRENT_LIST_DIR}/*.fbs)
foreach(filepath ${files})
    get_filename_component(filename ${filepath} NAME_WE)
    add_custom_target(fbs_${filename} ALL)
    add_custom_command (
        TARGET fbs_${filename}
        COMMAND cmake -E make_directory "${PROJECT_BINARY_DIR}/include/fbs"
        COMMAND ${cmd} -a --json -o "${PROJECT_BINARY_DIR}/include/fbs" ${filepath}
        COMMENT "Building C headers for ${filename}"
        DEPENDS project_flatcc
    )
endforeach()
