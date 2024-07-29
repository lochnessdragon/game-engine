#pragma once

// System
#include <memory>
#include <array>

// Dependencies
#include <glm/glm.hpp>

// Engine Files
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

/**
* We are reimplementing a lot of the 3d renderer methods. A better idea would be to have a base renderer class with derived rendering functionality.
* The base class handles overall stuff, such as clear color, clearing frame, opengl state management, whilst the renderers handle their specific subset of graphics features
* 
* Features:
*  - Batched Rendering
* 
* Planned:
*  - Interleaving (better performance (allegedly)
*/
class Renderer2D {
public:
	Renderer2D();

	void Clear();
	void SetClearColor(float red, float green, float blue, float alpha);
	void SetClearColor(glm::vec4 color);

	void StartFrame(std::shared_ptr<Camera> camera);

	void RenderQuad(glm::mat4 modelMat, std::shared_ptr<Texture> texture, glm::vec4 uvs = { 0.0f, 0.0f, 1.0f, 1.0f }, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	/*void renderSprite(Transform& transform, SpriteComponent& sprite);
	void renderSpriteSet(Transform& transform, std::shared_ptr<Spriteset> spriteset, int id, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void renderTilemap(Transform& transform, Tilemap& tilemap);*/

	void SetLineWidth(float size) { glLineWidth(size); };
	void DrawLine(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void DrawRect(glm::vec2 centerPos, glm::vec2 size, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void DrawCircle(glm::vec2 pos, float radius, float thickness = 1.0f, glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// must be called to present graphics to the screen
	void EndFrame();
	void Flush();

protected:
	// Used to draw Quads to the screen
	static struct QuadVertexInfo {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec4 color;
		float texId;
	};

	// used to draw lines to the screen
	static struct LineVertexInfo {
		glm::vec2 position;
		glm::vec4 color;
	};

private:
	// texture management
	std::shared_ptr<Texture> whiteTex;
	std::shared_ptr<Texture> createWhiteTexture();

	// sprite-batching settings
	static const uint32_t MAX_TEXTURES = 32;
	static const uint32_t MAX_QUADS = 64; // quads per batch
	static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
	static const uint32_t MAX_INDICES = MAX_QUADS * 6;

	// sprite data containers
	Shader spriteShader;
	uint32_t textureSlotIdx = 1;
	std::array<std::shared_ptr<Texture>, MAX_TEXTURES> textures;

	// quad data containers
	std::shared_ptr<Mesh> quadMesh;
	// create a quad mesh
	std::shared_ptr<Mesh> CreateQuadMesh();
	uint32_t quadIdx; // the quad we are currently constructing TODO: Move data into struct
	// [non webgl compatible]: considering rewriting this to map the buffer to a range instead of storing the data on the cpu.
	// stores all of the quad info in a handy array
	QuadVertexInfo quadData[MAX_VERTICES];

	// line-batching settings
	static const uint32_t MAX_LINES = 16; // lines per batch
	static const uint32_t MAX_LINE_VERTS = MAX_LINES * 2;

	// line data containers
	Shader lineShader;
	std::shared_ptr<Mesh> lineMesh;
	uint32_t lineIdx;
	LineVertexInfo lineData[MAX_LINE_VERTS];
	//glm::vec2 lineVerts[MAX_LINE_VERTS];
	//glm::vec4 lineColors[MAX_LINE_VERTS];

	// camera matrix
	glm::mat4 VPMat;

	// batch management functions
	void CheckQuadFlushConditions(std::shared_ptr<Texture> texture, float& textureIndex);
	void AddQuad(glm::mat4 modelMat, glm::vec4 uvs, float textureIdx, glm::vec4 color);

	void CheckLineFlushConditions();

	void FlushQuads();
	void FlushCircles();
	void FlushLines();
};