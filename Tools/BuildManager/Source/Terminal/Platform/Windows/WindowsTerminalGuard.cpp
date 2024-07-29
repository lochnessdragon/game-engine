#include "WindowsTerminalGuard.h"

#include <iostream>

#include <stdexcept>

// used for UTF-16 to UTF-8 conversion
#include <locale>
#include <codecvt>

Platform::Windows::WindowsTerminalGuard::WindowsTerminalGuard() {
	// enable UTF8 support (if not enabled already)
	initialCinCodePage = GetConsoleCP();
	if (initialCinCodePage == 0) { // call failed
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	if (initialCinCodePage != CP_UTF8) {
		if (!SetConsoleCP(CP_UTF8)) {
			throw std::runtime_error("Terminal Error: " + GetWindowsError());
		}
	}

	initialCoutCodePage = GetConsoleOutputCP();
	if (initialCoutCodePage == 0) { // call failed 
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	if (initialCoutCodePage != CP_UTF8) {
		if (!SetConsoleOutputCP(CP_UTF8)) {
			throw std::runtime_error("Terminal Error: " + GetWindowsError());
		}
	}

	// enable virtual terminal sequences
	stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOutHandle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
	if (stdInHandle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	DWORD currentStdOutMode = 0;
	DWORD currentStdInMode = 0;

	if (!GetConsoleMode(stdOutHandle, &currentStdOutMode)) {
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	if (!GetConsoleMode(stdInHandle, &currentStdInMode)) {
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	DWORD requestedStdOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	DWORD requestedStdInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

	DWORD stdOutMode = currentStdOutMode | requestedStdOutModes;
	if (!SetConsoleMode(stdOutHandle, stdOutMode))
	{
		// we failed to set both modes, try to step down mode gracefully.
		requestedStdOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		stdOutMode = currentStdOutMode | requestedStdOutModes;
		if (!SetConsoleMode(stdOutHandle, stdOutMode))
		{
			// Failed to set any VT mode, can't do anything here.
			throw std::runtime_error("Terminal Error: " + GetWindowsError());
		}
	}

	originalStdOutMode = currentStdOutMode;

	DWORD stdInMode = currentStdInMode | requestedStdInModes;
	if (!SetConsoleMode(stdInHandle, stdInMode))
	{
		// Failed to set VT input mode, can't do anything here.
		throw std::runtime_error("Terminal Error: " + GetWindowsError());
	}

	originalStdInMode = currentStdInMode;
}

Platform::Windows::WindowsTerminalGuard::~WindowsTerminalGuard() {
	if (initialCinCodePage != 0) {
		SetConsoleCP(initialCinCodePage);
	}

	if (initialCoutCodePage != 0) {
		SetConsoleOutputCP(initialCoutCodePage);
	}

	if (stdOutHandle != INVALID_HANDLE_VALUE && originalStdOutMode.has_value()) {
		SetConsoleMode(stdOutHandle, originalStdOutMode.value());
	}

	if (stdInHandle != INVALID_HANDLE_VALUE && originalStdInMode.has_value()) {
		SetConsoleMode(stdInHandle, originalStdInMode.value());
	}
}

std::string Platform::Windows::GetWindowsError() {
	DWORD error = GetLastError();
	LPTSTR errorText = NULL;
	FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note

	std::string errorStr = "";

	if (NULL != errorText) {
		#ifdef UNICODE
		errorStr = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.to_bytes(errorText); // deprecated, but no replacement yet.
		#else
		errorStr = errorText;
		#endif

		// release memory allocated by FormatMessage()
		LocalFree(errorText);
		errorText = NULL;
	}

	return errorStr;
}

void Platform::Windows::PrintWindowsError() {
	std::cerr << GetWindowsError() << std::endl;
}