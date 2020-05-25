#pragma once
#include "ShadePass.h"
class ColorShadePass :public ShadePass {
public:
	ColorShadePass(Camera* camera);
	void configurate() override;
	void initialize() override;
private:
	Camera* camera;
};

