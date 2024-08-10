#pragma once

#include <cstdint>

namespace cli {
	namespace Private {
		class ANSITerminal {
		public:
			// -- CURSOR METHODS --
			
			// enable the blinking cursor
			static void enableCursor();

			// disable the blinking cursor
			static void disableCursor();

			// move the cursor up by <x> lines
			static void moveCursorUp(int16_t lines);

			// move the cursor down by <x> lines
			static void moveCursorDown(int16_t lines);

			// move the cursor right by <x> columns
			static void moveCursorRight(int16_t lines);

			// move the cursor left by <x> columns
			static void moveCursorLeft(int16_t lines);

			// delete n characters
			static void deleteCharacters(int16_t chars);
		};
	}
}