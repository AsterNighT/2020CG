#pragma once
#include "ShadePass.h"
class ColorShader :
	public ShadePass {
public:
	ColorShader(Camera*);
	void configurate() override;
	void initialize() override;
	void CompileFromFile(const std::string& filename) override;
protected:
	Camera* camera;
};

