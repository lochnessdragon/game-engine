# extracts the glfw keys and converts it to valid c++
import sys
import re

digits = ["Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"]

def convert_key_name(name):
	try:
		digit = int(name)
		return digits[digit]
	except:
		return name.title().replace('_', '').replace('Kp', 'Keypad')

aliases = {
	"Space": "space",
	"Apostrophe": "'",
	"Comma": ",",
	"Minus": "-",
	"Period": ".",
	"Slash": "/",
	"Zero": "0", "One": "1", "Two": "2", "Three": "3", "Four": "4", 
	"Five": "5", "Six": "6", "Seven": "7", "Eight": "8", "Nine": "9",
	"Semicolon": ";",
	"Equal": "=",
	"A": "a", "B": "b", "C": "c", "D": "d", "E": "e", "F": "f", "G": "g", "H": "h",
	"I": "i", "J": "j", "K": "k", "L": "l", "M": "m", "N": "n", "O": "o", "P": "p",
	"Q": "q", "R": "r", "S": "s", "T": "t", "U": "u", "V": "v", "W": "w", "X": "x",
	"Y": "y", "Z": "z",
	"LeftBracket": "[",
	"RightBracket": "]",
	"Backslash": "\\\\",
	"GraveAccent": "`",
	"World1": "world_1", "World2": "world_2",
	"Escape": ["escape", "esc"],
	"Enter": "enter",
	"Tab": "tab",
	"Backspace": "backspace",
	"Insert": "insert",
	"Delete": "delete",
	"Right": [">", "right"], "Left": ["<", "left"], "Up": ["^", "up"], "Down": ["V", "down"],
	"PageUp": "page_up", "PageDown": "page_down",
	"Home": "home",
	"End": "end",
	"CapsLock": "caps_lock",
	"ScrollLock": "scoll_lock",
	"NumLock": "num_lock",
	"PrintScreen": "print_screen",
	"Pause": "pause",
	"F1": "f1", "F2": "f2", "F3": "f3", "F4": "f4", "F5": "f5", "F6": "f6", "F7": "f7", "F8": "f8", "F9": "f9", "F10": "f10", "F11": "f11", "F12": "f12",
	"F13": "f13", "F14": "f14", "F15": "f15", "F16": "f16", "F17": "f17", "F18": "f18", "F19": "f19", "F20": "f20", "F21": "f21", "F22": "f22", "F23": "f23", "F24": "f24", "F25": "f25",
	"Keypad0": "kp_0", "Keypad1": "kp_1", "Keypad2": "kp_2", "Keypad3": "kp_3", "Keypad4": "kp_4",
	"Keypad5": "kp_5", "Keypad6": "kp_6", "Keypad7": "kp_7", "Keypad8": "kp_8", "Keypad9": "kp_9",
	"KeypadDecimal": "kp_decimal", "KeypadDivide": "kp_divide", "KeypadMultiply": "kp_multiply", "KeypadSubtract": "kp_subtract",
	"KeypadAdd": "kp_add", "KeypadEnter": "kp_enter", "KeypadEqual": "kp_equal",
	"LeftShift": "left_shift",
	"LeftControl": "left_control",
	"LeftAlt": "left_alt",
	"LeftSuper": "left_super",
	"RightShift": "right_shift",
	"RightControl": "right_control",
	"RightAlt": "right_alt",
	"RightSuper": "right_super",
	"Menu": "menu"
}

def get_aliases(name):
	alias = aliases[name]
	if isinstance(alias, str):
		return [alias]

	return alias

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("You must include a filename in your call!")
		sys.exit(1)

	glfw_header_filename = sys.argv[1]

	keys_header = open("Keys.h", "wt")
	print("""#pragma once

#include <string>
#include <unordered_map>

#include <Graphics/GlApi.h>

class Key {
public:
	// extracted from glfw
	enum Value : int {""", file = keys_header)

	keys = []
	regex = re.compile(r"#define (GLFW_KEY_(\w*))")
	with open(glfw_header_filename) as file:
		for line in file:
			match = regex.search(line)
			if not match is None and not "LAST" in match.group():
				glfw_name = match.group(1) # including GLFW_KEY_
				key_name = convert_key_name(match.group(2)) # group 2 has no GLFW_KEY_ prefix
				keys.append(key_name)
				print(f"\t\t{key_name} = {glfw_name},", file = keys_header)

	print("""\t};

    Key() = default;
constexpr Key(Value key) : value(key) { }

	// allow switch and comparisons
	constexpr operator Value() const { return value; }
	// prevent usage if(key)
	explicit operator bool() const = delete;

	// returns the scancode of this key
	int Scancode() { glfwGetKeyScancode(value); }

	// returns a string representation of this key
	const char* ToString();

	// converts a string to a key value
	static Key FromString(const std::string& key);

private:
	Value value;
};""", file = keys_header)
	keys_header.close()

	keys_cpp = open("Keys.cpp", "wt")
	print("""#include \"Keys.h\"

#include <stdexcept>

std::unordered_map<std::string, Key> StrToKey = {""", file = keys_cpp)

	for key in keys:
		if not key == "Unknown":
			for alias in get_aliases(key):
				print(f"\t{{\"{alias}\", Key::{key}}},", file = keys_cpp)

	print("""};

Key Key::FromString(const std::string& key) {
	auto it = StrToKey.find(key);
	if (it != StrToKey.end()) {
		return it->second;
	}
	else { throw std::runtime_error("Cannot find a key mapping for: " + key); }
}
""", file = keys_cpp)

	print("const char* Key::ToString() {\n\tswitch(value) {", file = keys_cpp)
	for key in keys:
		print(f"\t{key}:\n\t\treturn \"[{key}]\";", file = keys_cpp)
	print("\tdefault:\n\t\treturn \"[Unknown Key]\"; // may want to refactor to return key scancode", file = keys_cpp)
	print("\t}\n}", file = keys_cpp)

	keys_cpp.close()