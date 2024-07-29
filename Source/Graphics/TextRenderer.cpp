#include "TextRenderer.h"

#include <Core/Log.h>

#include <Utils/MeshGenerator.h>

/**
* Triangle Idxs
* [0] = Top-left
* [1] = Top-right
* [2] = Bottom-left
* [3] = Bottom-right
*/

TextRenderer::TextRenderer(std::shared_ptr<Surface> surface) : TextShader("res/shaders/glyph.vert", "res/shaders/glyph.frag"), DynamicMesh(CreateDynMesh()), QuadIndex(0), VertexData(), UVData()
{
	glm::ivec2 size = surface->GetSize();
	this->OrthographicMatrix = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, -1.0f, 1.0f);

	// tbh, this was an inelegant solution at best.
	/*window->getWindowResizeHandler().addListener([this](const WindowResizeEventData& data) {
		this->orthoMat = glm::ortho(0.0f, (float) data.width, (float) data.height, 0.0f, -1.0f, 1.0f);
		return false;
	});*/
}


void TextRenderer::Render(const std::string& message, const std::weak_ptr<Font> font, glm::dvec2 pos, double scale, glm::vec4 color)
{
	// set color (flush if poopy)
	if (QuadIndex > 0 && color != CurrentColor) {
		Flush();
	}
	CurrentColor = color;

	// set font (FLUSH IF POOOOOPY!!!!)
	if (QuadIndex > 0 && CurrentFont.lock() && *CurrentFont.lock().get() != *font.lock().get()) { // this could be the site of a memory issue
		Flush();
	}
	CurrentFont = font;

	double x = pos.x;
	double y = pos.y;

	// add more vertex information
	/* For one glyph, we need to know:
	*	(1) UV Coords
	*	(2) Position of the 4 vertices on screen
	*		(a) Spacing info between glyphs on-screen
	*		(b) Baseline info (How far above the baseline?)
	*  Thankfully!?! This is all handled by the font class.
	*/

	const msdf_atlas::FontGeometry& fontInfo = font.lock()->GetFontGeometry();
	msdfgen::FontMetrics metrics = fontInfo.getMetrics();

	double spaceAdvance = fontInfo.getGlyph(' ')->getAdvance() * scale;

	// store the ratio between pixel coordinates and uv coordinates
	double pxToUvWidthRatio = 1.0 / font.lock()->GetAtlasWidth();
	double pxToUvHeightRatio = 1.0 / font.lock()->GetAtlasHeight();

	for (uint32_t i = 0; i < message.size(); i++) {
		// reached end of buffer?
		if (QuadIndex >= MAX_QUADS) {
			// clear buffer
			Flush();
		}

		char c = message[i];
		
		if (c == '\r')
			continue; // skip carriage return
		if (c == '\n') {
			// newline, reset x and update y
			x = 0.0;
			y += metrics.lineHeight * scale;
			continue;
		}
		if (c == ' ') {
			// space character, add a little to x, and grab the next character
			x += spaceAdvance;
			continue;
		}

		auto glyph = fontInfo.getGlyph(c);
		
		const uint32_t floatIdxStart = QuadIndex * 4 * 2; // 4 vertices per quad, 2 floats per vertex

		// grab vertex coordinates and store in buffer
		// don't forget ccw winding order
		double vx_min, vy_min, vx_max, vy_max;
		// left is smallest x, right is largest x
		// however, the top is the smallest y, whilst the bottom is the largest y, because we are defining 0,0 as the top left of the screen
		// not the bottom left, like some plebians!
		glyph->getQuadPlaneBounds(vx_min, vy_max, vx_max, vy_min);
		vx_min = vx_min * scale;
		vy_min = vy_min * scale;
		vx_max = vx_max * scale;
		vy_max = vy_max * scale;

		// correct for improper coordinate system:
		// ^ y                  +--> x
		// |                    |
		// +--> x  (msdf)  vs.  v y     (ours)
		vy_min = -vy_min;
		vy_max = -vy_max;

		// top left
		VertexData[floatIdxStart] = (float) (vx_min + x);
		VertexData[floatIdxStart + 1] = (float) (vy_min + y);
		
		// top right
		VertexData[floatIdxStart + 2] = (float) (vx_max + x);
		VertexData[floatIdxStart + 3] = (float) (vy_min + y);

		// bottom left
		VertexData[floatIdxStart + 4] = (float) (vx_min + x);
		VertexData[floatIdxStart + 5] = (float) (vy_max + y);

		// bottom right
		VertexData[floatIdxStart + 6] = (float) (vx_max + x);
		VertexData[floatIdxStart + 7] = (float) (vy_max + y);

		// grab uv coordinates and store in buffer
		double uvx_min, uvy_min, uvx_max, uvy_max;
		glyph->getQuadAtlasBounds(uvx_min, uvy_min, uvx_max, uvy_max);
		glm::dvec2 uvCoordMin(uvx_min * pxToUvWidthRatio, uvy_min * pxToUvHeightRatio);
		glm::dvec2 uvCoordMax(uvx_max * pxToUvWidthRatio, uvy_max * pxToUvHeightRatio);
		// top left
		UVData[floatIdxStart] = (float) uvCoordMin.x;
		UVData[floatIdxStart + 1] = (float) uvCoordMax.y;

		// top right
		UVData[floatIdxStart + 2] = (float) uvCoordMax.x;
		UVData[floatIdxStart + 3] = (float) uvCoordMax.y;

		// bottom left
		UVData[floatIdxStart + 4] = (float) uvCoordMin.x;
		UVData[floatIdxStart + 5] = (float) uvCoordMin.y;

		// bottom right
		UVData[floatIdxStart + 6] = (float) uvCoordMax.x;
		UVData[floatIdxStart + 7] = (float) uvCoordMin.y;

		QuadIndex++;

		if (i < message.size() - 1) {
			char next_c = message[i + 1];
			double deltaAdvance = 0;
			fontInfo.getAdvance(deltaAdvance, c, next_c);
			x += deltaAdvance * scale;
		}
	}
}

void TextRenderer::Flush() {
	TextShader.Use();
	TextShader.LoadUniform("projMat", OrthographicMatrix);
	TextShader.LoadUniform("textColor", CurrentColor);
	TextShader.LoadUniform("msdfAtlas", 0);

	glActiveTexture(GL_TEXTURE0);
	if (std::shared_ptr<Font> font = this->CurrentFont.lock()) {
		font->BindAtlas();
	}
	else {
		return; // lost the font? no worries
	}

	DynamicMesh->Bind();
	// upload mesh info
	DynamicMesh->SetBuffer(0, QuadIndex * 4 * 2 * sizeof(float), VertexData);
	DynamicMesh->SetBuffer(1, QuadIndex * 4 * 2 * sizeof(float), UVData);
	glDrawElements(GL_TRIANGLES, QuadIndex * 6, GL_UNSIGNED_INT, 0);
	DynamicMesh->Unbind();

	QuadIndex = 0;
}

void TextRenderer::EndFrame() {
	if (QuadIndex > 0)
		Flush();
}

std::shared_ptr<Mesh> TextRenderer::CreateDynMesh() {
	// generate indices [0, 1, 2, 0, 2, 3] = 1 triangle
	unsigned int indices[MAX_INDICES] = {0};
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

	return std::make_shared<Mesh>(MAX_INDICES, indices, MAX_VERTICES * 2, nullptr, MAX_VERTICES * 2, nullptr, MeshUsage::Dynamic); // I am assured OpenGL doesn't copy when a nullptr is passed to glBufferData
}
