#include "Framebuffer.h"

#include <Core/Log.h>

Framebuffer::Framebuffer(uint32_t width, uint32_t height, TextureScaling scaling) : Handle(0), Size(width, height) {
	glGenFramebuffers(1, &Handle);
	this->Bind();

	glGenTextures(1, &ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	GLenum scalingType = GL_LINEAR;
	if (scaling == TextureScaling::Nearest)
		scalingType = GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scalingType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scalingType);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach color buffer to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBuffer, 0);

	// create depth and stencil renderbuffer
	glGenRenderbuffers(1, &DepthStencilRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilRenderbuffer);

	// check for completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::GetRendererLog()->critical("Error creating {}x{} framebuffer.", width, height);
		throw std::runtime_error("Failed to create fbo.");
	}

	this->Unbind();
}