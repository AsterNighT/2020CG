#pragma once
#include "ShadePass.h"
class ShadePass;

class PointLight {
public:
	PointLight();
	vec3 position;
	vec3 color;
	vec3 target;
	void configurate(ShadePass* shadePass);
	void configurateDepth(ShadePass* depthShader);
	void initialize(ShadePass* colorShader);
private:
	mat4 getViewProjectionMatrix();
	Texture depthMap;
	FrameBuffer depthMapFBO;
};
