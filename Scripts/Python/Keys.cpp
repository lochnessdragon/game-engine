#include "Keys.h"

#include <stdexcept>

std::unordered_map<std::string, Key> StrToKey = {
	{"space", Key::Space},
	{"'", Key::Apostrophe},
	{",", Key::Comma},
	{"-", Key::Minus},
	{".", Key::Period},
	{"/", Key::Slash},
	{"0", Key::Zero},
	{"1", Key::One},
	{"2", Key::Two},
	{"3", Key::Three},
	{"4", Key::Four},
	{"5", Key::Five},
	{"6", Key::Six},
	{"7", Key::Seven},
	{"8", Key::Eight},
	{"9", Key::Nine},
	{";", Key::Semicolon},
	{"=", Key::Equal},
	{"a", Key::A},
	{"b", Key::B},
	{"c", Key::C},
	{"d", Key::D},
	{"e", Key::E},
	{"f", Key::F},
	{"g", Key::G},
	{"h", Key::H},
	{"i", Key::I},
	{"j", Key::J},
	{"k", Key::K},
	{"l", Key::L},
	{"m", Key::M},
	{"n", Key::N},
	{"o", Key::O},
	{"p", Key::P},
	{"q", Key::Q},
	{"r", Key::R},
	{"s", Key::S},
	{"t", Key::T},
	{"u", Key::U},
	{"v", Key::V},
	{"w", Key::W},
	{"x", Key::X},
	{"y", Key::Y},
	{"z", Key::Z},
	{"[", Key::LeftBracket},
	{"\\", Key::Backslash},
	{"]", Key::RightBracket},
	{"`", Key::GraveAccent},
	{"world_1", Key::World1},
	{"world_2", Key::World2},
	{"escape", Key::Escape},
	{"esc", Key::Escape},
	{"enter", Key::Enter},
	{"tab", Key::Tab},
	{"backspace", Key::Backspace},
	{"insert", Key::Insert},
	{"delete", Key::Delete},
	{">", Key::Right},
	{"right", Key::Right},
	{"<", Key::Left},
	{"left", Key::Left},
	{"V", Key::Down},
	{"down", Key::Down},
	{"^", Key::Up},
	{"up", Key::Up},
	{"page_up", Key::PageUp},
	{"page_down", Key::PageDown},
	{"home", Key::Home},
	{"end", Key::End},
	{"caps_lock", Key::CapsLock},
	{"scoll_lock", Key::ScrollLock},
	{"num_lock", Key::NumLock},
	{"print_screen", Key::PrintScreen},
	{"pause", Key::Pause},
	{"f1", Key::F1},
	{"f2", Key::F2},
	{"f3", Key::F3},
	{"f4", Key::F4},
	{"f5", Key::F5},
	{"f6", Key::F6},
	{"f7", Key::F7},
	{"f8", Key::F8},
	{"f9", Key::F9},
	{"f10", Key::F10},
	{"f11", Key::F11},
	{"f12", Key::F12},
	{"f13", Key::F13},
	{"f14", Key::F14},
	{"f15", Key::F15},
	{"f16", Key::F16},
	{"f17", Key::F17},
	{"f18", Key::F18},
	{"f19", Key::F19},
	{"f20", Key::F20},
	{"f21", Key::F21},
	{"f22", Key::F22},
	{"f23", Key::F23},
	{"f24", Key::F24},
	{"f25", Key::F25},
	{"kp_0", Key::Keypad0},
	{"kp_1", Key::Keypad1},
	{"kp_2", Key::Keypad2},
	{"kp_3", Key::Keypad3},
	{"kp_4", Key::Keypad4},
	{"kp_5", Key::Keypad5},
	{"kp_6", Key::Keypad6},
	{"kp_7", Key::Keypad7},
	{"kp_8", Key::Keypad8},
	{"kp_9", Key::Keypad9},
	{"kp_decimal", Key::KeypadDecimal},
	{"kp_divide", Key::KeypadDivide},
	{"kp_multiply", Key::KeypadMultiply},
	{"kp_subtract", Key::KeypadSubtract},
	{"kp_add", Key::KeypadAdd},
	{"kp_enter", Key::KeypadEnter},
	{"kp_equal", Key::KeypadEqual},
	{"left_shift", Key::LeftShift},
	{"left_control", Key::LeftControl},
	{"left_alt", Key::LeftAlt},
	{"left_super", Key::LeftSuper},
	{"right_shift", Key::RightShift},
	{"right_control", Key::RightControl},
	{"right_alt", Key::RightAlt},
	{"right_super", Key::RightSuper},
	{"menu", Key::Menu},
};

Key Key::FromString(const std::string& key) {
	auto it = StrToKey.find(key);
	if (it != StrToKey.end()) {
		return it->second;
	}
	else { throw std::runtime_error("Cannot find a key mapping for: " + key); }
}

const char* Key::ToString() {
	switch(value) {
	Unknown:
		return "[Unknown]";
	Space:
		return "[Space]";
	Apostrophe:
		return "[Apostrophe]";
	Comma:
		return "[Comma]";
	Minus:
		return "[Minus]";
	Period:
		return "[Period]";
	Slash:
		return "[Slash]";
	Zero:
		return "[Zero]";
	One:
		return "[One]";
	Two:
		return "[Two]";
	Three:
		return "[Three]";
	Four:
		return "[Four]";
	Five:
		return "[Five]";
	Six:
		return "[Six]";
	Seven:
		return "[Seven]";
	Eight:
		return "[Eight]";
	Nine:
		return "[Nine]";
	Semicolon:
		return "[Semicolon]";
	Equal:
		return "[Equal]";
	A:
		return "[A]";
	B:
		return "[B]";
	C:
		return "[C]";
	D:
		return "[D]";
	E:
		return "[E]";
	F:
		return "[F]";
	G:
		return "[G]";
	H:
		return "[H]";
	I:
		return "[I]";
	J:
		return "[J]";
	K:
		return "[K]";
	L:
		return "[L]";
	M:
		return "[M]";
	N:
		return "[N]";
	O:
		return "[O]";
	P:
		return "[P]";
	Q:
		return "[Q]";
	R:
		return "[R]";
	S:
		return "[S]";
	T:
		return "[T]";
	U:
		return "[U]";
	V:
		return "[V]";
	W:
		return "[W]";
	X:
		return "[X]";
	Y:
		return "[Y]";
	Z:
		return "[Z]";
	LeftBracket:
		return "[LeftBracket]";
	Backslash:
		return "[Backslash]";
	RightBracket:
		return "[RightBracket]";
	GraveAccent:
		return "[GraveAccent]";
	World1:
		return "[World1]";
	World2:
		return "[World2]";
	Escape:
		return "[Escape]";
	Enter:
		return "[Enter]";
	Tab:
		return "[Tab]";
	Backspace:
		return "[Backspace]";
	Insert:
		return "[Insert]";
	Delete:
		return "[Delete]";
	Right:
		return "[Right]";
	Left:
		return "[Left]";
	Down:
		return "[Down]";
	Up:
		return "[Up]";
	PageUp:
		return "[PageUp]";
	PageDown:
		return "[PageDown]";
	Home:
		return "[Home]";
	End:
		return "[End]";
	CapsLock:
		return "[CapsLock]";
	ScrollLock:
		return "[ScrollLock]";
	NumLock:
		return "[NumLock]";
	PrintScreen:
		return "[PrintScreen]";
	Pause:
		return "[Pause]";
	F1:
		return "[F1]";
	F2:
		return "[F2]";
	F3:
		return "[F3]";
	F4:
		return "[F4]";
	F5:
		return "[F5]";
	F6:
		return "[F6]";
	F7:
		return "[F7]";
	F8:
		return "[F8]";
	F9:
		return "[F9]";
	F10:
		return "[F10]";
	F11:
		return "[F11]";
	F12:
		return "[F12]";
	F13:
		return "[F13]";
	F14:
		return "[F14]";
	F15:
		return "[F15]";
	F16:
		return "[F16]";
	F17:
		return "[F17]";
	F18:
		return "[F18]";
	F19:
		return "[F19]";
	F20:
		return "[F20]";
	F21:
		return "[F21]";
	F22:
		return "[F22]";
	F23:
		return "[F23]";
	F24:
		return "[F24]";
	F25:
		return "[F25]";
	Keypad0:
		return "[Keypad0]";
	Keypad1:
		return "[Keypad1]";
	Keypad2:
		return "[Keypad2]";
	Keypad3:
		return "[Keypad3]";
	Keypad4:
		return "[Keypad4]";
	Keypad5:
		return "[Keypad5]";
	Keypad6:
		return "[Keypad6]";
	Keypad7:
		return "[Keypad7]";
	Keypad8:
		return "[Keypad8]";
	Keypad9:
		return "[Keypad9]";
	KeypadDecimal:
		return "[KeypadDecimal]";
	KeypadDivide:
		return "[KeypadDivide]";
	KeypadMultiply:
		return "[KeypadMultiply]";
	KeypadSubtract:
		return "[KeypadSubtract]";
	KeypadAdd:
		return "[KeypadAdd]";
	KeypadEnter:
		return "[KeypadEnter]";
	KeypadEqual:
		return "[KeypadEqual]";
	LeftShift:
		return "[LeftShift]";
	LeftControl:
		return "[LeftControl]";
	LeftAlt:
		return "[LeftAlt]";
	LeftSuper:
		return "[LeftSuper]";
	RightShift:
		return "[RightShift]";
	RightControl:
		return "[RightControl]";
	RightAlt:
		return "[RightAlt]";
	RightSuper:
		return "[RightSuper]";
	Menu:
		return "[Menu]";
	default:
		return "[Unknown Key]"; // may want to refactor to return key scancode
	}
}
