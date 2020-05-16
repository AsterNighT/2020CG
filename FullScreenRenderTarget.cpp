#include "FullScreenRenderTarget.h"

FullScreenRenderTarget::FullScreenRenderTarget() :VAO() {
	this->bind();
	float points[18] = {
		-1,-1,0,
		1,1,0,
		-1,1,0,
		1,1,0,
		1,-1,0,
		-1,-1,0,
	};
	VBO* vbo = new VBO();
	vbo->setData(sizeof(float) * 18, points);
	vbo->setAttrib(0, 3, 3 * sizeof(float));
}
