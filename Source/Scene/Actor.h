#pragma once

// System includes
#include <memory>

// Engine dependencies includes
#include <glm/glm.hpp>

// Engine includes
#include <Graphics/Renderer2d.h>

class Actor {
protected:
	glm::mat4 ModelMat = glm::identity<glm::mat4>();
public:
	glm::vec2 Position;
	float Rotation;
	glm::vec2 Scale;

	Actor(glm::vec2 position = { 0.0f, 0.0f }, float rotation = 0.0f, glm::vec2 scale = { 1.0f, 1.0f });

	void RecalculateModelMat() {
		ModelMat = glm::translate(glm::mat4(1.0f), glm::vec3(Position, 0.0f))
			* glm::rotate(glm::mat4(1.0f), Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(Scale, 1.0f));
	}

	glm::mat4 GetModelMat() { return ModelMat; };
};