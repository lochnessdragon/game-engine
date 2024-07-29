#include "Scene/SpriteActor.h"

SpriteActor::SpriteActor(std::shared_ptr<Texture> texture) : Actor({ 0.0f, 0.0f }, 0.0f, { texture->Width() / 2.0f, texture->Height() / 2.0f }), BaseTexture(texture) {}

SpriteActor::SpriteActor(glm::vec2 position, float rotation, glm::vec2 scale, std::shared_ptr<Texture> texture, glm::vec4 color, glm::vec4 uvs) : Actor(position, rotation, scale), BaseTexture(texture), Color(color), UVs(uvs) {
	if (scale == glm::vec2(1.0f, 1.0f) && texture != nullptr) {
		scale = glm::vec2(texture->Width() / 2.0f, texture->Height() / 2.0f);
	}
}

void SpriteActor::Render(std::shared_ptr<Renderer2D> renderer) const {
	renderer->RenderQuad(this->ModelMat, BaseTexture, UVs, Color);
}