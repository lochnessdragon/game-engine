#pragma once

#include <vector>
#include <string>
#include <chrono>

namespace cli {
	// start spinner
	// stop spinner ->  done, leave message
	//              \-> done, delete message
	// let's run the spinner on a separate thread, no coroutines necessary

	struct SpinnerSettings {
		const std::chrono::milliseconds interval = std::chrono::milliseconds(80); // 80 ms default between next frame
		const std::vector<std::string> frames = { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" };
	};

	// Represents a handle to a spinner running on a separate thread
	class Spinner {
	public:
		Spinner(SpinnerSettings settings = {});
		~Spinner() { stop(); }

		void stop();
	};

}