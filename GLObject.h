#pragma once
#include <glew.h>

class GLObject {
public:
	virtual GLuint getId() const {
		return id;
	}
protected:
	GLuint id;
};

