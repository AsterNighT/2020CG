#pragma once
#include "ShadePass.h"
class ShadowShadePass :
	public ShadePass {
public:
	void configurate() override;
	void initialize() override;
};
