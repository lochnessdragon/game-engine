#include "math/intersection.h"

bool Math::Intersects(glm::vec2 circlePos, float circleRadius, glm::vec2 pos) {
	return glm::length(circlePos - pos) < circleRadius;
}

/* intersects - returns true if the pos is within the vector defined by its center and size */
bool Math::Intersects(glm::vec2 rectPos, glm::vec2 rectSize, glm::vec2 pos) {
	glm::vec2 halfSize = rectSize / 2.0f;
	return pos.x > rectPos.x - halfSize.x && pos.x < rectPos.x + halfSize.x && pos.y > rectPos.y - halfSize.y && pos.y < rectPos.y + halfSize.y;
}

bool Math::Intersects(Rectangle rect1, Rectangle rect2) {
	return rect1.Left() <= rect2.Right() && rect1.Top() <= rect2.Bottom() && rect1.Right() >= rect2.Left() && rect1.Bottom() >= rect2.Top();
}
