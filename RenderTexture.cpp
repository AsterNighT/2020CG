#include "RenderTexture.h"
#include "Utility.h"

RenderTexture::RenderTexture(int width, int height) {
	this->width = width;
	this->height = height;
	this->type = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
}

void RenderTexture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
}

void RenderTexture::initialize() {
	bind();
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void RenderTexture::genMipmaps(){
	bind();
	glGenerateMipmap(GL_TEXTURE_2D);
}

void RenderTexture::loadImage(GLenum type, GLint mipmapLevel, GLenum loadStruct, GLenum loadType, const void* content) {
	glBindTexture(GL_TEXTURE_2D, id);
	this->type = type;
	switch (type) {
	case GL_DEPTH24_STENCIL8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		break;
	default:
		glTexImage2D(GL_TEXTURE_2D, mipmapLevel, type, width, height, 0, loadStruct, loadType, content);
		break;
	}
}
