#include "scene/actor.h"

Actor::Actor(glm::vec2 position, float rotation, glm::vec2 scale) : Position(position), Rotation(rotation), Scale(scale) {
	RecalculateModelMat();
}