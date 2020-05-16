#pragma once
#include "ShadePass.h"

class MSAAShader: public ShadePass {
public:
	MSAAShader();
	void initialize() override;
	void configurate() override;
};

