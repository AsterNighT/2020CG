#include "FrameBuffer.h"
#include "RenderTexture.h"

FrameBuffer::FrameBuffer(int width, int height) :width(width), height(height) {
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	colorBuffer = new RenderTexture(width, height);
	colorBuffer->initialize();
	colorBuffer->loadImage(GL_RGB, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer->getId(), 0);
	depthStencilBuffer = new RenderTexture(width, height);
	depthStencilBuffer->initialize();
	depthStencilBuffer->loadImage(GL_DEPTH24_STENCIL8, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer->getId(), 0);
	checkStatus();
}

FrameBuffer::~FrameBuffer() {
	glDeleteFramebuffers(1, &id);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

GLboolean FrameBuffer::checkStatus() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		return false;
	}
	return true;
}

RenderTexture* FrameBuffer::getColorBuffer() {
	return colorBuffer;
}
