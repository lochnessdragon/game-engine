#include "io.h"

#include <fstream>

void io::write_file(const std::filesystem::path& path, const std::string& contents) {
	std::ofstream file(path);
	if (file.is_open()) {
		file.write(contents.c_str(), contents.size());
	}
}