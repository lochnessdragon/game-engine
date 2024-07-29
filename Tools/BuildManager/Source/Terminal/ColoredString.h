#pragma once

#include <string>
#include <ostream>

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
class ColoredString {
public:
	ColoredString(std::string value) : Raw(value) {};

	ColoredString Fg(Color color) {
		Foreground = color;
		return *this; // we could alternatively return a new ColoredString on the stack
	}

	ColoredString Bg(Color color) {
		Background = color;
		return *this;
	}

	ColoredString Styled(Style style) {
		Style |= (uint8_t) style;
		return *this;
	}

	ColoredString Black() {
		return Fg(Color::Black);
	}

	ColoredString Red() {
		return Fg(Color::Red);
	}

	ColoredString Green() {
		return Fg(Color::Green);
	}

	ColoredString Yellow() {
		return Fg(Color::Yellow);
	}

	ColoredString Blue() {
		return Fg(Color::Blue);
	}

	ColoredString Magenta() {
		return Fg(Color::Magenta);
	}

	ColoredString Cyan() {
		return Fg(Color::Cyan);
	}

	ColoredString White() {
		return Fg(Color::White);
	}

	ColoredString Bold() {
		return Styled(Style::Bold);
	}

	ColoredString Swapped() {
		return Styled(Style::Swapped);
	}

	ColoredString Underline() {
		return Styled(Style::Underline);
	}

	std::string Render() const;

private:
	// represents the raw text
	std::string Raw;

	Color Foreground = Color::None;
	Color Background = Color::None;
	uint8_t Style = 0;
};

// operators on ColoredString
ColoredString operator""_c(const char* str, std::size_t len);
std::ostream& operator<<(std::ostream& os, ColoredString const& colored_string);