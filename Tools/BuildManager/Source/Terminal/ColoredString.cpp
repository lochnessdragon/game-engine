#include "ColoredString.h"

#include <vector>

std::string ColoredString::Render() const {
	std::vector<char> sgr_codes;

	switch (Foreground) {
	case Color::Black:
		sgr_codes.push_back(30);
		break;
	case Color::Red:
		sgr_codes.push_back(31);
		break;
	case Color::Green:
		sgr_codes.push_back(32);
		break;
	case Color::Yellow:
		sgr_codes.push_back(33);
		break;
	case Color::Blue:
		sgr_codes.push_back(34);
		break;
	case Color::Magenta:
		sgr_codes.push_back(35);
		break;
	case Color::Cyan:
		sgr_codes.push_back(36);
		break;
	case Color::White:
		sgr_codes.push_back(37);
		break;
	case Color::BrightBlack:
		sgr_codes.push_back(90);
		break;
	case Color::BrightRed:
		sgr_codes.push_back(91);
		break;
	case Color::BrightGreen:
		sgr_codes.push_back(92);
		break;
	case Color::BrightYellow:
		sgr_codes.push_back(93);
		break;
	case Color::BrightBlue:
		sgr_codes.push_back(94);
		break;
	case Color::BrightMagenta:
		sgr_codes.push_back(95);
		break;
	case Color::BrightCyan:
		sgr_codes.push_back(96);
		break;
	case Color::BrightWhite:
		sgr_codes.push_back(97);
		break;
	default:
		break;
	}

	switch (Background) {
	case Color::Black:
		sgr_codes.push_back(40);
		break;
	case Color::Red:
		sgr_codes.push_back(41);
		break;
	case Color::Green:
		sgr_codes.push_back(42);
		break;
	case Color::Yellow:
		sgr_codes.push_back(43);
		break;
	case Color::Blue:
		sgr_codes.push_back(44);
		break;
	case Color::Magenta:
		sgr_codes.push_back(45);
		break;
	case Color::Cyan:
		sgr_codes.push_back(46);
		break;
	case Color::White:
		sgr_codes.push_back(47);
		break;
	case Color::BrightBlack:
		sgr_codes.push_back(100);
		break;
	case Color::BrightRed:
		sgr_codes.push_back(101);
		break;
	case Color::BrightGreen:
		sgr_codes.push_back(102);
		break;
	case Color::BrightYellow:
		sgr_codes.push_back(103);
		break;
	case Color::BrightBlue:
		sgr_codes.push_back(104);
		break;
	case Color::BrightMagenta:
		sgr_codes.push_back(105);
		break;
	case Color::BrightCyan:
		sgr_codes.push_back(106);
		break;
	case Color::BrightWhite:
		sgr_codes.push_back(107);
		break;
	default:
		break;
	}

	if (Style & (uint8_t) Style::Bold)
		sgr_codes.push_back(1);
	if (Style & (uint8_t) Style::Underline)
		sgr_codes.push_back(4);
	if (Style & (uint8_t) Style::Swapped)
		sgr_codes.push_back(7);
	
	std::string ansi_escape_seq = "\x1b[";
	for (std::size_t i = 0; i < sgr_codes.size(); i++) {
		ansi_escape_seq += std::to_string(sgr_codes[i]);
		if (i + 1 < sgr_codes.size())
			ansi_escape_seq += ";";
	}
	ansi_escape_seq += "m";
	
	return ansi_escape_seq + Raw + "\x1b[m";
}

// operators on ColoredString

ColoredString operator""_c(const char* str, std::size_t len) {
	return ColoredString(std::string(str, len));
}

std::ostream& operator<<(std::ostream& os, ColoredString const& colored_string) {
	return os << colored_string.Render();
}