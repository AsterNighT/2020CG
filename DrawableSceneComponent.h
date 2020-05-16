#pragma once
#include "VAO.h"

class DrawableSceneComponent {
public:
	DrawableSceneComponent(GLenum mode,GLsizei count):drawMode(mode),count(count){
		vao = new VAO();		
	}
	virtual void draw()	{
		vao->bind();
		glDrawArrays(drawMode, 0, count);
		VAO::clear();
	}
	VAO* getVAO()	{
		return vao;
	}
private:
	VAO* vao;
	GLenum drawMode;
	GLsizei count;
};

