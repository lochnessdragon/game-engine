standalone = false -- if we are included by an external project, we should already be in a workspace.

function setup_workspace()
	workspace "game-engine"
		configurations { "Debug", "Release" }
	
		filter "system:Windows"
			platforms { "x64" }-- , "uwp", "playstation", "switch", "web" } -- regular (win32) and uwp (xbox) building allowed on windows

		filter "system:Mac"
			platforms { "universal", "playstation", "switch", "web" }

		filter "system:Unix"
			platforms { "x64", "ARM64", "playstation", "switch", "web" }

		filter { "platforms:x64" }
			architecture "x86_64"
end

if _MAIN_SCRIPT == path.getabsolute("./premake5.lua") then
	-- if not included as an external library
	standalone = true
	setup_workspace()
end

-- these two _need_ to happen inside a workspace
wares = require "wares"  -- don't forget to change to get_wares
-- only include dev dependencies if we are building standalone
-- don't include tinygltf
-- todo: add emscripten support
deps = wares.sync({["dev-dependencies"]=standalone, ["desktop-dependencies"]=true}, {"tinygltf"})

-- only for desktop, with Emscripten, -sUSE_GLFW3=TRUE
setup_glfw = include("Scripts/Premake/setup_glfw.lua")
setup_glfw(deps["glfw"])

setup_spdlog = include("Scripts/Premake/setup_spdlog.lua")
setup_spdlog(deps["spdlog"])

setup_openal = include("Scripts/Premake/setup_openal.lua")
setup_openal(deps["openal-soft"])

setup_imgui = include("Scripts/Premake/setup_imgui.lua")
setup_imgui(deps["imgui"])

--setup_freetype = include("Scripts/Premake/setup_freetype.lua")
--setup_freetype(deps["freetype"]) move to a msdf atlas gen

setup_msdf_atlas_gen = include("Scripts/Premake/setup_msdf_atlas_gen.lua")
setup_msdf_atlas_gen(deps["msdf-atlas-gen"])

include ("External Libraries/glad")

project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	files { 
		"Source/**.h", 
		"Source/**.cpp" 
	}

	links {
		"spdlog",
		"GLFW",
		"OpenAL",
		"Glad",
		"imgui",
		"msdf-atlas-gen"
	}

	includedirs {
		"Source/",
	}

	externalincludedirs { 
		deps["spdlog"] .. "/include/",
		deps["glfw"] .. "/include/",
		deps["openal-soft"] .. "/include/",
		deps["glm"], -- header only
		deps["stb"], -- header only
		deps["json"] .. "/include/", -- header only
		"External Libraries/par_shapes/", -- header only
		"External Libraries/glad/include/",
		deps["imgui"]
	}

	defines { "GLM_FORCE_INTRINSICS" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"