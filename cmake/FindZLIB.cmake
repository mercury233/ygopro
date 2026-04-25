# cmake/FindZLIB.cmake
# When zlib is being built as a subdirectory (target "zlibstatic" already exists),
# skip the normal file search and satisfy find_package(ZLIB) using that target.

if(TARGET zlibstatic)
    set(ZLIB_FOUND TRUE)
    set(ZLIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/zlib" "${CMAKE_BINARY_DIR}/zlib")
    set(ZLIB_LIBRARIES ZLIB::ZLIB)
    if(NOT TARGET ZLIB::ZLIB)
        add_library(ZLIB::ZLIB ALIAS zlibstatic)
    endif()
    return()
endif()

# Fallback: delegate to CMake's built-in FindZLIB
list(REMOVE_ITEM CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(ZLIB ${ZLIB_FIND_VERSION} ${_ZLIB_QUIET_OR_REQUIRED})
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
