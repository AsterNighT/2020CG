#pragma once
#include "GLObject.h"
#include "Texture.h"
#include "RenderTexture.h"

class FrameBuffer :
	public GLObject {
public:
	FrameBuffer(int width,int height);
	~FrameBuffer();
	void bind();
	GLboolean checkStatus();
	RenderTexture* getColorBuffer();
	static void Clear() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
protected:
	int width;
	int height;
	RenderTexture* colorBuffer;
	RenderTexture* depthStencilBuffer;
};

