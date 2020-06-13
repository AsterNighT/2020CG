#include "Render.h"
#include <fstream>

#include "MainScene.h"
#include "Utility.h"

class Texture;

Render::Render(int width, int height) :width(width), height(height) {
}

void Render::draw() {
	scene->draw();
}



void Render::initializeOpenGLObject() {

}

void Render::initialize() {
	initializeOpenGLObject();
	//Depth

	glEnable(GL_DEPTH_TEST);

	//Blending

	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	this->camera = new Camera(this);
	camera->Initialize();
	camera->SetPosition(3,3,3);
	camera->SetLookAt(0, 0, 0);
	camera->SetFOV(45);
	camera->UpdateViewMatrix();
	scene = new MainScene(camera,width,height);
	scene->initialize();
}
void Render::updatetextureMapID(int ID) {
	scene->updatetextureMapID(ID);
}
void Render::updateItemWorldMatrix(int ItemID, mat4 WorldMatrix) {
	scene->updateItemWorldMatrix(ItemID, WorldMatrix);
}
void Render::updateLight(vec3 pos, vec3 tar, float strengh) {
	scene->updateLight(pos, tar, strengh);
};

void Render::updateCameraPos(vec3 pos) {
	camera->SetPosition(pos);
	camera->SetFOV(45);
	camera->UpdateViewMatrix();
}
void Render::updateCameraFront(vec3 pos) {
	camera->SetLookAt(pos.x, pos.y, pos.z);
	camera->SetFOV(45);
	camera->UpdateViewMatrix();
}

void Render::updateExpObj(bool fExpObj, std::string meshFilename) {
	scene->updateExpObj(fExpObj, meshFilename);
};
float Render::aspectRatio() const{
	return static_cast<float>(width) / static_cast<float>(height);
}
