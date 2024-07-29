#pragma once

#include <string>

namespace Core::IO {
	// writes an array of characters to a file, specified by filename.
	// throws an exception if the file isn't found.
	void WriteFile(const std::string& filename, const char* data);

	// read a file's contents to a char array given a filename
	// throws an exception if the file isn't found.
	char* ReadFile(const std::string& filename);

	// read a file's contents to a string given a filename
	// throws an exception if the file isn't found.
	std::string ReadFileStr(const std::string& filename);

	// returns the file size of an std::ifstream
	std::streamsize GetFileSize(std::ifstream& file);
}