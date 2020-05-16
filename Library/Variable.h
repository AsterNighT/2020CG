#pragma once

#include "Common.h"
#include "ShadePass.h"
class ShadePass;
class Variable {
public:
	Variable(ShadePass& effect, const std::string& name);

	ShadePass& GetEffect();
	const std::string& Name() const;
	GLint getLoc();
	Variable& operator<<(mat4x4 value);
	Variable& operator<<(vec4 value);
	Variable& operator<<(vec3 value);
	Variable& operator<<(int value);
	Variable& operator<<(float value);


private:
	Variable(const Variable& rhs);
	Variable& operator=(const Variable& rhs);

	ShadePass& mShader;
	std::string mName;
};
