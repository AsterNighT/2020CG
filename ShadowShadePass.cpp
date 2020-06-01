#include "ShadowShadePass.h"

void ShadowShadePass::configurate() {
	mShader->bind();
}

void ShadowShadePass::initialize() {
	std::vector<std::string> names = { "lightSpaceMatrix","worldMatrix" };
	for (const std::string& name : names) {
		Variable* variable = new Variable(*this, name);
		mVariables.push_back(variable);
		mVariablesByName.insert(std::pair<std::string, Variable*>(variable->Name(), variable));
	}
}
