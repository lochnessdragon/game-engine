import re 

colors = ["Black","Red","Green","Yellow","Blue","Magenta","Cyan","White","BrightBlack","BrightRed","BrightGreen","BrightYellow","BrightBlue","BrightMagenta","BrightCyan","BrightWhite"]

def PascalCase_to_snake_case(string):
	return re.sub('(?<!^)(?=[A-Z])', '_', string).lower()

if __name__ == "__main__":
	for color in colors:
		color_lower = PascalCase_to_snake_case(color)
		print(f"""\tColoredString {color_lower}() {{
		return fg(Color::{color});
	}}
""")

	for color in colors:
		color_lower = PascalCase_to_snake_case(color)
		print(f"""\tColoredString on_{color_lower}() {{
		return bg(Color::{color});
	}}
""")

	for color in colors:
		color_lower = PascalCase_to_snake_case(color)
		print(f"ColoredString {color_lower}(const std::string& str);")

	for color in colors:
		color_lower = PascalCase_to_snake_case(color)
		print(f"""ColoredString {color_lower}(const std::string& str) {{
	return ColoredString(str, Color::{color});
}}
""")