#pragma once

#include <thread>
#include <atomic>

namespace cli {
	namespace Private {
		class CliWorker {
		public:
			void start();
			void loop();
			void stop();

		private:
			std::thread thread;
			std::atomic_bool running;
		};
	}
}