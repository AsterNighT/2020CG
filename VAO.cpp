#include "VAO.h"

VAO::VAO(){
	glGenVertexArrays(1, &id);
}

VAO::~VAO(){
	glDeleteVertexArrays(1, &id);
}

void VAO::bind(){
	glBindVertexArray(id);
}

void VAO::clear(){
	glBindVertexArray(0);
}

