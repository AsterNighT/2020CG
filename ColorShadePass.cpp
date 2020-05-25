#include "ColorShadePass.h"

ColorShadePass::ColorShadePass(Camera* camera):ShadePass(),camera(camera){
}

void ColorShadePass::configurate(){
	mShader->bind();
	mat4 viewProjectionMatrix = camera->ViewProjectionMatrix();
	mat4 viewMatrix = camera->ViewMatrix();
	mat4 projectionMatrix = camera->ProjectionMatrix();

	////////////////////////////////////////////////////////////////////////////
	//RenderingPass

	float mSpecularPower = 32.0f;

	*mVariablesByName["ViewProjectionMatrix"] << viewProjectionMatrix;

	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->transformFeedback->getId());
}

void ColorShadePass::initialize(){
	std::vector<std::string> names = { "ViewProjectionMatrix", "WorldMatrix"};
	for (const std::string& name : names) {
		Variable* variable = new Variable(*this, name);
		mVariables.push_back(variable);
		mVariablesByName.insert(std::pair<std::string, Variable*>(variable->Name(), variable));
	}
}
