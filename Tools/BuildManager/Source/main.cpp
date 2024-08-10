#include <iostream>
#include <thread>
#include <filesystem>

#include <Terminal/Terminal.h>
#include <Terminal/ColoredString.h>
#include <Terminal/Spinner.h>
#include <Terminal/Input.h>
#include "io.h"

using namespace cli;

// bldr generate icons from "Assets/Icon" to "build/"
// bldr icons "Assets/Icon" "build/"
// bldr new [name]
//  - name

// cli ui:
//	1. colored strings
//	2. print message
//	3. ask user for input
//		a. string input (defaults)
//		b. number input (defaults)
//		c. multiselect input
//	4. spinner - \ | /
//	5. loading bar [#####......]

const std::string new_wares_code = "{\n\
	\"version\": 0,\n\
	\"dependencies\": [\n\
		{\n\
		\"name\": \"game-engine\",\n\
		\"type\" : \"path\",\n\
		\"path\" : \"../game-engine\"\n\
		}\n\
	]\n\
}";

void new_project(std::optional<const std::string&> project_name) {
	if (!project_name.has_value()) {
		project_name.emplace(cli::prompt("What should we call this creation", "The Dark Souls of Racing"));
	}

	std::cout << "Creating: " << green(project_name.value() + "/") << std::endl;

	std::filesystem::path root_dir = project_name.value();
	std::filesystem::create_directory(root_dir);
	
	// create wares.json
	io::write_file(root_dir / "wares.json", new_wares_code);

	// create premake5.lua
	io::write_file(root_dir / "premake5.lua", new_premake5_code);

	// create CMakeLists.txt

	// create Source directory
	std::filesystem::create_directory(root_dir / "Source");

	// create starter source files
}

class Task {
public:
	enum Value {
		NewProject,
		GenerateIcons
	};

	Task() = default;
	constexpr Task(Value task) : value(task) {};
	constexpr operator Value() const { return value; };
	explicit operator bool() const = delete;

	std::string toString() const {
		switch (value) {
		case NewProject:
			return "New Project";
		case GenerateIcons:
			return "Generate Icons";
		default:
			return "Programmer Error";
		}
	};

private:
	Value value;
};

// print usage information
void usage() {
	std::cout << "bldr new [name]" << std::endl;
}

int main(int argc, char* argv[]) {
	Terminal term; // when this object goes out of scope, UTF-8 and colored terminal support is reset

	if (argc < 2) {
		std::cout << red("More arguments required") << std::endl;
		usage();
		return 1;
	}

	if (argv[1] == "new") {
		if (argc > 2) {
			new_project(std::string(argv[2]));
		}
		else {
			new_project(std::nullopt);
		}
	}

	// std::string game_name = cli::prompt("What should we call this creation", "The Dark Souls of Racing");

	return 0;
}