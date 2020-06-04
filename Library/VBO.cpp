#include "VBO.h"

VBO::VBO() {
	glGenBuffers(1, &id);
}

VBO::~VBO() {
	glDeleteBuffers(1, &id);
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
	//glVertexAttribPointer(0, count, GL_FLOAT, GL_FALSE, count * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
}

void VBO::setData(GLsizei size, void* source, GLenum usage) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, source, usage);
}

void VBO::setAttrib(int pos, int count, int stride, GLboolean normalized) {
	this->bind();
	glVertexAttribPointer(pos, count, GL_FLOAT, normalized, stride, (GLvoid*)0);
	glEnableVertexAttribArray(pos);
}

void VBO::clear() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
