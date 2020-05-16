#include "Variable.h"
#include "ShadePass.h"
ShadePass::ShadePass(){
	this->transformFeedback = NULL;
}

ShadePass::~ShadePass() {
	for (Variable* variable : mVariables) {
		delete variable;
	}
	mVariables.clear();
}

ShaderProgram* ShadePass::GetShader() {
	return mShader;
}

const std::vector<Variable*>& ShadePass::Variables() const {
	return mVariables;
}

const std::map<std::string, Variable*>& ShadePass::VariablesByName() const {
	return mVariablesByName;
}

Variable* ShadePass::operator[](const std::string& variableName) {
	const std::map<std::string, Variable*>& variables = VariablesByName();
	Variable* foundVariable = nullptr;

	std::map<std::string, Variable*>::const_iterator found = variables.find(variableName);
	if (found != variables.end()) {
		foundVariable = found->second;
	}

	return foundVariable;
}

void ShadePass::CompileFromFile(const std::string& filename) {
	std::string frag = filename + std::string(".frag");
	std::string vert = filename + std::string(".vert");
	mShader = new ShaderProgram(Shader(GL_VERTEX_SHADER, vert.c_str()), Shader(GL_FRAGMENT_SHADER, frag.c_str()));
	mShader->link();
}

VBO* ShadePass::getTranformFeedbackBuffer() {
	return this->transformFeedback;
}
