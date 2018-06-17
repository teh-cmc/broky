externalproject_add(
    project_libuv
    GIT_REPOSITORY https://github.com/libuv/libuv.git
    GIT_TAG v1.20.3

    BUILD_IN_SOURCE 1
    UPDATE_DISCONNECTED 1
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND sh <SOURCE_DIR>/autogen.sh &&
                      ./configure --prefix "${PROJECT_EXTERNAL_DIR}/libuv"
                                  --includedir "${PROJECT_EXTERNAL_DIR_INCLUDE}/libuv"
                                  --libdir "${PROJECT_EXTERNAL_DIR_LIB}"
    BUILD_COMMAND make -j8
    INSTALL_COMMAND make install
    TEST_COMMAND ""
)

add_library(libuv STATIC IMPORTED)
set_property(TARGET libuv
    PROPERTY IMPORTED_LOCATION ${PROJECT_EXTERNAL_DIR_LIB}/libuv.a)
