#include "Texture.h"
#include "Utility.h"
#include <SOIL/SOIL.h>

Texture::Texture(int width, int height) {
	this->width = width;
	this->height = height;
	this->type = 0;
	this->unit = GL_NONE;
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
}

Texture::Texture(ShaderProgram* shader, const char* path, const std::string& name, GLint unit) {
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	initialize(shader, name, unit);
	unsigned char* image = SOIL_load_image(path, &this->width, &this->height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(ShaderProgram* shader, const std::string& name, GLint unit, GLuint id) {
	this->unit = unit;
	this->id = id;
	this->name = name;
	this->shader = shader;
	this->bind();
}

bool Texture::belongTo(int id){
	return this->shader->getId() == id;
}

void Texture::bind(int unit) const {
	shader->bind();
	int u = unit == -1 ? this->unit : unit;
	glActiveTexture(u + GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	GLint loc = glGetUniformLocation(shader->getId(), name.c_str());
	glUniform1i(loc, u);
}

void Texture::initialize(ShaderProgram* shader, const std::string& name, GLint unit, GLboolean isInt) {
	this->shader = shader;
	this->unit = unit;
	this->name = name;
	glBindTexture(GL_TEXTURE_2D, id);
	if (isInt){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}else{
		float fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
}

void Texture::loadImage(GLenum type, GLint mipmapLevel, GLenum loadStruct, GLenum loadType, const void* content) {
	glBindTexture(GL_TEXTURE_2D, id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	this->type = type;
	switch (type) {
	case GL_DEPTH24_STENCIL8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		break;
	default:
		glTexImage2D(GL_TEXTURE_2D, mipmapLevel, type, width, height, 0, loadStruct, loadType, content);
		glGenerateMipmap(GL_TEXTURE_2D);
		break;
	}
}
