workspace "BuildManager"
	configurations { "Debug", "Release" }
	platforms { "x64" }

	targetdir "bin/%{cfg.buildcfg}-%{cfg.platform}"
	objdir "bin-obj/%{cfg.buildcfg}-%{cfg.platform}"

project "bldr"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	files {
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs {
		"Source/"
	}
	
	filter { "platforms:Win64" }
		architecture "x86_64"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"