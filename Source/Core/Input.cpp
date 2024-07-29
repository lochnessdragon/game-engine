#include "input.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "Log.h"

Input* Input::singleton = nullptr;

float Input::StickAllowance = 0.05f;

Input::Input(GLFWwindow* window) {
	this->handle = window;
	glfwSetJoystickCallback(Input::JoystickUpdate);

	Input::singleton = this;
}

void Input::JoystickUpdate(int jid, int event) {
	const char* name = glfwGetJoystickName(jid);
	if (event == GLFW_CONNECTED)
	{
		// A joystick was connected
		Log::GetGameLog()->trace("Joystick #{}: {} was connected.", jid, name);
		if (glfwJoystickIsGamepad(jid) == GLFW_TRUE) {
			const char* gamepad_name = glfwGetGamepadName(jid);
			Log::GetGameLog()->trace("{} is gamepad \"{}\"", name, gamepad_name);
		}
	}
	else if (event == GLFW_DISCONNECTED)
	{
		// A joystick was disconnected
		Log::GetGameLog()->trace("Joystick #{}: {} was disconnected.", jid, name);
	}
}

void Input::JoystickInfo()
{
	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {
			const char* name = glfwGetJoystickName(i);
			Log::GetGameLog()->info("Joystick {} is present (name: {})", i, name);
			if (glfwJoystickIsGamepad(i))
			{
				const char* gamepadName = glfwGetGamepadName(i);
				Log::GetGameLog()->info("Joystick {} is also a gamepad named {}", i, gamepadName);
			}
		}
	}
}

bool Input::IsKeyPressed(Key key) {
	return glfwGetKey(handle, key) == GLFW_PRESS;
}

void Input::DisableCursor() {
	glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::ResetCursor() {
	glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

glm::vec2 Input::GetMousePos() {
	double x_pos, y_pos;
	glfwGetCursorPos(handle, &x_pos, &y_pos);
	return glm::vec2(x_pos, y_pos);
}

bool Input::IsMouseButtonPressed(int button) {
	return glfwGetMouseButton(handle, button) == GLFW_PRESS;
}

void Input::Map(const std::string& filename)
{
	try {
		std::fstream file(filename);
		nlohmann::json data = nlohmann::json::parse(file);
		file.close();

		if (!data.is_object()) {
			throw std::runtime_error("Root of mapping file is non-object type");
		}

		for (auto& [name, mapping] : data.items()) {
			Log::GetGameLog()->info("Processing Mapping: {}", name);
			auto type = mapping.at("type");
			if (type == "normalized_vec2") {
				Vector2Mapping vec2Mapping;
				
				// keyboard mappings
				for (auto key : mapping.at("keyboard")) {
					Vector2Mapping::KeyboardMap keyMapping;
					keyMapping.PositiveAxis1 = Key::FromString(key.at("axis_1").at("pos").template get<std::string>());
					keyMapping.NegativeAxis1 = Key::FromString(key.at("axis_1").at("neg").template get<std::string>());
					keyMapping.PositiveAxis2 = Key::FromString(key.at("axis_2").at("pos").template get<std::string>());
					keyMapping.NegativeAxis2 = Key::FromString(key.at("axis_2").at("neg").template get<std::string>());
					vec2Mapping.KeyboardMappings.push_back(keyMapping);
				}

				// gamepad mappings (consider moving this to a FromString method?)
				for (auto key : mapping.at("gamepad")) {
					std::string axisName = key.template get<std::string>();
					if (axisName == "left_axis") {
						vec2Mapping.GamepadAxes |= (unsigned char) Vector2Mapping::GamepadAxis::LeftStick;
					}
					else if (axisName == "right_axis") {
						vec2Mapping.GamepadAxes |= (unsigned char) Vector2Mapping::GamepadAxis::RightStick;
					}
					else if (axisName == "dpad") {
						vec2Mapping.GamepadAxes |= (unsigned char) Vector2Mapping::GamepadAxis::DPad;
					}
					else {
						throw std::runtime_error("Couldn't map gamepad axis from: " + axisName);
					}
				}

				this->vec2Mappings.emplace(name, vec2Mapping);
			}
			else if (type == "normalized_float") {
				FloatMapping floatMapping;

				for (auto key : mapping.at("keyboard")) {
					FloatMapping::KeyboardMap keyMapping;
					keyMapping.Positive = Key::FromString(key.at("pos").template get<std::string>());
					keyMapping.Negative = Key::FromString(key.at("neg").template get<std::string>());
					floatMapping.KeyboardMappings.push_back(keyMapping);
				}

				for (auto gamepad_axis : mapping.at("gamepad")) {
					std::string axisName = gamepad_axis.template get<std::string>();
					if (axisName == "left_axis_lr") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::LeftStickLeftRight;
					}
					else if (axisName == "left_axis_ud") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::LeftStickUpDown;
					}
					else if (axisName == "right_axis_lr") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::RightStickLeftRight;
					}
					else if (axisName == "right_axis_ud") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::RightStickUpDown;
					}
					else if (axisName == "left_trigger") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::LeftTrigger;
					}
					else if (axisName == "right_trigger") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::RightTrigger;
					}
					else if (axisName == "dpad_lr") {
						floatMapping.GamepadAxes |= (unsigned char) FloatMapping::GamepadAxis::DPadLeftRight;
					}
					else if (axisName == "dpad_ud") {
						floatMapping.GamepadAxes |= (unsigned char)FloatMapping::GamepadAxis::DPadUpDown;
					}
					else {
						throw std::runtime_error("Couldn't map gamepad axis from: " + axisName);
					}
				}

				this->floatMappings.emplace(name, floatMapping);
			}
			else if (type == "button") {
				ButtonMapping buttonMapping;

				for (auto key : mapping.at("keyboard")) {
					buttonMapping.KeyboardMappings.push_back(Key::FromString(key.template get<std::string>()));
				}

				for (auto gamepadBtn : mapping.at("gamepad")) {
					buttonMapping.GamepadMappings.push_back(GamepadButton::FromString(gamepadBtn.template get<std::string>()));
				}

				this->buttonMappings.emplace(name, buttonMapping);
			}
		}
	}
	catch (std::exception& e) {
		Log::GetGameLog()->error("Failed to load mappings from file: {}. ERROR: {}", filename, e.what());
	}
}

bool Input::GetInput(const std::string& name)
{
	ButtonMapping mapping = this->buttonMappings.at(name);

	// check keyboard first
	bool result = false;

	for (Key key : mapping.KeyboardMappings) {
		if (IsKeyPressed(key)) {
			result = true;
			break;
		}
	}

	if (!result) {
		for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
			if (result)
				break;
			if (glfwJoystickPresent(i)) {
				if (glfwJoystickIsGamepad(i)) {
					GLFWgamepadstate state;
					glfwGetGamepadState(i, &state);

					for (int gamepadKey : mapping.GamepadMappings) {
						result = state.buttons[gamepadKey];
						if (result)
							break;
					}
				}
			}
		}
	}

	return result;
}

float Input::GetInputFloat(const std::string& name) {
	FloatMapping mapping = this->floatMappings.at(name);

	// check keyboard first
	bool positive = false;
	bool negative = false;

	for (FloatMapping::KeyboardMap map : mapping.KeyboardMappings) {
		if (IsKeyPressed(map.Positive))
			positive = true;
		if (IsKeyPressed(map.Negative))
			negative = true;
	}

	if (positive || negative)
		return positive - negative;

	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
		if (glfwJoystickPresent(i)) {
			if (glfwJoystickIsGamepad(i)) {
				GLFWgamepadstate state;
				glfwGetGamepadState(i, &state);

				for (int bit = 0; bit < 8; bit++) {
					uint8_t axis_mask = 0x1 << bit;
					if (mapping.GamepadAxes & axis_mask) {
						FloatMapping::GamepadAxis axis = FloatMapping::GamepadAxis(axis_mask);
						if (static_cast<FloatMapping::GamepadAxis::Value>(axis) == FloatMapping::GamepadAxis::DPadLeftRight) {
							float value = 0.0f;
							bool updated = false;

							if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS) {
								value += 1.0f;
								updated = true;
							}
							
							if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS) {
								value -= 1.0f;
								updated = true;
							}

							if (updated)
								return value;
						}
						else if (static_cast<FloatMapping::GamepadAxis::Value>(axis) == FloatMapping::GamepadAxis::DPadUpDown) {
							float value = 0.0f;
							bool updated = false;

							if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS) {
								value += 1.0f;
								updated = true;
							}

							if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS) {
								value -= 1.0f;
								updated = true;
							}

							if (updated)
								return value;
						}
						else {
							if (abs(state.axes[axis.GetGLFWEnum()]) > StickAllowance)
								return state.axes[axis.GetGLFWEnum()];
						}
					}
				}
			}
		}
	}

	return 0;
}

glm::vec2 Input::GetInputAxis(const std::string& name)
{
	Vector2Mapping mapping = this->vec2Mappings.at(name);

	glm::vec2 input = glm::vec2(0.0f);
	
	// check keyboard first

	// this is really idiotic code and prime for refactoring
	bool posAxis1 = false;
	bool negAxis1 = false;
	bool posAxis2 = false;
	bool negAxis2 = false;

	for (Vector2Mapping::KeyboardMap keyMap : mapping.KeyboardMappings) {
		if (IsKeyPressed(keyMap.PositiveAxis1))
			posAxis1 = true;
		if (IsKeyPressed(keyMap.NegativeAxis1))
			negAxis1 = true;
		if (IsKeyPressed(keyMap.PositiveAxis2))
			posAxis2 = true;
		if (IsKeyPressed(keyMap.NegativeAxis2))
			negAxis2 = true;
	}

	if (posAxis1) {
		input.x += 1.0f;
	}
	if (negAxis1) {
		input.x -= 1.0f;
	}
	if (posAxis2) {
		input.y += 1.0f;
	}
	if (negAxis2) {
		input.y -= 1.0f;
	}

	// then check controller input (only if keyboard input is wack)
	if (!posAxis1 && !negAxis1 && !posAxis2 && !negAxis2) {
		bool updated = false;
		for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++) {
			if (updated)
				break;
			if (glfwJoystickPresent(i)) {
				if (glfwJoystickIsGamepad(i)) {
					GLFWgamepadstate state;
					glfwGetGamepadState(i, &state);
					if (mapping.GamepadAxes & (unsigned char) Vector2Mapping::GamepadAxis::LeftStick) {
						// check left axis
						float x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
						float y = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]; // invert
						
						if (abs(x) > Input::StickAllowance) {
							input.x = x;
							updated = true;
						}

						if (abs(y) > Input::StickAllowance) {
							input.y = y;
							updated = true;
						}
					}

					if (mapping.GamepadAxes & (unsigned char) Vector2Mapping::GamepadAxis::RightStick && !updated) {
						// check right axis
						float x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
						float y = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]; // invert

						if (abs(x) > Input::StickAllowance) {
							input.x = x;
							updated = true;
						}

						if (abs(y) > Input::StickAllowance) {
							input.y = y;
							updated = true;
						}
					}

					if (mapping.GamepadAxes & (unsigned char) Vector2Mapping::GamepadAxis::DPad && !updated) {
						// check all directions of the dpad
						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS) {
							input.x -= 1.0f;
							updated = true;
						}

						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS) {
							input.x += 1.0f;
							updated = true;
						}

						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS) {
							input.y -= 1.0f;
							updated = true;
						}

						if (state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS) {
							input.y += 1.0f;
							updated = true;
						}
					}
				}
			}
		}
	}

	return input;
}

void Input::Update() {
	// needs to happen every frame
	glfwPollEvents();
}
