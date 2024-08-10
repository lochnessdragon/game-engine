return function(source_dir)
	project "imgui"
	kind "StaticLib"
	language "C++"

	files {
		source_dir .. "/imconfig.h",
		source_dir .. "/imgui.cpp",
		source_dir .. "/imgui.h",
		source_dir .. "/imgui_demo.cpp",
		source_dir .. "/imgui_internal.h",
		source_dir .. "/imgui_tables.cpp",
		source_dir .. "/imgui_widgets.cpp",
		source_dir .. "/backends/imgui_impl_glfw.cpp",
		source_dir .. "/backends/imgui_impl_glfw.h",
		source_dir .. "/backends/imgui_impl_opengl3.cpp",
		source_dir .. "/backends/imgui_impl_opengl3.h"
	}

	filter "action:vs*"
		files {
			source_dir .. "/misc/debuggers/imgui.natvis",
			source_dir .. "/misc/debuggers/imgui.natstepfilter"
		}

end