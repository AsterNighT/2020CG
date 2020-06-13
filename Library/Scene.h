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
	virtual void updatetextureMapID(int ItemID, int TextureID) = 0;
	virtual void updateItemWorldMatrix(int ItemID, mat4 WorldMatrix) = 0;
	virtual void updateLight(vec3 pos, vec3 tar, float strengh) = 0;
	virtual void updateExpObj(bool fExpObj, std::string meshFilename) = 0;
	virtual void updateItemName(int ItemID, std::string ItemRename) = 0;
	virtual std::vector<std::string> ImportItems(std::string InportMeshFilename) = 0;
protected:
	std::vector<ShadePass*> mShaderPass;
	std::vector<SceneModel*> components;
	std::vector<Texture*> texturePack;
	std::vector<Material*> materials;
	int width;
	int height;
};