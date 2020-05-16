#pragma once
#include "../GLObject.h"

class VBO :public GLObject {
public:
	VBO();
	~VBO();
	void bind();
	void setData(GLsizei size, void* source, GLenum usage = GL_DYNAMIC_DRAW);
	void setAttrib(int pos, int count, int stride,GLboolean normalized = false);
	static void clear();
};
