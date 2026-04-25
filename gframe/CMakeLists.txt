project (ygopro)

if (NOT WIN32 AND NOT APPLE)
    include (FindX11)   
endif ()

set (AUTO_FILES_RESULT)
if (MSVC)
    AutoFiles("." "res" "\\.(rc)$")
    AutoFiles("." "src" "\\.(cpp|c|h)$" "CGUIButton.cpp|lzma/\\.*")
else ()
    AutoFiles("." "src" "\\.(cpp|c|h)$" "lzma/\\.*")
endif ()

if (MSVC)
    add_executable (ygopro WIN32 ${AUTO_FILES_RESULT})
    set_target_properties (ygopro PROPERTIES
        LINK_FLAGS /ENTRY:"mainCRTStartup"
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ygopro)
else ()
    add_executable (ygopro ${AUTO_FILES_RESULT})
endif ()

target_link_libraries (ygopro ocgcore lzma)

if (MSVC)
    target_link_libraries (ygopro irrlicht freetype sqlite3 event jpeg png_static zlibstatic)
    include_directories (
        "../irrlicht/include"
        "../freetype/include"
        "../event/include"
        "../sqlite3"
        "../jpeg/src"
        "../lzma/src/liblzma/api"
    )
else ()
    target_link_libraries (ygopro
        ${IRRLICHT_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        ${SQLITE_LIBRARIES}
        ${LIBEVENT_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}
        LibLZMA::LibLZMA
    )
    include_directories (
        ${IRRLICHT_INCLUDE_DIR}
        ${FREETYPE_INCLUDE_DIR}
        ${SQLITE_INCLUDE_DIRS}
        ${LIBEVENT_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR}
        ${JPEG_INCLUDE_DIRS}
        ${PNG_INCLUDE_DIRS}
        ${LIBLZMA_INCLUDE_DIRS}
    )
    target_link_libraries (ygopro ${CMAKE_THREAD_LIBS_INIT} ${DL_LIBRARIES})
endif ()

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(ygopro ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
endif ()

if (USE_AUDIO)
    add_definitions ("-DYGOPRO_USE_AUDIO")
    add_definitions ("-DYGOPRO_USE_MINIAUDIO")
    target_link_libraries (ygopro miniaudio)
    include_directories ("../miniaudio")
endif ()

if (WIN32)
    target_link_libraries (ygopro ws2_32 winmm gdi32 kernel32 user32 imm32 opengl32)
endif ()
