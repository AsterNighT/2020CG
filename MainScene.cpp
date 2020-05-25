#include "MainScene.h"
#include "Utility.h"
MainScene::MainScene(Camera* camera, int width, int height):Scene(width,height), colorShader(camera){
	this->camera = camera;
}

void MainScene::draw(){
	colorShader.GetShader()->bind();
	colorShader.configurate();
	for(auto item:items){
		item->configurate(&colorShader);
		item->draw();
	}
}

void MainScene::initialize(){
	colorShader.CompileFromFile("shader/color0");
	colorShader.initialize();
	auto t = Model::loadModel("obj/cube.obj");
	t.at(0)->name = "cube1";
	items.insert(items.end(), t.begin(), t.end());
	t = Model::loadModel("obj/cube.obj");
	t.at(0)->name = "cube2";
	t.at(0)->worldMatrix = glm::translate(glm::identity<mat4>(),vec3(2, 0, -2));
	items.insert(items.end(),t.begin(), t.end());
}
