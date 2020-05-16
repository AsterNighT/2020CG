#pragma once
#include "GLObject.h"
#include "VBO.h"

class VAO :
	public GLObject {
public:
	VAO();
	~VAO();
	void bind();
	static void clear();
};

