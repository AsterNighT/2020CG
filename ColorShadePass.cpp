#include "ColorShadePass.h"

ColorShadePass::ColorShadePass(Camera* camera):ShadePass(),camera(camera){
}

void ColorShadePass::configurate(){
	mShader->bind();
	mat4 viewProjectionMatrix = camera->ViewProjectionMatrix();

	////////////////////////////////////////////////////////////////////////////
	//RenderingPass

	float mSpecularPower = 32.0f;

	*mVariablesByName["ViewProjectionMatrix"] << viewProjectionMatrix;
	*mVariablesByName["cameraPos"] << camera->Position();
	//glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->transformFeedback->getId());
}

void ColorShadePass::initialize(){
	std::vector<std::string> names = { "ViewProjectionMatrix", "WorldMatrix","lightMatrix","lightCount","lightPos","lightColor","cameraPos","specPower","ambientPower"};
	for (const std::string& name : names) {
		Variable* variable = new Variable(*this, name);
		mVariables.push_back(variable);
		mVariablesByName.insert(std::pair<std::string, Variable*>(variable->Name(), variable));
	}
}
