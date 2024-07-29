#pragma once

#include <string>
#include <memory>

#include <Core/Window.h>
#include <Graphics/Shader.h>
#include <Graphics/Mesh.h>
#include "font.h"

class TextRenderer {
private:
	static const uint32_t MAX_QUADS = 512;
	static const uint32_t MAX_VERTICES = MAX_QUADS * 4; // 4 vertices per quad (but technically 8 floats per quad)
	static const uint32_t MAX_INDICES = MAX_QUADS * 6;

	glm::vec4 CurrentColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	std::weak_ptr<Font> CurrentFont = std::weak_ptr<Font>();

	Shader TextShader;
	// What we want to do here is:
	// 1. Have the indices be the same per-frame
	// 2. But draw a custom length of indices (so that we don't constantly rewrite them)
	// 3. Update vertices per-frame if need be (along with uvs)
	float VertexData[MAX_VERTICES * 2]; // 2 floats per vertex
	float UVData[MAX_VERTICES * 2];
	uint32_t QuadIndex = 0; // that is the number of quads that are already in the vertexData and uvData buffer
	std::shared_ptr<Mesh> DynamicMesh;
	glm::mat4 OrthographicMatrix;

	std::shared_ptr<Mesh> CreateDynMesh();
public:
	TextRenderer(std::shared_ptr<Surface> drawableSurf);

	/*
	* render - displays a message to the screen. Don't forget to call endFrame()
	*	pos is the top-left position of the text in screen-space coordinates
	*	color is the color the text should be displayed in.
	*/
	void Render(const std::string& message, const std::weak_ptr<Font> font, glm::dvec2 pos, double scale, glm::vec4 color);
	
	/*
	* Flush - renders the text to the screen and clears the buffer. Run when endFrame is called or the font (or color) is changed (whichever comes first).
	*/
	void Flush();
	void EndFrame();
};