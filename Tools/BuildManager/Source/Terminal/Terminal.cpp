#include "Terminal.h"

#include <iostream>

#ifdef _WIN32
#include "Platform/Windows/WindowsTerminalGuard.h"
#endif

uint8_t Terminal::INSTANCE_COUNT = 0;

Terminal::Terminal() {
	if (INSTANCE_COUNT > 0) {
		// do nothing
		std::cerr << "You have created an extra Terminal instance! This is a program bug." << std::endl;
		return;
	}

	first = true;
	INSTANCE_COUNT++;

	// create worker thread
	worker.start();
}

Terminal::~Terminal() {
	if (!first) {
		return;
	}

	// send stop signal and join on worker thread
	worker.stop();

	INSTANCE_COUNT--;
}