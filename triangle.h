#pragma once
#include "DrawableSceneComponent.h"
#include <glew.h>

class triangle :
	public DrawableSceneComponent {
public:
	triangle(GLfloat* vertices);
	void draw() override;
private:
	GLfloat vertices;
};

