# Minimal liblzma static library built from source (mirrors lzma/premake5.lua)

add_library(lzma STATIC
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/check/crc32_fast.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/common.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_common.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_buffer_encoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_encoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_flags_encoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_buffer_decoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_decoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common/filter_flags_decoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma/lzma_encoder_presets.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma/lzma_encoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma/lzma_encoder_optimum_fast.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma/lzma_encoder_optimum_normal.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma/lzma_decoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma/fastpos_table.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lz/lz_encoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lz/lz_encoder_mf.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lz/lz_decoder.c
    ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/rangecoder/price_table.c
)

target_compile_definitions(lzma PRIVATE
    HAVE_INTTYPES_H=1
    HAVE_STDINT_H=1
    HAVE_STDBOOL_H=1
    HAVE_STRING_H=1
    HAVE_STDLIB_H=1
    HAVE_STDIO_H=1
    HAVE_CHECK_CRC32=1
    HAVE_ENCODERS=1
    HAVE_ENCODER_LZMA1=1
    HAVE_DECODERS=1
    HAVE_DECODER_LZMA1=1
    HAVE_MF_HC3=1
    HAVE_MF_HC4=1
    HAVE_MF_BT2=1
    HAVE_MF_BT3=1
    HAVE_MF_BT4=1
    HAVE_VISIBILITY=0
)

target_include_directories(lzma
    PRIVATE
        ${CMAKE_SOURCE_DIR}/lzma/src/common
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/common
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/check
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lzma
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/lz
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/rangecoder
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/simple
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/delta
    PUBLIC
        ${CMAKE_SOURCE_DIR}/lzma/src/liblzma/api
)
