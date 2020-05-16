#pragma once
#include "Shader.h"
#include "GLObject.h"
class ShaderProgram:public GLObject {
public:
	ShaderProgram();
	ShaderProgram(const Shader& shader1, const Shader& shader2);
	~ShaderProgram();
	void attachShader(const Shader& shader);
	void link();
	void bind();
};

