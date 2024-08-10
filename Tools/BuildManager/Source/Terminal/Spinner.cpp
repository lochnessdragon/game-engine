#include "Spinner.h"

#include <iostream>

#include "Terminal.h"
#include "ColoredString.h"

cli::Spinner::Spinner(cli::SpinnerSettings settings) {
	// send a request to the terminal rendering thread to create a spinner
	running.store(true);
	runner = std::thread([this, settings]() {
		// disable cursor
		Terminal::disableCursor();
		std::cout << settings.frames[0] << " " << settings.message << "\r";
		std::cout.flush();
		while (running.load()) {
			for (int j = 0; j < settings.frames.size(); j++) {
				std::cout << settings.frames[j];
				std::cout.flush();
				// wait settings.interval
				std::this_thread::sleep_for(settings.interval);
				std::cout << "\r";
			}
		}
		std::cout << green("✓") << std::endl; // ✔️ or ❌
		//enable cursor
		Terminal::enableCursor();
	});
}

cli::Spinner::~Spinner() {
	if (running.load()) { 
		stop(); 
	}
}

void cli::Spinner::stop() {
	running.store(false);
	if (runner.joinable()) {
		runner.join();
	}
}