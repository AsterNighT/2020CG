#pragma once
#include "Render.h"
#include "GLObject.h"

class Texture :public GLObject {
public:
	Texture(int width, int height);
	Texture(ShaderProgram* shader, const char* path, const std::string& name, GLint unit);
	Texture(ShaderProgram* shader, const std::string& name, GLint unit, GLuint id);
	bool belongTo(int id);
	void bind(GLint unit = -1) const;
	void initialize(ShaderProgram* shader, const std::string& name, GLint unit, GLboolean isInt = false);
	void loadImage(GLenum type, GLint mipmapLevel, GLenum loadStruct = GL_RGB, GLenum loadType = GL_UNSIGNED_BYTE, const void* content = NULL);
	static void Clear() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
private:
	int width;
	int height;
	ShaderProgram* shader;
	std::string name;
	GLenum type;
	GLint unit;
};

