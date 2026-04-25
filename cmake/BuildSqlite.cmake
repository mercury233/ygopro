add_library(sqlite3 STATIC
    ${CMAKE_SOURCE_DIR}/sqlite3/sqlite3.c
    ${CMAKE_SOURCE_DIR}/sqlite3/sqlite3.h
)

target_compile_definitions(sqlite3 PRIVATE
    SQLITE_DQS=0
    SQLITE_DEFAULT_MEMSTATUS=0
    SQLITE_MAX_EXPR_DEPTH=0
    SQLITE_OMIT_DECLTYPE
    SQLITE_OMIT_DEPRECATED
    SQLITE_OMIT_PROGRESS_CALLBACK
    SQLITE_OMIT_SHARED_CACHE
    SQLITE_TRUSTED_SCHEMA=0
)

target_include_directories(sqlite3 PUBLIC ${CMAKE_SOURCE_DIR}/sqlite3)
