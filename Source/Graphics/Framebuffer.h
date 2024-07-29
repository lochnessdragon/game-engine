#pragma once

// System files
#include <cinttypes>

#include <glm/glm.hpp>

#include "GLAPI.h"
#include "texture.h"
#include <core/surface.h>

class Framebuffer : public Surface {
private:
	GLuint Handle;
	glm::uvec2 Size;

	GLuint ColorBuffer;
	GLuint DepthStencilRenderbuffer;
public:
	Framebuffer(uint32_t width, uint32_t height, TextureScaling scaling = TextureScaling::Nearest);

	void Bind() { glBindFramebuffer(GL_FRAMEBUFFER, Handle); };
	void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	void BindColorBuffer() { glBindTexture(GL_TEXTURE_2D, ColorBuffer); };

	glm::ivec2 GetSize() { return Size; }
};