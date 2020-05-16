#include "MSAAShader.h"

MSAAShader::MSAAShader():ShadePass(){
}

void MSAAShader::initialize(){
}

void MSAAShader::configurate(){
	mShader->bind();
}
