return function (source_dir)
	project "spdlog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	files {
		source_dir .. "/src/spdlog.cpp",
		source_dir .. "/src/stdout_sinks.cpp",
		source_dir .. "/src/color_sinks.cpp",
		source_dir .. "/src/file_sinks.cpp",
		source_dir .. "/src/async.cpp",
		source_dir .. "/src/cfg.cpp",
		source_dir .. "/include/**.h"
	}

	defines {
		"SPDLOG_COMPILED_LIB",
		"SPDLOG_USE_STD_FORMAT"
	}

	includedirs {
		source_dir .. "/include/"
	}
end