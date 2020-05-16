#include "Material.h"
#include "GameException.h"
#include "Model.h"

Material::Material(){}

Material::~Material() {
}

void Material::Initialize() {
	mAttributes.clear();
}

GLuint Material::VertexSize() const {
	GLuint size = 0;
	for (auto& attrib : mAttributes) {
		size += attrib.size;
	}
	return size;
}

void Material::setVertexCount(GLuint count) {
	vertexCount = count;
}

GLuint Material::getVertexCount() const{
	return vertexCount;
}

void Material::addAttribute(const Attribute& attrib) {
	mAttributes.push_back(attrib);
}

void Material::configurate(int type) const {
	GLuint stride = 0;
	GLuint startPos = 0;
	if (type == ATTRIBUTE_INTERLEAVED) {//1234123412341234
		for (auto& attrib : mAttributes) {
			stride += attrib.size;
		}
		for (auto& attrib : mAttributes) {
			glVertexAttribPointer(attrib.pos, attrib.count, attrib.type, attrib.normalized, stride, (GLvoid*)startPos);
			glEnableVertexAttribArray(attrib.pos);
			startPos += attrib.size;
		}
	}
	if (type == ATTRIBUTE_SEPARATE) {//1111222233334444
		for (auto& attrib : mAttributes) {
			glVertexAttribPointer(attrib.pos, attrib.count, attrib.type, attrib.normalized, attrib.size, (GLvoid*)startPos);
			glEnableVertexAttribArray(attrib.pos);
			startPos += attrib.size*vertexCount;
		}
	}
	for(auto& tex:mTextures){
		tex.bind();
	}

}
