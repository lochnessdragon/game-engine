#include "Spinner.h"

#include <iostream>
#include <thread>

cli::Spinner::Spinner(cli::SpinnerSettings settings) {
	// send a request to the terminal rendering thread to create a spinner
	
	while (true) {
		for (int j = 0; j < settings.frames.size(); j++) {
			std::cout << settings.frames[j];
			// wait settings.interval
			std::this_thread::sleep_for(settings.interval);
			std::cout << "\r";
		}
	}
}

void cli::Spinner::stop() {

}