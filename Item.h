#pragma once
#include "Texture.h"
class Mesh;
class Texture;
class ShadePass;
class Item {
public:
	Item();
	std::string name;
	Texture* texture;
	Mesh* mesh;
	glm::mat4 worldMatrix;
	void draw();
	void configurate(ShadePass* shadePass);
};

