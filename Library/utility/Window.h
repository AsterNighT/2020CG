//
// Created by klxjt on 2020/4/19.
//

#pragma once

#include "../../GUI.h"
#include "../../Render.h"
#include "glfw3.h"
#include "../../include/glm/glm.hpp"

const float AngleMoveSpeed = 0.01;
const float PositionMoveSpeed = 0.03;

class Window {
public:
	Window() :Window(1280,720) {

	}

	Window(int width, int height);

	int initialize(int* argcp, char** argv);

	void run();
	//bool updateFreeViewpoint(int guiIsFreeViewpoint);

private:
	int width, height;
	int handle;
	static Render* render;
	GLFWwindow* window;
	bool isFreeViewpoint;
	bool isScreenShot;
	std::string screenShotFilename;
	void Control();
	GUI* myGUI;
}; 
