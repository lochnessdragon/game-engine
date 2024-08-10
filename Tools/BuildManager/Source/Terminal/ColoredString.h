#pragma once

#include <string>
#include <ostream>
#include <vector>

enum class Color {
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magenta,
	Cyan,
	White,
	BrightBlack,
	BrightRed,
	BrightGreen,
	BrightYellow,
	BrightBlue,
	BrightMagenta,
	BrightCyan,
	BrightWhite,
	None
};

enum class Style : uint8_t {
	Bold = 0x1,
	Underline = 0x2,
	Swapped = 0x4
};

// it might make a lot more sense to match the c++ apis here with a colored_string class
class ColoredString : public std::string {
public:
	ColoredString(std::string value, Color foreground = Color::None, Color background = Color::None);

	ColoredString fg(Color color) {
		uniform_style();
		slices.at(0).foreground = color;
		return *this; // we could alternatively return a new ColoredString on the stack
	}

	ColoredString bg(Color color) {
		uniform_style();
		slices.at(0).background = color;
		return *this;
	}

	ColoredString styled(Style style) {
		uniform_style();
		slices.at(0).style_flags |= (uint8_t) style;
		return *this;
	}

	ColoredString black() {
		return fg(Color::Black);
	}

	ColoredString red() {
		return fg(Color::Red);
	}

	ColoredString green() {
		return fg(Color::Green);
	}

	ColoredString yellow() {
		return fg(Color::Yellow);
	}

	ColoredString blue() {
		return fg(Color::Blue);
	}

	ColoredString magenta() {
		return fg(Color::Magenta);
	}

	ColoredString cyan() {
		return fg(Color::Cyan);
	}

	ColoredString white() {
		return fg(Color::White);
	}

	ColoredString bright_black() {
		return fg(Color::BrightBlack);
	}

	ColoredString bright_red() {
		return fg(Color::BrightRed);
	}

	ColoredString bright_green() {
		return fg(Color::BrightGreen);
	}

	ColoredString bright_yellow() {
		return fg(Color::BrightYellow);
	}

	ColoredString bright_blue() {
		return fg(Color::BrightBlue);
	}

	ColoredString bright_magenta() {
		return fg(Color::BrightMagenta);
	}

	ColoredString bright_cyan() {
		return fg(Color::BrightCyan);
	}

	ColoredString bright_white() {
		return fg(Color::BrightWhite);
	}

	ColoredString on_black() {
		return bg(Color::Black);
	}

	ColoredString on_red() {
		return bg(Color::Red);
	}

	ColoredString on_green() {
		return bg(Color::Green);
	}

	ColoredString on_yellow() {
		return bg(Color::Yellow);
	}

	ColoredString on_blue() {
		return bg(Color::Blue);
	}

	ColoredString on_magenta() {
		return bg(Color::Magenta);
	}

	ColoredString on_cyan() {
		return bg(Color::Cyan);
	}

	ColoredString on_white() {
		return bg(Color::White);
	}

	ColoredString on_bright_black() {
		return bg(Color::BrightBlack);
	}

	ColoredString on_bright_red() {
		return bg(Color::BrightRed);
	}

	ColoredString on_bright_green() {
		return bg(Color::BrightGreen);
	}

	ColoredString on_bright_yellow() {
		return bg(Color::BrightYellow);
	}

	ColoredString on_bright_blue() {
		return bg(Color::BrightBlue);
	}

	ColoredString on_bright_magenta() {
		return bg(Color::BrightMagenta);
	}

	ColoredString on_bright_cyan() {
		return bg(Color::BrightCyan);
	}

	ColoredString on_bright_white() {
		return bg(Color::BrightWhite);
	}

	ColoredString bold() {
		return styled(Style::Bold);
	}

	ColoredString swapped() {
		return styled(Style::Swapped);
	}

	ColoredString underline() {
		return styled(Style::Underline);
	}

	std::string render() const;

	ColoredString operator+(const ColoredString& other);
private:
	struct StyledSlice {
		size_t start = 0;
		size_t end = 1;

		Color foreground = Color::None;
		Color background = Color::None;
		uint8_t style_flags = 0;

		StyledSlice right(size_t amount) const {
			return { start + amount, end + amount, foreground, background, style_flags };
		}
	};

	std::vector<StyledSlice> slices;

	// private ctor
	ColoredString(std::string value, std::vector<StyledSlice> slices) : std::string(value), slices(slices) {};

	void uniform_style() {
		if (slices.size() > 1)
			slices.clear();
		if (slices.size() == 0)
			slices.push_back(StyledSlice {});
		slices.at(0).end = size();
	}

	// map positions in the stream to foreground, background, style
	// have a list of ranges mapped to foreground, background, style
	// 
};

// operators on ColoredString
ColoredString operator""_c(const char* str, std::size_t len);
std::ostream& operator<<(std::ostream& os, ColoredString const& colored_string);

// helper functions to construct colored strings
ColoredString black(const std::string& str);
ColoredString red(const std::string& str);
ColoredString green(const std::string& str);
ColoredString yellow(const std::string& str);
ColoredString blue(const std::string& str);
ColoredString magenta(const std::string& str);
ColoredString cyan(const std::string& str);
ColoredString white(const std::string& str);
ColoredString bright_black(const std::string& str);
ColoredString bright_red(const std::string& str);
ColoredString bright_green(const std::string& str);
ColoredString bright_yellow(const std::string& str);
ColoredString bright_blue(const std::string& str);
ColoredString bright_magenta(const std::string& str);
ColoredString bright_cyan(const std::string& str);
ColoredString bright_white(const std::string& str);