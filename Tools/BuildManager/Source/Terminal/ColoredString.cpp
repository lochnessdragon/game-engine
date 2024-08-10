#include "ColoredString.h"

#include <vector>
#include <iostream>
#include <unordered_map> 
#include <optional>
#include <algorithm>

ColoredString::ColoredString(std::string value, Color foreground, Color background) : std::string(value), slices() {
	slices.push_back(StyledSlice { 0, value.size(), foreground, background });
}

std::unordered_map<Color, char> colorToForeground = { { Color::Black, 30 }, { Color::Red, 31 }, { Color::Green, 32 }, { Color::Yellow, 33 }, 
													  { Color::Blue, 34}, { Color::Magenta, 35 }, { Color::Cyan, 36 }, { Color::White, 37 },
													  { Color::BrightBlack, 90}, { Color::BrightRed, 91}, { Color::BrightGreen, 92 }, { Color::BrightYellow, 93 }, 
													  { Color::BrightBlue, 94 }, { Color::BrightMagenta, 95 }, { Color::BrightCyan, 96 }, { Color::BrightWhite, 97 } };

std::optional<char> getForegroundCode(Color color) {
	if (colorToForeground.contains(color)) {
		return colorToForeground[color];
	} else {
		return std::nullopt;
	}
}

std::optional<char> getBackgroundCode(Color color) {
	if (colorToForeground.contains(color)) {
		return colorToForeground[color] + 10;
	}
	else {
		return std::nullopt;
	}
}

std::string ColoredString::render() const {
	std::string result = "";

	size_t current_pos = 0;
	for (const StyledSlice& slice : slices) {
		if (slice.start > current_pos) {
			result += substr(current_pos, slice.start - current_pos);
			current_pos = slice.start;
		}

		std::vector<char> sgr_codes;
		std::optional<char> foreground = getForegroundCode(slice.foreground);
		if (foreground.has_value()) {
			sgr_codes.push_back(foreground.value());
		}
		std::optional<char> background = getForegroundCode(slice.background);
		if (background.has_value()) {
			sgr_codes.push_back(background.value());
		}

		if (slice.style_flags & (uint8_t)Style::Bold)
			sgr_codes.push_back(1);
		if (slice.style_flags & (uint8_t)Style::Underline)
			sgr_codes.push_back(4);
		if (slice.style_flags & (uint8_t)Style::Swapped)
			sgr_codes.push_back(7);

		std::string ansi_escape_seq = "\x1b[";
		for (std::size_t i = 0; i < sgr_codes.size(); i++) {
			ansi_escape_seq += std::to_string(sgr_codes[i]);
			if (i + 1 < sgr_codes.size())
				ansi_escape_seq += ";";
		}
		ansi_escape_seq += "m";

		result += ansi_escape_seq + substr(slice.start, slice.end - slice.start) + "\x1b[m";
		current_pos = slice.end;
	}

	size_t end_pos = slices[slices.size() - 1].end;
	if (end_pos < size()) {
		result += substr(end_pos); // add rest of string
	}
	
	return result;
}

ColoredString ColoredString::operator+(const ColoredString& other) {
	std::vector<StyledSlice> new_slices = slices;

	for (const StyledSlice& other_slice : other.slices) {
		new_slices.push_back(other_slice.right(size()));
	}

	return ColoredString(((std::string&)*this) + (const std::string&)other, new_slices);
}

// operators on ColoredString

ColoredString operator""_c(const char* str, std::size_t len) {
	return ColoredString(std::string(str, len));
}

std::ostream& operator<<(std::ostream& os, ColoredString const& colored_string) {
	if (std::cout.rdbuf() == os.rdbuf()) {
		return os << colored_string.render(); // render to cout
	} else {
		return os << (std::string) colored_string; // don't render any colors
	}
}

ColoredString black(const std::string& str) {
	return ColoredString(str, Color::Black);
}

ColoredString red(const std::string& str) {
	return ColoredString(str, Color::Red);
}

ColoredString green(const std::string& str) {
	return ColoredString(str, Color::Green);
}

ColoredString yellow(const std::string& str) {
	return ColoredString(str, Color::Yellow);
}

ColoredString blue(const std::string& str) {
	return ColoredString(str, Color::Blue);
}

ColoredString magenta(const std::string& str) {
	return ColoredString(str, Color::Magenta);
}

ColoredString cyan(const std::string& str) {
	return ColoredString(str, Color::Cyan);
}

ColoredString white(const std::string& str) {
	return ColoredString(str, Color::White);
}

ColoredString bright_black(const std::string& str) {
	return ColoredString(str, Color::BrightBlack);
}

ColoredString bright_red(const std::string& str) {
	return ColoredString(str, Color::BrightRed);
}

ColoredString bright_green(const std::string& str) {
	return ColoredString(str, Color::BrightGreen);
}

ColoredString bright_yellow(const std::string& str) {
	return ColoredString(str, Color::BrightYellow);
}

ColoredString bright_blue(const std::string& str) {
	return ColoredString(str, Color::BrightBlue);
}

ColoredString bright_magenta(const std::string& str) {
	return ColoredString(str, Color::BrightMagenta);
}

ColoredString bright_cyan(const std::string& str) {
	return ColoredString(str, Color::BrightCyan);
}

ColoredString bright_white(const std::string& str) {
	return ColoredString(str, Color::BrightWhite);
}