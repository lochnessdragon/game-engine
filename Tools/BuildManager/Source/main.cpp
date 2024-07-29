#include <iostream>
#include <thread>

#include <Terminal/Terminal.h>
#include <Terminal/ColoredString.h>
#include <Terminal/Spinner.h>

using namespace cli;

// bldr generate icons from "Assets/Icon" to "build/"
// bldr icons "Assets/Icon" "build/"
int main(int argc, char* argv[]) {
	Terminal term; // when this object goes out of scope, UTF-8 and colored terminal support is reset

	std::cout << "bldr"_c.Green() << " generating iconset..." << std::endl;
	Spinner spinner;

	// also need to await here to return to the spinner
	// otherwise, we can just send the spinner to another thread
	std::this_thread::sleep_for(std::chrono::seconds(5));

	spinner.stop();

	return 0;
}