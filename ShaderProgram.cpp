#include "ShaderProgram.h"
#include <ostream>
#include <iostream>
#include "Utility.h"

ShaderProgram::ShaderProgram() {
	id = glCreateProgram();
}

ShaderProgram::ShaderProgram(const Shader& shader1, const Shader& shader2):ShaderProgram(){
	attachShader(shader1);
	attachShader(shader2);
}

ShaderProgram::~ShaderProgram(){
	glDeleteProgram(id);
}

void ShaderProgram::attachShader(const Shader& shader) {
	glAttachShader(id, shader.getId());
}

void ShaderProgram::link() {
	glLinkProgram(id);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void ShaderProgram::bind(){
	glUseProgram(id);
}
