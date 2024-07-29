#pragma once

#include <glm/glm.hpp>

#include "Shapes.h"

namespace Math {
	bool Intersects(glm::vec2 circlePos, float circleRadius, glm::vec2 pos);
	bool Intersects(glm::vec2 rectPos, glm::vec2 rectSize, glm::vec2 pos);
	bool Intersects(Rectangle rect1, Rectangle rect2);
}