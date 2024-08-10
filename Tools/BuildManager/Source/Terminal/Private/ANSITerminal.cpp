#include "ANSITerminal.h"

#include <iostream>
#include <string>

using namespace cli::Private;

void ANSITerminal::enableCursor() {
	std::cout << "\x1b[?25h";
}

void cli::Private::ANSITerminal::disableCursor() {
	std::cout << "\x1b[?25l";
}

void cli::Private::ANSITerminal::moveCursorUp(int16_t lines) {
	if (lines < 1) { return; }
	std::cout << "\x1b[" + std::to_string(lines) + "F";
}

void cli::Private::ANSITerminal::moveCursorDown(int16_t lines) {
	if (lines < 1) { return; }
	std::cout << "\x1b[" + std::to_string(lines) + "E";
}

void cli::Private::ANSITerminal::moveCursorRight(int16_t columns) {
	if (columns < 1) { return; }
	std::cout << "\x1b[" + std::to_string(columns) + "C";
}

void cli::Private::ANSITerminal::moveCursorLeft(int16_t columns) {
	if (columns < 1) { return; }
	std::cout << "\x1b[" + std::to_string(columns) + "D";
}

void cli::Private::ANSITerminal::deleteCharacters(int16_t chars) {
	if (chars < 1) { return; }
	std::cout << "\x1b[" << std::to_string(chars) << "P";
}
