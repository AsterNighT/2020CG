#include "PointLight.h"

PointLight::PointLight() :depthMap(1024, 1024), depthMapFBO(1024, 1024) {
	strength = 1;
}

void PointLight::configurate(ShadePass* shadePass) {
	depthMap.bind();
	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_BACK);
	*(*shadePass)[std::string("lightPos")] << position;
	*(*shadePass)[std::string("lightStrength")] << strength;
	*(*shadePass)[std::string("lightColor")] << color;
	*(*shadePass)[std::string("lightSpaceMatrix")] << getViewProjectionMatrix();
}

void PointLight::configurateDepth(ShadePass* depthShader) {
	glViewport(0, 0, 1024, 1024);
	depthMapFBO.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glClear(GL_DEPTH_BUFFER_BIT);
	*(*depthShader)[std::string("lightSpaceMatrix")] << getViewProjectionMatrix();
}

void PointLight::initialize(ShadePass* colorShader) {
	depthMap.initialize(colorShader->GetShader(), "shadowMap", 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	depthMap.loadImage(GL_DEPTH24_STENCIL8, 0, GL_DEPTH24_STENCIL8, GL_FLOAT, nullptr);
	depthMapFBO.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthMap.getId(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	depthMapFBO.checkStatus();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void PointLight::UpdateLight(vec3 pos, vec3  tar, float streng){
	position = pos;
	target = tar;
	strength = streng;
}
mat4 PointLight::getViewProjectionMatrix() {
	GLfloat near_plane = 1.0f, far_plane = 1000.0f;
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f),1.0f,1.0f,10.0f);
	glm::mat4 lightView = glm::lookAt(position, target, glm::vec3(0, 1, 0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}
