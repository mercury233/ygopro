project "ImgBench"
    kind "ConsoleApp"
    language "C++"
    rtti "Off"
    openmp "On"

    files { "*.cpp", "*.h", "../../gframe/image_resizer.cpp" }
    includedirs { "../../gframe" }

    links { "irrlicht" }

    if BUILD_IRRLICHT then
        includedirs { "../../irrlicht/include" }
    else
        includedirs { IRRLICHT_INCLUDE_DIR }
        libdirs { IRRLICHT_LIB_DIR }
    end

    filter "system:windows"
        defines { "WINVER=0x0601", "_IRR_WCHAR_FILESYSTEM" }
        links { "ws2_32", "iphlpapi" }

    filter "not action:vs*"
        cppdialect "C++14"

    filter "system:macosx"
        openmp "Off"
        if MAC_ARM then
            linkoptions { "-arch arm64" }
        end
        if MAC_INTEL then
            linkoptions { "-arch x86_64" }
        end

    filter {}
