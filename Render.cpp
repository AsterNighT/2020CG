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

float Render::aspectRatio() const{
	return static_cast<float>(width) / static_cast<float>(height);
}
