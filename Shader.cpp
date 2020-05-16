#include "Shader.h"
#include <ostream>
#include <iostream>
#include <fstream>

Shader::Shader(GLenum type) {
	id = glCreateShader(type);
	this->type = type;
}

Shader::Shader(GLenum type, const std::string& filename):Shader(type){
	std::ifstream file(filename, std::fstream::in);
	std::string content;
	if (file.bad()) {
		throw(new std::runtime_error(std::string("File not found:")+ filename));
	}
	content = std::string((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	file.close();
	setContent(content);
	//printContent();
	compile();
}

Shader::~Shader(){
	glDeleteShader(id);
}

void Shader::setContent(const std::string& str) {
	source = str;
}

void Shader::compile() {
	const GLchar* str = source.c_str();
	glShaderSource(id, 1, &str, NULL);
	glCompileShader(id);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
