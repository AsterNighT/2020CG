//
// Created by klxjt on 2020/4/19.
//

#pragma once

#include "../../GUI.h"
#include "../../Render.h"
#include "glfw3.h"


class Window {
public:
	Window() :Window(1280,720) {

	}

	Window(int width, int height);

	int initialize(int* argcp, char** argv);

	void run();

private:
	int width, height;
	int handle;
	static Render* render;
	GLFWwindow* window;

	GUI * myGUI;
};
