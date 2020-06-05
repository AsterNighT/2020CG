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
	std::string toObjFile();
private:
	Camera* camera;
	PointLight light;
	ShadowShadePass shadowShader;
	ColorShadePass colorShader;
	std::vector<Item*> items;
};


