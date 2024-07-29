#include "CliWorker.h"

#include "../Platform/Windows/WindowsTerminalGuard.h"

void cli::Private::CliWorker::start() {
	running.store(true);
	thread = std::thread([this]() { loop(); });
}

void cli::Private::CliWorker::stop() {
	running.store(false);
	if (thread.joinable())
		thread.join();
}

void cli::Private::CliWorker::loop() {
	#ifdef _WIN32
	Platform::Windows::WindowsTerminalGuard win32Guard;
	#endif
	while (running.load()) {

	}
}