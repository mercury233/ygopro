project "turbojpeg"
    kind "StaticLib"

    local nasm = os.getenv("ASM_NASM") or "nasm"
    local turboSimdNasmDir = path.getabsolute("simd/nasm")
    local turboSimdX64Dir  = path.getabsolute("simd/x86_64")
    local turboSimdX86Dir  = path.getabsolute("simd/i386")

    files {
        -- libjpeg-turbo (libjpeg API)
        -- Keep this aligned with libjpeg-turbo's JPEG_SOURCES in its CMakeLists.txt.
        "src/jcapimin.c",
        "src/jchuff.c",
        "src/jcicc.c",
        "src/jcinit.c",
        "src/jclhuff.c",
        "src/jcmarker.c",
        "src/jcmaster.c",
        "src/jcomapi.c",
        "src/jcparam.c",
        "src/jcphuff.c",
        "src/jctrans.c",
        "src/jdapimin.c",
        "src/jdatadst.c",
        "src/jdatasrc.c",
        "src/jdhuff.c",
        "src/jdicc.c",
        "src/jdinput.c",
        "src/jdlhuff.c",
        "src/jdmarker.c",
        "src/jdmaster.c",
        "src/jdphuff.c",
        "src/jdtrans.c",
        "src/jerror.c",
        "src/jfdctflt.c",
        "src/jmemmgr.c",
        "src/jmemnobs.c",
        "src/jpeg_nbits.c",
        -- Arithmetic coding support (enabled in our jconfig*)
        "src/jaricom.c",
        "src/jcarith.c",
        "src/jdarith.c",
        -- TurboJPEG API sources
        "src/rdbmp.c",
        "src/wrbmp.c",
        "src/turbojpeg.c",
        "src/tjutil.c",
        "src/jdatadst-tj.c",
        "src/jdatasrc-tj.c",
        "src/transupp.c",
        -- Wrapper sources (multiple precisions / variants)
        "src/wrapper/j*.c",
        "src/wrapper/rdppm-*.c",
        "src/wrapper/wrppm-*.c",
    }

    -- NOTE: Several libjpeg-turbo .c files are meant to be #include'd into
    -- other compilation units (not compiled separately.) Those are not listed
    -- above on purpose.

    filter { "system:windows", "architecture:x86_64" }
        includedirs {
            "simd",
            "simd/nasm",
            "simd/x86_64",
        }
        files {
            "simd/x86_64/jsimd.c",
            "simd/x86_64/*.asm",
        }
        -- These are implementation fragments that are %include'd by the
        -- corresponding top-level files (jccolor/jcgray/jdcolor/jdmerge).
        -- They must NOT be assembled as standalone translation units.
        removefiles {
            "simd/x86_64/jccolext-*.asm",
            "simd/x86_64/jcgryext-*.asm",
            "simd/x86_64/jdcolext-*.asm",
            "simd/x86_64/jdmrgext-*.asm",
        }

    filter { "system:windows", "architecture:x86" }
        includedirs {
            "simd",
            "simd/nasm",
            "simd/i386",
        }
        files {
            "simd/i386/jsimd.c",
            "simd/i386/*.asm",
        }
        -- These are implementation fragments that are %include'd by the
        -- corresponding top-level files (jccolor/jcgray/jdcolor/jdmerge).
        -- They must NOT be assembled as standalone translation units.
        removefiles {
            "simd/i386/jccolext-*.asm",
            "simd/i386/jcgryext-*.asm",
            "simd/i386/jdcolext-*.asm",
            "simd/i386/jdmrgext-*.asm",
        }

    -- Build libjpeg-turbo NASM sources (SSE2/AVX2 accelerated routines)
    filter { "system:windows", "architecture:x86_64", "files:simd/**.asm" }
        buildmessage "NASM %{file.relpath}"
        buildcommands {
            '"' .. nasm .. '" -f win64 -DWIN64 -D__x86_64__ '
            .. '-I"' .. turboSimdNasmDir .. '" '
            .. '-I"' .. turboSimdX64Dir .. '" '
            .. '"%{file.relpath}" -o "%{cfg.objdir}/%{file.basename}.obj"'
        }
        buildoutputs { "%{cfg.objdir}/%{file.basename}.obj" }

    filter { "system:windows", "architecture:x86", "files:simd/**.asm" }
        buildmessage "NASM %{file.relpath}"
        buildcommands {
            '"' .. nasm .. '" -f win32 -DWIN32 '
            .. '-I"' .. turboSimdNasmDir .. '" '
            .. '-I"' .. turboSimdX86Dir .. '" '
            .. '"%{file.relpath}" -o "%{cfg.objdir}/%{file.basename}.obj"'
        }
        buildoutputs { "%{cfg.objdir}/%{file.basename}.obj" }
