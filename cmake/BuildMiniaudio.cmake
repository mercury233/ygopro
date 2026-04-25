# BuildMiniaudio.cmake
# Builds miniaudio as a static library directly from the split amalgamation source.
# No opus/vorbis support (MSVC build uses built-in decoders only).
# Mirrors premake/miniaudio/premake5.lua (no MINIAUDIO_SUPPORT_OPUS_VORBIS branch).

set(MINIAUDIO_DIR "${CMAKE_SOURCE_DIR}/miniaudio")

add_library(miniaudio STATIC
    ${MINIAUDIO_DIR}/miniaudio.c
    ${MINIAUDIO_DIR}/miniaudio.h
)

target_include_directories(miniaudio PUBLIC
    ${MINIAUDIO_DIR}
)

target_compile_definitions(miniaudio PRIVATE
    MA_NO_ENCODING
    MA_NO_GENERATION
)
