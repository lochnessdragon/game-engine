#pragma once

#include <filesystem>
#include <string>

namespace io {
	void write_file(const std::filesystem::path& path, const std::string& contents);
}