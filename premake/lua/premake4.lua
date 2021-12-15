project "lua"
    kind "StaticLib"

    files { "src/*.c", "src/*.h", "src/*.hpp" }
    removefiles { "src/lua.c", "src/luac.c" }

    configuration "vs*"	
        buildoptions { "/TP" }
