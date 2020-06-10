//
// Created by klxjt on 2020/4/19.
//

#include "Window.h"
#include <cstdio>
#include <ostream>
#include <iostream>
#include <glew.h>
#include "Utility.h"


#include <stdio.h>

Render* Window::render = nullptr;
int Window::initialize(int* argcp, char** argv) {
	Window::render = new Render(width, height);
	//init

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(width, height, "Opengl Test", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		exit(-1);
	}
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	glViewport(0, 0, width, height);
	render->initialize();
	GUI tmpmyGUI;
	myGUI = &tmpmyGUI;
	myGUI->init(window);
	return 0;
}

void Window::run() {


	while (!glfwWindowShouldClose(window)) {
		myGUI->newframe();
		myGUI->draw(render);

		/* Render here */
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render->draw();
		
		myGUI->RenderDrawData();
        
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	myGUI->clearup();

	glfwTerminate();
}

Window::Window(int width, int height) :width(width), height(height), handle(0) {

}
