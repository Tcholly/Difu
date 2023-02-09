workspace "Difu"
    language "C++"
    cppdialect "C++17"
    
    architecture "x86_64"
    configurations { "Debug", "Release" }

    warnings "Extra"

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        optimize "On"

    filter { }

    targetdir ("bin/%{prj.name}/%{cfg.longname}")
    objdir ("obj/%{prj.name}/%{cfg.longname}")

project "Difu"
    kind "StaticLib"
    files "Difu/**"

    includedirs {
		"Difu",
        "Dependencies/Raylib/%{cfg.system}/include",
        "Dependencies/fmt/%{cfg.system}/include"
    }

    libdirs { 
        "Dependencies/Raylib/%{cfg.system}/lib",
        "Dependencies/fmt/%{cfg.system}/lib"
    }

    
    links { "raylib", "fmt" } -- , "m", "dl", "rt", "X11"

	prebuildcommands {
		"{RMDIR} bin/%{prj.name}/%{cfg.longname}/include/"
	}

	postbuildcommands {
		"{MKDIR} bin/%{prj.name}/%{cfg.longname}/include",
		"{CHDIR} Difu/ && find . -name '*.h' -exec cp -f --parents '{}' ../bin/%{prj.name}/%{cfg.longname}/include ';'",
		-- "{COPYFILE} --parents **/*.h '../../bin/%{prj.name}/%{cfg.longname}/include/Difu/'"
	}
