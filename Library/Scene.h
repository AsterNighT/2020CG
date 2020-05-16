#pragma once
#include <vector>
#include "ShadePass.h"
#include "SceneModel.h"
#include "../Render.h"
class ShadePass;
class Render;
class SceneModel;
class Material;
class Scene {
public:
	Scene(int width, int height) :width(width), height(height){};
	virtual void draw() = 0;
	virtual void initialize() = 0;
protected:
	std::vector<ShadePass*> mShaderPass;
	std::vector<SceneModel*> components;
	std::vector<Texture*> texturePack;
	std::vector<Material*> materials;
	int width;
	int height;
};