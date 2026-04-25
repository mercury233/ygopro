file(GLOB LUA_SOURCES "${CMAKE_SOURCE_DIR}/lua/src/*.c")
list(REMOVE_ITEM LUA_SOURCES
    "${CMAKE_SOURCE_DIR}/lua/src/lua.c"
    "${CMAKE_SOURCE_DIR}/lua/src/luac.c"
    "${CMAKE_SOURCE_DIR}/lua/src/linit.c"
    "${CMAKE_SOURCE_DIR}/lua/src/onelua.c"
)

# Lua must be compiled as C++ (required by ocgcore)
set_source_files_properties(${LUA_SOURCES} PROPERTIES LANGUAGE CXX)

add_library(lua STATIC ${LUA_SOURCES})

target_include_directories(lua PUBLIC ${CMAKE_SOURCE_DIR}/lua/src)

target_compile_definitions(lua PRIVATE
    $<$<CONFIG:Debug>:LUA_USE_APICHECK>
)
