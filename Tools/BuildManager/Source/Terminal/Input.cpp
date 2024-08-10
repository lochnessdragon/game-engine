#include "Input.h"

#include "Terminal.h"

std::string cli::prompt(const std::string& question, std::optional<std::string> defaultResponse) {
	std::cout << green("? ") << question << ": " << bright_black(defaultResponse.value_or("")); // press key => delete default response. delete key => input default response
	size_t defaultResponseLen = utf8::length(defaultResponse.value_or(""));
	Terminal::moveCursorLeft(defaultResponseLen);
	std::cout << "\x1b[33m"; // ANSI Escape sequence for yellow (typically gold?)
	//Terminal::get()->rawInput();
	std::string response;
	bool first = true;
	do {
		if (!first) {
			std::cout << std::endl << red("Please enter a value.") << std::endl << green("? ") << question << ": " << bright_black(defaultResponse.value_or(""));
			Terminal::moveCursorLeft(defaultResponseLen);
		}

		first = false;
		bool enter_pressed = false;
		while (!enter_pressed) {
			#ifdef _WIN32
			int c = getch();
			#endif
			if (c == '\x7f' || c == 8) {
				// DEL character
				if (response.length() > 0) {
					response.pop_back();
					Terminal::moveCursorLeft(1);
					Terminal::deleteCharacters(1);

					if (response.length() == 0) {
						// if deleting the last character move us to 0 length, display default value
						std::cout << bright_black(defaultResponse.value_or(""));
						Terminal::moveCursorLeft(defaultResponseLen);
						std::cout << "\x1b[33m";
					}
				}
			} 
			#if _WIN32
			else if (c == 0 || c == 0xe0) { // windows start of keycode
				c = getch();
			} 
			#endif
			else if (c == '\r' || c == '\n') {
				// enter pressed!
				enter_pressed = true;
				break;
			} else {
				// wipe the command line
				if (response.length() == 0) {
					std::cout << "\x1b[0K";
				}

				response += (char) c;
				std::cout << (char) c;
			}
		}
	} while (response.length() == 0 && !defaultResponse.has_value());

	if (response.length() == 0 && defaultResponse.has_value()) {
		std::cout << defaultResponse.value(); // display the default response as the chosen value
		response = defaultResponse.value();
	}
	//Terminal::get()->cookedInput();
	std::cout << "\x1b[m"; // ANSI Escape sequence reset
	std::cout << std::endl;

	return response;
}
