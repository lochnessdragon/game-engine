#pragma once

#include <vector>
#include <string>

namespace cli {
	struct LoadingBarSettings {
		// for FiraCode: leftCapFrames = { "\uEE00", "\uEE03" };
		const std::vector<std::string> leftCapFrames = { "[" };
		// for FiraCode: rightCapFrames = { "\uEE03", "\uEE05" };
		const std::vector<std::string> rightCapFrames = { "]" };
		// for FiraCode: middleFrames = { "\uEE01", "\uEE04" };
		const std::vector<std::string> middleFrames = { ".", "#" };
	};

	class LoadingBar {
	public:
		LoadingBar();
	};
}