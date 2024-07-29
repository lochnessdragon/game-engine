#include "Gamepad.h"

#include <unordered_map>
#include <stdexcept>

std::unordered_map<std::string, GamepadButton> StrToGamepadButton = {{"a", GamepadButton::A},
	{"b", GamepadButton::B},
	{"x", GamepadButton::X},
	{"y", GamepadButton::Y},
	{"left_bumper", GamepadButton::LeftBumper},
	{"right_bumper", GamepadButton::RightBumper},
	{"back", GamepadButton::Back},
	{"start", GamepadButton::Start},
	{"guide", GamepadButton::Guide},
	{"left_thumb", GamepadButton::LeftThumb},
	{"right_thumb", GamepadButton::RightThumb},
	{"dpad_up", GamepadButton::DPadUp},
	{"dpad_right", GamepadButton::DPadRight},
	{"dpad_down", GamepadButton::DPadDown}, 
	{"dpad_left", GamepadButton::DPadLeft}};

GamepadButton GamepadButton::FromString(const std::string& name) {
	auto it = StrToGamepadButton.find(name);
	if (it != StrToGamepadButton.end()) {
		return it->second;
	}
	else { throw std::runtime_error("Cannot find a key mapping for: " + name); }
}

const char* GamepadButtonToStr[] = {"A", "B", "X", "Y", "LeftBumper", "RightBumper", "Back", "Start", "Guide", "LeftThumb", "RightThumb", "DPadUp", "DPadRight", "DPadDown", "DPadLeft"};

static_assert(sizeof(GamepadButtonToStr) / sizeof(char*) == (GamepadButton::DPadLeft + 1), "number of gamepad button strings doesn't match the number of gamepad button values");

const char* GamepadButton::ToString() {
	return GamepadButtonToStr[value];
}