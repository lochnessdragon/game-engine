local function setup_glfw(root_directory)
	local glfw_file = function (filename)
		return root_directory .. "/" .. filename
	end

	project "GLFW"

	files {
		glfw_file("src/internal.h"),
		glfw_file("src/platform.h"),
		glfw_file("src/mappings.h"),
		glfw_file("src/context.c"),
		glfw_file("src/init.c"),
		glfw_file("src/input.c"),
		glfw_file("src/monitor.c"),
		glfw_file("src/platform.c"),
		glfw_file("src/vulkan.c"),
		glfw_file("src/window.c"),
		glfw_file("src/egl_context.c"),
		glfw_file("src/osmesa_context.c"),
		glfw_file("src/null_platform.h"),
		glfw_file("src/null_joystick.h"),
		glfw_file("src/null_init.h"),
		glfw_file("src/null_monitor.h"),
		glfw_file("src/null_window.h"),
		glfw_file("src/null_joystick.h"),
	}

	includedirs {
		glfw_file("include/")
	}

	filter "system:windows"
		files {
			glfw_file("src/win32_time.h"),
			glfw_file("src/win32_thread.h"),
			glfw_file("src/win32_module.c"),
			glfw_file("src/win32_time.c"),
			glfw_file("src/win32_thread.c"),
			glfw_file("src/win32_platform.h"),
			glfw_file("src/win32_joystick.h"),
			glfw_file("src/win32_init.h"),
			glfw_file("src/win32_joystick.c"),
			glfw_file("src/win32_monitor.c"),
			glfw_file("src/win32_window.c"),
			glfw_file("src/wgl_context.c")
		}

		defines {
			"_GLFW_WIN32"
		}

end

return setup_glfw