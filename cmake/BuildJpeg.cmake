# Minimal libjpeg-turbo static library built from source, SIMD disabled via YGOPRO_NO_SIMD.
# The custom jpeg/src/jconfig.h guards WITH_SIMD with #ifndef YGOPRO_NO_SIMD,
# so defining YGOPRO_NO_SIMD is the correct way to disable SIMD for this project.
# File list mirrors what the upstream CMakeLists.txt puts in JPEG_SOURCES (no SIMD objects).

set(JPEG_SRC "${CMAKE_SOURCE_DIR}/jpeg/src")
set(JPEG_WRAP "${CMAKE_SOURCE_DIR}/jpeg/src/wrapper")

add_library(jpeg STATIC
    ${JPEG_SRC}/jcapimin.c
    ${JPEG_WRAP}/jcapistd-8.c   ${JPEG_WRAP}/jcapistd-12.c  ${JPEG_WRAP}/jcapistd-16.c
    ${JPEG_WRAP}/jccoefct-8.c   ${JPEG_WRAP}/jccoefct-12.c
    ${JPEG_WRAP}/jccolor-8.c    ${JPEG_WRAP}/jccolor-12.c    ${JPEG_WRAP}/jccolor-16.c
    ${JPEG_WRAP}/jcdctmgr-8.c   ${JPEG_WRAP}/jcdctmgr-12.c
    ${JPEG_WRAP}/jcdiffct-8.c   ${JPEG_WRAP}/jcdiffct-12.c  ${JPEG_WRAP}/jcdiffct-16.c
    ${JPEG_SRC}/jchuff.c        ${JPEG_SRC}/jcicc.c          ${JPEG_SRC}/jcinit.c
    ${JPEG_SRC}/jclhuff.c
    ${JPEG_WRAP}/jclossls-8.c   ${JPEG_WRAP}/jclossls-12.c  ${JPEG_WRAP}/jclossls-16.c
    ${JPEG_WRAP}/jcmainct-8.c   ${JPEG_WRAP}/jcmainct-12.c  ${JPEG_WRAP}/jcmainct-16.c
    ${JPEG_SRC}/jcmarker.c      ${JPEG_SRC}/jcmaster.c       ${JPEG_SRC}/jcomapi.c
    ${JPEG_SRC}/jcparam.c       ${JPEG_SRC}/jcphuff.c
    ${JPEG_WRAP}/jcprepct-8.c   ${JPEG_WRAP}/jcprepct-12.c  ${JPEG_WRAP}/jcprepct-16.c
    ${JPEG_WRAP}/jcsample-8.c   ${JPEG_WRAP}/jcsample-12.c  ${JPEG_WRAP}/jcsample-16.c
    ${JPEG_SRC}/jctrans.c       ${JPEG_SRC}/jdapimin.c
    ${JPEG_WRAP}/jdapistd-8.c   ${JPEG_WRAP}/jdapistd-12.c  ${JPEG_WRAP}/jdapistd-16.c
    ${JPEG_SRC}/jdatadst.c      ${JPEG_SRC}/jdatasrc.c
    ${JPEG_WRAP}/jdcoefct-8.c   ${JPEG_WRAP}/jdcoefct-12.c
    ${JPEG_WRAP}/jdcolor-8.c    ${JPEG_WRAP}/jdcolor-12.c    ${JPEG_WRAP}/jdcolor-16.c
    ${JPEG_WRAP}/jddctmgr-8.c   ${JPEG_WRAP}/jddctmgr-12.c
    ${JPEG_WRAP}/jddiffct-8.c   ${JPEG_WRAP}/jddiffct-12.c  ${JPEG_WRAP}/jddiffct-16.c
    ${JPEG_SRC}/jdhuff.c        ${JPEG_SRC}/jdicc.c          ${JPEG_SRC}/jdinput.c
    ${JPEG_SRC}/jdlhuff.c
    ${JPEG_WRAP}/jdlossls-8.c   ${JPEG_WRAP}/jdlossls-12.c  ${JPEG_WRAP}/jdlossls-16.c
    ${JPEG_WRAP}/jdmainct-8.c   ${JPEG_WRAP}/jdmainct-12.c  ${JPEG_WRAP}/jdmainct-16.c
    ${JPEG_SRC}/jdmarker.c      ${JPEG_SRC}/jdmaster.c
    ${JPEG_WRAP}/jdmerge-8.c    ${JPEG_WRAP}/jdmerge-12.c
    ${JPEG_SRC}/jdphuff.c
    ${JPEG_WRAP}/jdpostct-8.c   ${JPEG_WRAP}/jdpostct-12.c  ${JPEG_WRAP}/jdpostct-16.c
    ${JPEG_WRAP}/jdsample-8.c   ${JPEG_WRAP}/jdsample-12.c  ${JPEG_WRAP}/jdsample-16.c
    ${JPEG_SRC}/jdtrans.c       ${JPEG_SRC}/jerror.c         ${JPEG_SRC}/jfdctflt.c
    ${JPEG_WRAP}/jfdctfst-8.c   ${JPEG_WRAP}/jfdctfst-12.c
    ${JPEG_WRAP}/jfdctint-8.c   ${JPEG_WRAP}/jfdctint-12.c
    ${JPEG_WRAP}/jidctflt-8.c   ${JPEG_WRAP}/jidctflt-12.c
    ${JPEG_WRAP}/jidctfst-8.c   ${JPEG_WRAP}/jidctfst-12.c
    ${JPEG_WRAP}/jidctint-8.c   ${JPEG_WRAP}/jidctint-12.c
    ${JPEG_WRAP}/jidctred-8.c   ${JPEG_WRAP}/jidctred-12.c
    ${JPEG_SRC}/jmemmgr.c       ${JPEG_SRC}/jmemnobs.c       ${JPEG_SRC}/jpeg_nbits.c
    ${JPEG_WRAP}/jquant1-8.c    ${JPEG_WRAP}/jquant1-12.c
    ${JPEG_WRAP}/jquant2-8.c    ${JPEG_WRAP}/jquant2-12.c
    ${JPEG_WRAP}/jutils-8.c     ${JPEG_WRAP}/jutils-12.c     ${JPEG_WRAP}/jutils-16.c
    # Arithmetic coding
    ${JPEG_SRC}/jaricom.c       ${JPEG_SRC}/jcarith.c        ${JPEG_SRC}/jdarith.c
)

# YGOPRO_NO_SIMD causes jpeg/src/jconfig.h to omit WITH_SIMD, so jsimd.h
# declares no SIMD functions and the source files skip all SIMD code paths.
target_compile_definitions(jpeg PRIVATE YGOPRO_NO_SIMD)
target_include_directories(jpeg PUBLIC ${JPEG_SRC})
