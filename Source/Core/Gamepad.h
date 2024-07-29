#pragma once

#include <string>

#include <Graphics/GlApi.h>

class GamepadButton {
public:
	// extracted from glfw
	enum Value {
		A = GLFW_GAMEPAD_BUTTON_A,
		B = GLFW_GAMEPAD_BUTTON_B,
		X = GLFW_GAMEPAD_BUTTON_X,
		Y = GLFW_GAMEPAD_BUTTON_Y,
		LeftBumper = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
		RightBumper = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		Back = GLFW_GAMEPAD_BUTTON_BACK,
		Start = GLFW_GAMEPAD_BUTTON_START,
		Guide = GLFW_GAMEPAD_BUTTON_GUIDE,
		LeftThumb = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
		RightThumb = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
		DPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
		DPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
		DPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
		DPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT
	};

	GamepadButton() = default;
	GamepadButton(Value button) : value(button) { }

	// allow switch and comparisons
	constexpr operator Value() const { return value; }
	// prevent usage if(button)
	explicit operator bool() const = delete;

	// returns a string representation of this key
	const char* ToString();

	// converts a string to a GamepadButton value
	static GamepadButton FromString(const std::string& name);
private:
	Value value;
};