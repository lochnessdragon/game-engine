#pragma once

#include <memory>
#include <glm/glm.hpp>

#include <Graphics/Texture.h>
#include <Graphics/Spriteset.h>
#include "Actor.h"

class SpriteActor : public Actor {
public:
	// need texture, color
	glm::vec4 Color = { 1.0, 1.0, 1.0, 1.0 };
	glm::vec4 UVs = { 0.0, 0.0, 1.0, 1.0 };
	std::shared_ptr<Texture> BaseTexture = nullptr;

	SpriteActor(std::shared_ptr<Texture> texture);
	SpriteActor(glm::vec2 position = { 0.0f, 0.0f }, float rotation = 0.0f, glm::vec2 scale = { 1.0f, 1.0f }, std::shared_ptr<Texture> texture = nullptr, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec4 uvs = { 0.0, 0.0, 1.0, 1.0 });

	void Render(std::shared_ptr<Renderer2D> renderer) const;
};