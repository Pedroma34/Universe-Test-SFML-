-- premake5.lua
workspace "Universe Test" --Solution name
	architecture "x64"
	cppdialect "C++20"
	systemversion "latest" --Windows SDK version
    configurations { 
		"Debug", 
		"Release" 
	}
	

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" --Output directory

--Copy Data folder that is where solution is, into "bin/" .. outputdir .. "/%{prj.name}"
-- postbuildcommands { 
-- 	("{MKDIR} %{cfg.targetdir}/Data"),
-- 	("{COPY} %{wks.location}/Data %{cfg.targetdir}/Data") 
-- }

project "Universe Test"
	location "Universe Test"
	kind "ConsoleApp" --executable
	--kind "SharedLib" --dll
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/source/pch.cpp"

	files { 
		"%{prj.name}/source/**.h", 
		"%{prj.name}/source/**.cpp" 
	}

	includedirs {
		"%{prj.name}/source",
		"%{prj.name}/vendor/SFML/include",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/Imgui/include",
		"%{prj.name}/vendor/Imgui-sfml/include"
	}
	libdirs {
		"%{prj.name}/vendor/SFML/lib",
		"%{prj.name}/vendor/Imgui/lib",
		"%{prj.name}/vendor/Imgui-sfml/lib"
	}

	filter "configurations:Debug"
		staticruntime "on" 
		runtime "Debug" 
		buildoptions "/MTd" 
		defines { 
			"DEBUG",
			"SFML_STATIC"
		}
		links{
			"sfml-graphics-s-d.lib",
            "sfml-window-s-d.lib",
            "sfml-system-s-d.lib",
            "opengl32.lib",
            "winmm.lib",
			"freetype.lib",
            "gdi32.lib",
			"imgui.lib",
			"ImGui-SFML-d.lib"
		}
		symbols "On"

	filter "configurations:Release"
		staticruntime "on"
        runtime "Release" 
        buildoptions "/MT" 
		defines { 
			"NDEBUG",
			"SFML_STATIC"
		}
		links {
            "sfml-graphics-s.lib",
            "sfml-window-s.lib",
            "sfml-system-s.lib",
            "opengl32.lib",
            "winmm.lib",
			"freetype.lib",
			"gdi32.lib",
			"imgui.lib",
			"ImGui-SFML.lib"
        }
		optimize "On"