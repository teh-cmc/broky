externalproject_add(
    project_logc
    GIT_REPOSITORY https://github.com/teh-cmc/log.c.git
    GIT_TAG 8fe43a59a35c189ab6240bb79bbc9acd584e89b5

    UPDATE_DISCONNECTED 1
    UPDATE_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${PROJECT_EXTERNAL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    TEST_COMMAND ""
)
