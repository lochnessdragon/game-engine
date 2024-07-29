#include "Renderer2D.h"

#include <Core/Platform.h>
#include "GlApi.h"

#include <Core/Log.h>

#include <glm/gtx/string_cast.hpp>

/**
* Triangle Idxs
* [0] = Top-left
* [1] = Top-right
* [2] = Bottom-left
* [3] = Bottom-right
*/

// don't know if I love the white texture's initialization. It's concise, but not readable
Renderer2D::Renderer2D() : whiteTex(createWhiteTexture()), spriteShader("assets/builtin/shaders/sprite_2d.vert", "assets/builtin/shaders/sprite_2d.frag"), lineShader("assets/builtin/shaders/line.vert", "assets/builtin/shaders/line.frag"), quadMesh(CreateQuadMesh()), quadIdx(0), textureSlotIdx(1), lineIdx(0) {
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	textures[0] = whiteTex;

	spriteShader.Use();
	int textureIds[32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	spriteShader.LoadUniformArray("spriteTextures[0]", 32, textureIds);

	// create dynamic mesh for the lines
	lineMesh = std::make_shared<Mesh>(MeshUsage::Dynamic);
	// in order:           [  vPos:  vec2,  vColor: vec4  ]
	lineMesh->AttachBuffer({ {GL_FLOAT, 2}, {GL_FLOAT, 4} }, MAX_LINE_VERTS * sizeof(LineVertexInfo), nullptr);
}

std::shared_ptr<Texture> Renderer2D::createWhiteTexture() {
	TextureSpecification spec;
	spec.height = 1;
	spec.width = 1;
	unsigned char data[4] = {255, 255, 255, 255};
	return std::make_shared<Texture>(spec, data);
}

std::shared_ptr<Mesh> Renderer2D::CreateQuadMesh() {
	// generate indices [0, 1, 2, 0, 2, 3] = 1 triangle
	unsigned int indices[MAX_INDICES] = { 0 };
	for (unsigned int i = 0; i < (MAX_INDICES / 6); i++) {
		unsigned int real_idx = i * 6;
		unsigned int base_index = i * 4;
		indices[real_idx] = base_index;
		indices[real_idx + 1] = base_index + 2;
		indices[real_idx + 2] = base_index + 1;
		indices[real_idx + 3] = base_index + 1;
		indices[real_idx + 4] = base_index + 2;
		indices[real_idx + 5] = base_index + 3;
	}

	// create the mesh.
	auto mesh = std::make_shared<Mesh>(MeshUsage::Dynamic);
	// one index buffer.
	mesh->SetIndexBuffer(MAX_INDICES * (uint32_t) sizeof(unsigned int), indices);
	// one uniform (all-in) vbo.
	// in this order:  [  vPos:  vec3,   vUv:   vec2,   vColor:vec4,   vTexId: float]
	mesh->AttachBuffer({ {GL_FLOAT, 3}, {GL_FLOAT, 2}, {GL_FLOAT, 4}, {GL_FLOAT, 1} }, MAX_VERTICES * sizeof(QuadVertexInfo), nullptr);
	
	return mesh;
}

void Renderer2D::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer2D::SetClearColor(float red, float green, float blue, float alpha) {
	glClearColor(red, green, blue, alpha);
}

void Renderer2D::SetClearColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::StartFrame(std::shared_ptr<Camera> camera) {
	this->Clear();
	// update view and projection matrix
	VPMat = camera->getProjMat() * camera->getViewMat();
}

void Renderer2D::CheckQuadFlushConditions(std::shared_ptr<Texture> texture, float& textureIndex) {
	if (quadIdx >= MAX_QUADS)
		FlushQuads();

	if (!texture) {
		texture = whiteTex;
	}

	textureIndex = -1.0f;
	for (uint32_t i = 1; i < textureSlotIdx; i++)
	{
		if (textures[i] == texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex < 0) {
		if (textureSlotIdx >= MAX_TEXTURES) {
			FlushQuads();
		}

		textureIndex = (float) textureSlotIdx;
		textures[textureSlotIdx] = texture;
		textureSlotIdx++;
	}
}

void Renderer2D::AddQuad(glm::mat4 modelMat, glm::vec4 uvs, float textureIdx, glm::vec4 color) {
	uint32_t vertex_start = quadIdx * 4;

	// quad vertices for easy use in the for loop
	glm::vec4 quadPositions[4] = { glm::vec4(-1.0, 1.0, 0.0, 1.0), 
								   glm::vec4(1.0, 1.0, 0.0, 1.0), 
								   glm::vec4(-1.0, -1.0, 0.0, 1.0), 
								   glm::vec4(1.0, -1.0, 0.0, 1.0) };

	// set all the easy stuff
	for (uint32_t i = 0; i < 4; i++) {
		uint32_t vertex_idx = vertex_start + i;
		quadData[vertex_idx].position = modelMat * quadPositions[i];
		quadData[vertex_idx].texId = textureIdx;
		quadData[vertex_idx].color = color;
	}

	// set the uvs
	uint32_t uv_idx = quadIdx * 4 * 2;

	// top-left
	quadData[vertex_start].uv = { uvs.x, uvs.w };

	// top-right
	quadData[vertex_start + 1].uv = { uvs.z, uvs.w };

	// bottom-left
	quadData[vertex_start + 2].uv = { uvs.x, uvs.y };

	// bottom-right
	quadData[vertex_start + 3].uv = { uvs.z, uvs.y };

	quadIdx++;
}

void Renderer2D::RenderQuad(glm::mat4 modelMat, std::shared_ptr<Texture> texture, glm::vec4 uvs, glm::vec4 color)
{
	float textureIndex = 0;
	CheckQuadFlushConditions(texture, textureIndex);
	AddQuad(modelMat, uvs, textureIndex, color);
}

void Renderer2D::CheckLineFlushConditions() {
	if (lineIdx >= MAX_LINES)
		FlushLines();
}

//void Renderer2D::renderSprite(Transform& transform, SpriteComponent& sprite) {
//	float textureIndex = 0;
//	checkQuadFlushConditions(sprite.texture, textureIndex);
//
//	glm::mat4 modelMat = transform.getModelMat();
//	addQuad(modelMat, sprite.uvs, textureIndex, sprite.color);
//}
//
//void Renderer2D::renderSpriteSet(Transform& transform, std::shared_ptr<Spriteset> spriteset, int id, glm::vec4 color) {
//	float textureIdx = 0;
//	checkQuadFlushConditions(spriteset->texture, textureIdx);
//	glm::vec4 uvs = spriteset->getUVCoords(id);
//
//	addQuad(transform.getModelMat(), uvs, textureIdx, color);
//}
//
//void Renderer2D::renderTilemap(Transform& transform, Tilemap& tilemap) {
//	int baseX = tilemap.getSpriteset()->tileSize / 2;
//	int baseY = tilemap.getSpriteset()->tileSize / 2;
//
//	for (int y = 0; y < tilemap.height; y++) {
//		for (int x = 0; x < tilemap.width; x++) {
//			int tileId = tilemap.get({ x, y });
//			
//			// skip empty
//			if (tileId >= 0) {
//				// we have to retrive the texture index each iteration to ensure we don't overflow the quad buffer
//				float textureIndex = 0;
//				checkQuadFlushConditions(tilemap.getSpriteset()->texture, textureIndex);
//
//				// this is an absolute block of a unit. Basically converts the tile position to an onscreen position in coords.
//				// this is very specific for an orthographic camera
//				// also, it doesn't support transforming the tilemap, which is defininitely a TODO!
//				glm::mat4 modelMat = glm::translate(glm::mat4(1), { 
//					((float)x * tilemap.getSpriteset()->tileSize) + (float) baseX, 
//					((float)y * tilemap.getSpriteset()->tileSize) + (float) baseY, 
//					0.0f }) * glm::scale(glm::mat4(1), glm::vec3(tilemap.getSpriteset()->tileSize / 2));
//
//				// finally, add the quad to the buffer
//				addQuad(modelMat, tilemap.getSpriteset()->getUVCoords(tileId), textureIndex, { 1,1,1,1 });
//			}
//		}
//	}
//}

void Renderer2D::DrawLine(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
	CheckLineFlushConditions();
	lineData[lineIdx].position = pos1;
	lineData[lineIdx].color = color;

	lineData[lineIdx + 1].position = pos2;
	lineData[lineIdx + 1].color = color;

	lineIdx += 2;
}

void Renderer2D::DrawRect(glm::vec2 centerPos, glm::vec2 size, glm::vec4 color) {
	glm::vec2 halfSize = size / 2.0f;
	DrawLine({ centerPos.x - halfSize.x, centerPos.y - halfSize.y }, { centerPos.x + halfSize.x, centerPos.y - halfSize.y }, color);
	DrawLine({ centerPos.x + halfSize.x, centerPos.y - halfSize.y }, { centerPos.x + halfSize.x, centerPos.y + halfSize.y }, color);
	DrawLine({ centerPos.x + halfSize.x, centerPos.y + halfSize.y }, { centerPos.x - halfSize.x, centerPos.y + halfSize.y }, color);
	DrawLine({ centerPos.x - halfSize.x, centerPos.y + halfSize.y }, { centerPos.x - halfSize.x, centerPos.y - halfSize.y }, color);
}

void Renderer2D::DrawCircle(glm::vec2 pos, float radius, float thickness, glm::vec4 color) {

}

void Renderer2D::Flush() {
	if (quadIdx > 0)
		FlushQuads();
	if (lineIdx > 0)
		FlushLines();
}

void Renderer2D::FlushQuads() {
	spriteShader.Use();
	spriteShader.LoadUniform("VPMat", VPMat);

	for (uint32_t i = 0; i < textureSlotIdx; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->Bind();
	}

	quadMesh->Bind();
	// array length * 4 * size of each member. 4 b/c 4 vertices per quad.
	quadMesh->SetBuffer(0, quadIdx * 4 * sizeof(QuadVertexInfo), &quadData);
	glDrawElements(GL_TRIANGLES, quadIdx * 6, GL_UNSIGNED_INT, 0);
	quadMesh->Unbind();

	quadIdx = 0;
	textureSlotIdx = 1; // 0 is white texture
}

void Renderer2D::FlushLines() {
	lineShader.Use();
	lineShader.LoadUniform("VPMat", VPMat);

	lineMesh->Bind();
	lineMesh->SetBuffer(0, lineIdx * sizeof(LineVertexInfo), lineData);
	glDrawArrays(GL_LINES, 0, lineIdx);
	lineMesh->Unbind();

	lineIdx = 0;
}

void Renderer2D::FlushCircles() {

}

void Renderer2D::EndFrame() {
	if(quadIdx > 0 || lineIdx > 0)
		Flush();
}

