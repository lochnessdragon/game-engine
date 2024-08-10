#pragma once

#include <Windows.h>

#include <string>
#include <optional>

#include <Terminal/Private/ANSITerminal.h>

namespace Platform {
	namespace Windows {
		// enables UTF-8 and Virtual terminal sequences on windows, throws an exception in the case of failure
		class WindowsTerminal : public cli::Private::ANSITerminal {
		public:
			WindowsTerminal();
			~WindowsTerminal();

			// singleton pattern
			static WindowsTerminal* get() { return INSTANCE; }

			// disables echoing input and line buffering
			void rawInput();

			// enables echoing input and line buffering
			void cookedInput();
		private:
			UINT initialCinCodePage = 0;
			UINT initialCoutCodePage = 0;
			HANDLE stdOutHandle = INVALID_HANDLE_VALUE;
			HANDLE stdInHandle = INVALID_HANDLE_VALUE;
			std::optional<DWORD> originalStdOutMode = std::nullopt;
			std::optional<DWORD> originalStdInMode = std::nullopt;

			// singleton pattern
			static WindowsTerminal* INSTANCE;
		};

		std::string GetWindowsError();
		void PrintWindowsError();
	}
}