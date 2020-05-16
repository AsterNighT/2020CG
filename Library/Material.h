#pragma once

#include "Common.h"
#include "Variable.h"
#include "../VAO.h"
#include "../Attribute.h"
#include "../Texture.h"
#define ATTRIBUTE_SEPARATE 0
#define ATTRIBUTE_INTERLEAVED 1
class Texture;
class Mesh;
class Material {
public:
	Material();
	virtual ~Material();

	virtual void Initialize();
	virtual void CreateVertexBuffer(VAO* vertexArray, const Mesh& mesh) = 0;
	virtual GLuint VertexSize() const;
	virtual void setVertexCount(GLuint count);
	GLuint getVertexCount() const;
	virtual void addAttribute(const Attribute& attrib);
	virtual void configurate(int type = ATTRIBUTE_SEPARATE) const;
protected:
	GLuint vertexCount;
	std::vector<Attribute> mAttributes;
	std::vector<Texture> mTextures;
};