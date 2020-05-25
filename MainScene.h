#pragma once

#include "Camera.h"
#include "ColorShadePass.h"
#include "Scene.h"
class MainScene :
	public Scene {
public:
	MainScene(Camera* camera, int width, int height);
	void draw() override;
	void initialize() override;
private:
	Camera* camera;
	ColorShadePass colorShader;
	std::vector<Item*> items;
};


