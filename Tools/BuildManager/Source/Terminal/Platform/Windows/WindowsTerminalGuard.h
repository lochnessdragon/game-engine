#pragma once

#include <Windows.h>

#include <string>
#include <optional>

namespace Platform {
	namespace Windows {
		// enables UTF-8 and Virtual terminal sequences on windows, throws an exception in the case of failure
		class WindowsTerminalGuard {
		public:
			WindowsTerminalGuard();
			~WindowsTerminalGuard();
		private:
			UINT initialCinCodePage = 0;
			UINT initialCoutCodePage = 0;
			HANDLE stdOutHandle = INVALID_HANDLE_VALUE;
			HANDLE stdInHandle = INVALID_HANDLE_VALUE;
			std::optional<DWORD> originalStdOutMode = std::nullopt;
			std::optional<DWORD> originalStdInMode = std::nullopt;
		};

		std::string GetWindowsError();
		void PrintWindowsError();
	}
}