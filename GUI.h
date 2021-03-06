#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "MainScene.h"
class GUI {
public:
	void init(GLFWwindow* window);
	void newframe();
	std::string draw(Render* render);
	void RenderDrawData();
	void clearup();
	bool isOrbit();
	bool isFreeViewpoint();
private:
	ImGuiIO *io;
	char items[100][200];
	bool mycameraOrbit;
	bool myfreeViewpoint;
};