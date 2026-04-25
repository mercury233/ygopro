cmake_minimum_required (VERSION 3.10)

# !!! This file is NOT supported. Please use premake to build the project. !!!

project (YGOPro)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include (AutoFiles)

include (platform/settings)

if (MSVC)
    include (BuildEvent)
    add_subdirectory (freetype)
    include (BuildSqlite)
    include (BuildLua)
    # lzma and jpeg are built from source via custom cmake modules
    include (BuildLzma)
    include (BuildJpeg)
    # zlib and png use their own CMakeLists.txt
    set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(ZLIB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    add_subdirectory (zlib)
    set(PNG_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
    set(PNG_SHARED OFF CACHE BOOL "" FORCE)
    set(PNG_TESTS OFF CACHE BOOL "" FORCE)
    add_subdirectory (png)
    # irrlicht depends on jpeg and png, so must come after them
    include (BuildIrrlicht)
else ()
    find_package(LibEvent REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(Irrlicht REQUIRED)
    find_package(Sqlite REQUIRED)
    find_package(Lua REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(JPEG REQUIRED)
    find_package(PNG REQUIRED)
    find_package(LibLZMA REQUIRED)
endif ()

option(USE_AUDIO "Use audio support" ON)
if (USE_AUDIO)
    include (BuildMiniaudio)
endif ()

add_subdirectory (ocgcore)
add_subdirectory (gframe)
