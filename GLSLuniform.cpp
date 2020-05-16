#include "GLSLuniform.h"

GLSLuniform::GLSLuniform(GLuint owner, const std::string& name){
	program = owner;
	this->name = name;
}

GLuint GLSLuniform::getId(){
	return glGetUniformLocation(program ,name.c_str());
}

