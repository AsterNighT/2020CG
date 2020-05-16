#pragma once

#include "Common.h"
#include "../ShaderProgram.h"
#include "Variable.h"
#include "Camera.h"
class Variable;
class Camera;
class VBO;
class ShadePass {
public:
	ShadePass();
	virtual ~ShadePass();

	ShaderProgram* GetShader();
	const std::vector<Variable*>& Variables() const;
	const std::map<std::string, Variable*>& VariablesByName() const;
	Variable* operator[](const std::string& variableName);
	virtual void CompileFromFile(const std::string& filename);
	virtual void initialize() = 0;
	void virtual configurate() = 0;
	VBO* getTranformFeedbackBuffer();
protected:
	ShaderProgram* mShader;
	std::vector<Variable*> mVariables;
	std::map<std::string, Variable*> mVariablesByName;
	VBO* transformFeedback;
};
