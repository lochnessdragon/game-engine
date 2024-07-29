#pragma once

#include <memory>
#include <Graphics/Texture.h>

class Spriteset {
private:
	int TilesPerRow;
	int TilesPerCol;
public:
	std::shared_ptr<Texture> BaseTexture;
	int TileSize;

	Spriteset(std::shared_ptr<Texture> texture, int tileSize);

	glm::vec4 GetUVCoords(int tileId);
	uint32_t GetMaxId() { return TilesPerRow * TilesPerCol; };
};