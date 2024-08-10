#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <atomic>
#include <thread>

namespace cli {
	// start spinner
	// stop spinner ->  done, leave message
	//              \-> done, delete message
	// let's run the spinner on a separate thread, no coroutines necessary

	struct SpinnerSettings {
		const std::chrono::milliseconds interval = std::chrono::milliseconds(80); // 80 ms default between next frame
		// for FiraCode: frames = { "\uEE06", "\uEE07", "\uEE08", "\uEE09", "\uEE0A", "\uEE0B" };
		const std::vector<std::string> frames = { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" };

		const std::string message = "Loading...";
	};

	// Represents a handle to a spinner running on a separate thread
	class Spinner {
	public:
		Spinner(SpinnerSettings settings = {});
		~Spinner();

		void stop();
	private:
		std::thread runner;
		std::atomic_bool running;
	};

}