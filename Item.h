#pragma once
#include "ShadePass.h"
#include "Material.h"

class Item {
public:
	ShadePass* shader;
	Material* material;
	VAO* vertexArray;
	IBO* indexBuffer;
	void draw();
};

