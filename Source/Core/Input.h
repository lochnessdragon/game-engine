#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <memory>

#include "Graphics/GlApi.h"
#include <glm/glm.hpp>

#include "EventDispatcher.h"

#include "Keys.h"
#include "Gamepad.h"

enum class InputType {
    Keyboard, // Mouse and Keyboard
    Controller
};

enum class ControllerType {
	Xbox,
	Playstation,
	Switch,
	Unknown
};

struct ButtonMapping {
	std::vector<Key> KeyboardMappings;
	std::vector<GamepadButton> GamepadMappings;
};

struct FloatMapping {
	static struct KeyboardMap {
		Key Positive;
		Key Negative;
	};

	static class GamepadAxis {
	public:
		enum Value {
			LeftStickLeftRight = 1,
			LeftStickUpDown = 2,
			RightStickLeftRight = 4,
			RightStickUpDown = 8,
			LeftTrigger = 16,
			RightTrigger = 32,
			DPadLeftRight = 64,
			DPadUpDown = 128
		};

		GamepadAxis() = default;
		constexpr GamepadAxis(Value axis) : value(axis) { }
		constexpr GamepadAxis(uint8_t axis) : value(static_cast<Value>(axis)) { }

		// allow switch and comparisons
		constexpr operator Value() const { return value; }
		// prevent usage if(key)
		explicit operator bool() const = delete;

		uint8_t GetGLFWEnum() {
			switch (value) {
			case LeftStickLeftRight:
				return GLFW_GAMEPAD_AXIS_LEFT_X;
			case LeftStickUpDown:
				return GLFW_GAMEPAD_AXIS_LEFT_Y;
			case RightStickLeftRight:
				return GLFW_GAMEPAD_AXIS_RIGHT_X;
			case RightStickUpDown:
				return GLFW_GAMEPAD_AXIS_RIGHT_Y;
			case LeftTrigger:
				return GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
			case RightTrigger:
				return GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
			default:
				throw std::runtime_error("GetGLFWEnum() called on a GamepadAxis value with no direct translation.");
			}
		}
	private:
		Value value;
	};

	std::vector<KeyboardMap> KeyboardMappings;
	unsigned char GamepadAxes = 0; // bitmap representing gamepad half-axes
};

struct Vector2Mapping {
	/* Internal type for Vector2Mapping
	*  Represents the keys that make up a singular axis mapping
	*/
	static struct KeyboardMap {
		Key PositiveAxis1;
		Key NegativeAxis1;
		Key PositiveAxis2;
		Key NegativeAxis2;
	};
	
	/* Internal type for Vector2Mapping
	*  Represents the gamepad parts that make up a singular axis mapping
	*/
	static enum class GamepadAxis {
		LeftStick = 1,
		RightStick = 2,
		DPad = 4
	};

	std::vector<KeyboardMap> KeyboardMappings;
	unsigned char GamepadAxes = 0; // bitmap representing gamepad axes
};

struct KeyEventData {
	int key;
	int scancode;
	int action;
	int mods;
};

struct MouseButtonEventData {
	int button;
	int action;
	int mods;
};

class Input {
private:
	GLFWwindow* handle;

	static Input* singleton;

	// Event dispatchers
	EventDispatcher<KeyEventData> keyEvent;
	EventDispatcher<glm::vec2> mouseMoveEvent;
	EventDispatcher<MouseButtonEventData> mouseButtonEvent;

	// mappings
	std::unordered_map<std::string, ButtonMapping> buttonMappings;
	std::unordered_map<std::string, FloatMapping> floatMappings;
	std::unordered_map<std::string, Vector2Mapping> vec2Mappings;

	static float StickAllowance; // maximum amount of axis movement before we actually accept it
public:
	Input(GLFWwindow* win);
	~Input() { if (singleton == this) { singleton = nullptr; } };

	// joystick functions
	static void JoystickUpdate(int jid, int event);
	void JoystickInfo();

	// keyboard functionss
	bool IsKeyPressed(Key key);
	
	// mouse functions
	// make the cursor disappear
	void DisableCursor();
	void ResetCursor();
	// get the position of the mouse as a vec2
	glm::vec2 GetMousePos();
	// returns true if a mouse button is currently pressed.
	// button is a GLFW enum.
	bool IsMouseButtonPressed(int button);
	
	void Map(const std::string& filename); // load input mappings
	bool GetInput(const std::string& name);
	float GetInputFloat(const std::string& name);
	glm::vec2 GetInputAxis(const std::string& name); // get vec2 mapping for input
	//static float getInput(const std::string& name); // getInput("move_z")

	// event dispatcher get functions
	EventDispatcher<KeyEventData>& GetKeyEventHandler() { return keyEvent; };
	EventDispatcher<glm::vec2>& GetMouseMoveEventHandler() { return mouseMoveEvent; };
	EventDispatcher<MouseButtonEventData>& GetMouseButtonEventHandler() { return mouseButtonEvent; };
	
	// needs to be called once per-frame
	// manages input polling and v-sync
	void Update();

	// singleton implementations.
	static Input* Get() { if (!singleton) { throw std::runtime_error("Singleton doesn't exist for input class"); }; return singleton; };

	// a singleton implementation of map().
	inline static void SMap(const std::string& filename) { Get()->Map(filename); };
	// a singleton implementation of getInput().
	inline static bool SGetInput(const std::string& name) { return Get()->GetInput(name); };
	// a singleton implementation of GetInputFloat
	inline static float SGetInputFloat(const std::string& name) { return Get()->GetInputFloat(name); };
	// a singleton implementation of getInputAxis().
	inline static glm::vec2 SGetInputAxis(const std::string& name) { return Get()->GetInputAxis(name); };
};
