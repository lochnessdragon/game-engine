local function setup_openal(directory)
	local openal_soft_build_directory = _MAIN_SCRIPT_DIR .. "/External Projects/OpenAL/"
	-- generate the external project
	os.executef("cmake \"-B%s\" \"%s\"", openal_soft_build_directory, directory)

	-- add it
	externalproject "OpenAL"
		location (openal_soft_build_directory)
		uuid "87C9BFA6-39DA-38F7-98D8-194E76829B57"
		kind "SharedLib"
		language "C++"
end

return setup_openal