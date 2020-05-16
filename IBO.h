#pragma once
#include "GLObject.h"
class IBO :public GLObject {
public:
	IBO();
	void setData(GLsizeiptr size, void* source, GLenum usage = GL_STATIC_DRAW);
	void bind();
	static void clear();
private:
	int count;
};

