#include "Spriteset.h"

#include <cmath>

#include <Core/Log.h>
#include <glm/gtx/string_cast.hpp>

Spriteset::Spriteset(std::shared_ptr<Texture> texture, int tileSize) : BaseTexture(texture), TileSize(tileSize) {
	glm::ivec2 texSize = texture->GetSize();
	TilesPerRow = texSize.x / tileSize;
	TilesPerCol = texSize.y / tileSize;
	Log::GetRendererLog()->info("Loaded {} x {} tileset.", TilesPerRow, TilesPerCol);
}

glm::vec4 Spriteset::GetUVCoords(int tileId) {
	glm::ivec2 texSize = BaseTexture->GetSize();
	glm::vec4 uvs;

	uvs.x = (float)  (tileId % TilesPerRow) / (float)TilesPerRow;
	// define it backwards, because the top coord of the y is technically w
	uvs.w = 1.0f - (floor((float) tileId / (float) TilesPerRow) / (float) TilesPerCol);
	uvs.y = uvs.w - ((float)TileSize / (float)texSize.y);
	uvs.z = ((float) TileSize / (float) texSize.x) + uvs.x;

	return uvs;
}