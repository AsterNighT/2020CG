#include "Render.h"
#include <fstream>
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
	camera->SetPosition(144.23f, 63.84f, 61.74f);
	camera->SetLookAt(128.56, 56.09, 54.13);
	camera->SetFOV(32);
	camera->UpdateViewMatrix();
	//scene = new Gabardine(camera,width,height);
	//scene->initialize();
}

float Render::aspectRatio() const{
	return static_cast<float>(width) / static_cast<float>(height);
}
