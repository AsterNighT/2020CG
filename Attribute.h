#pragma once
#include <glew.h>

class Attribute {
public:
	Attribute(GLuint pos, GLenum type, GLuint count, GLuint size, GLboolean normalized = GL_FALSE) :pos(pos), type(type), count(count), size(size), normalized(normalized) {}
	GLuint pos;
	GLenum type;
	GLuint count;
	GLuint size;
	GLboolean normalized;
};

