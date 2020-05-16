#pragma once
#include <glew.h>
#include <string>
#include "ShaderProgram.h"

class GLSLuniform {
public:
	GLSLuniform(GLuint owner,const std::string& name);
	GLuint getId();
private:
	GLuint program;
	std::string name;
};

