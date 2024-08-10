#include "Utf8.h"

size_t cli::utf8::length(const std::string& str) {
	size_t length = 0;
	for (size_t i = 0; i < str.size(); i++) {
		// count all bytes that don't start with 10
		length += (str[i] & 0xc0) != 0x80;
	}
	return length;
}