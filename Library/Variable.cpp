#include "Variable.h"
#include "GameException.h"
#include "glm/gtc/type_ptr.hpp"
Variable::Variable(ShadePass& effect, const std::string& name)
	: mShader(effect), mName(name){
}

ShadePass& Variable::GetEffect() {
	return mShader;
}

const std::string& Variable::Name() const {
	return mName;
}

GLint Variable::getLoc() {
	mShader.GetShader()->bind();
	return glGetUniformLocation(mShader.GetShader()->getId(), mName.c_str());
}

Variable& Variable::operator<<(mat4x4 value)
{
	GLint pos = getLoc();
	glUniformMatrix4fv(pos,1,GL_FALSE, value_ptr(value));
	return *this;
}

Variable& Variable::operator<<(vec4 value) {
	GLint pos = getLoc();
	glUniform4f(pos, value.x, value.y, value.z, value.w);

	return *this;
}

Variable& Variable::operator<<(vec3 value)
{
	GLint pos = getLoc();
	glUniform3f(pos, value.x, value.y, value.z);

	return *this;
}

Variable& Variable::operator<<(int value) {
	GLint pos = getLoc();
	glUniform1i(pos, value);

	return *this;
}

Variable& Variable::operator<<(float value) {
	GLint pos = getLoc();
	glUniform1f(pos, value);
	return *this;
}