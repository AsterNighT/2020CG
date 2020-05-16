#pragma once
#include <string>
#include "GLObject.h"
#include <ostream>
#include <iostream>

class Shader :public GLObject {
public:
	Shader(GLenum type);
	Shader(GLenum type,const std::string& filename);
	~Shader();
	void setContent(const std::string& str);
	void printContent()
	{
		std::cout << source << std::endl;
	}
	void compile();
private:
	std::string source;
	GLenum type;
};

