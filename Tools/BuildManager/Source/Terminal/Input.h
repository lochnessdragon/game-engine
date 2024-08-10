#pragma once

#include <initializer_list>
#include <iostream>
#include <cstdint>
#include <optional>

#include "Terminal.h"
#include "ColoredString.h"
#include "Utf8.h"

#ifdef _WIN32
#include <conio.h>
#endif

namespace cli {

	// read a UTF-8 encoded character in a string from the command line.
	//std::string getChar();

	// prompt the user for a string input, can also accept a default response
	std::string prompt(const std::string& question, std::optional<std::string> defaultResponse);

	struct MultipleChoiceSettings {
		const std::string question = "Choose an option";
	};

	template <typename T>
	T multiple_choice(std::initializer_list<T> alternatives, MultipleChoiceSettings settings = {}) {
		ColoredString prompt = green("?") + ColoredString(" " + settings.question + ": ");
		std::cout << prompt;
		size_t prompt_len = utf8::length(prompt);
		std::string spacing(prompt_len, ' ');

		int i = 0;
		for (const T* elem = alternatives.begin(); elem < alternatives.end(); elem++) {
			if (i == 0) {
				std::cout << green("> ") << elem->toString() << std::endl; // first element highlighted
			} else {
				std::cout << "  " << elem->toString() << std::endl;
			} 

			if (elem + 1 != alternatives.end())
				std::cout << spacing;
			i++;
		}
		
		// reset cursor to prior to first green mark
		Terminal::moveCursorUp((int16_t)alternatives.size());
		Terminal::moveCursorRight((int16_t) prompt_len);

		size_t choice = 0;
		// disable cursor
		Terminal::disableCursor();
		#ifdef _WIN32
		// use windows getch()
		int input = '\0';
		do {
			input = getch();
			if (input == 0 || input == 0xE0) {
				input = getch();
				// see: https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa299374(v=vs.60)
				if (input == 72) { // up
					if (choice > 0) {
						choice--;
						// remove the previous icon
						std::cout << "  ";

						// add the icon to the correct line
						Terminal::moveCursorUp(1);
						Terminal::moveCursorRight((int16_t)prompt_len);
						std::cout << green("> ");
						Terminal::moveCursorLeft(2);
					}
				}
				else if (input == 80) { // down
					if (choice < (alternatives.size() - 1)) {
						choice++;

						// remove the previous icon
						std::cout << "  ";

						// add the icon to the correct line
						Terminal::moveCursorDown(1);
						Terminal::moveCursorRight((int16_t)prompt_len);
						std::cout << green("> ");
						Terminal::moveCursorLeft(2);
					}
				}
			}
		} while (input != '\r');
		#endif

		// for non-windows platforms:
		// up arrow: ESC [ A
		// down arrow: ESC [ B
		// enter: \n
		// don't forget to setup raw input mode (aka non-canonical)

		// remove the choices and display the one that was chosen
		// move down to the bottom of the selection
		Terminal::moveCursorDown((uint16_t) (alternatives.size() - (choice + 1)));
		for (size_t i = 0; i < (alternatives.size() - 1); i++) {
			std::cout << "\x1b[0K"; // replace this line with space characters
			Terminal::moveCursorUp(1);
		}
		// replace the first line with the user's answer
		Terminal::moveCursorRight((int16_t) prompt_len);
		std::cout << "\x1b[0K" << yellow((alternatives.begin() + choice)->toString()) << std::endl;

		//enable cursor
		Terminal::enableCursor();

		return *(alternatives.begin() + choice);
	};

	//void multiselect();
}