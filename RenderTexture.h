#pragma once
#include <glew.h>
#include <Common.h>
#include "GLObject.h"

class RenderTexture:public GLObject {
public:
	RenderTexture(int width, int height);
	void bind() const;
	void initialize();
	void genMipmaps();
	void loadImage(GLenum type, GLint mipmapLevel, GLenum loadStruct = GL_RGB, GLenum loadType = GL_UNSIGNED_BYTE, const void* content = NULL);
	static void Clear() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
private:
	int width;
	int height;
	GLenum type;
};

