externalproject_add(
    project_flatcc
    GIT_REPOSITORY https://github.com/dvidelabs/flatcc.git
    GIT_TAG v0.5.1

    UPDATE_DISCONNECTED 1
    UPDATE_COMMAND ""
    CMAKE_ARGS
        -DFLATCC_PORTABLE=OFF
        -DFLATCC_RTONLY=OFF
        -DFLATCC_INSTALL=ON
        -DFLATCC_TEST=OFF
        -DFLATCC_COVERAGE=OFF
        -DCMAKE_INSTALL_PREFIX=${PROJECT_EXTERNAL_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    TEST_COMMAND ""
)
