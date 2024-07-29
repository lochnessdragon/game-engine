#pragma once

#include <glm/vec2.hpp>

struct Rectangle {
	glm::vec2 TopLeft;
	glm::vec2 Size;

	// Constructors
	static Rectangle Centered(glm::vec2 pos, glm::vec2 scale) {
		return { glm::vec2(pos.x - scale.x, pos.y - scale.y), scale * 2.0f};
	}
	
	float Left() {
		return TopLeft.x;
	}

	float Right() {
		return TopLeft.x + Size.x;
	}

	float Top() {
		return TopLeft.y;
	}

	float Bottom() {
		return TopLeft.y + Size.y;
	}
};

struct Circle {
	glm::vec2 Position;
	float Radius;
};