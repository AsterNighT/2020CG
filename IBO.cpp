#include "IBO.h"

IBO::IBO() {
	glGenBuffers(1, &id);
}

void IBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IBO::setData(GLsizeiptr size, void* source, GLenum usage) {
	this->bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, source, usage);
}

void IBO::clear() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

