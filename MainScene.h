#pragma once

#include "Camera.h"
#include "ColorShadePass.h"
#include "PointLight.h"
#include "Scene.h"
#include "ShadowShadePass.h"
class PointLight;
class MainScene :
	public Scene {
public:
	MainScene(Camera* camera, int width, int height);
	void draw() override;
	void initialize() override;
	void updatetextureMapID(int ItemID, int TextureID) override;
	void updateItemWorldMatrix(int ItemID, mat4 WorldMatrix) override;
	void updateLight(vec3 pos, vec3 tar, float strengh) override;
	void updateExpObj(bool fExpObj, std::string meshFilename) override;
	void updateItemName(int ItemID, std::string ItemRename) override;
	void updateItemSpecpower(int ItemID, float specPower) override;
	std::vector<std::string> ImportItems(std::string InportMeshFilename) override;
	std::string toObjFile();
private:
	Camera* camera;
	PointLight light;
	ShadowShadePass shadowShader;
	ColorShadePass colorShader;
	std::vector<Item*> items;
	std::vector<Texture*>Textures;
	bool fExpObj;
	std::string meshFilename;
};


