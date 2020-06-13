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
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
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

void Window::Control() {
	// Move forward
	vec3 LookAt = normalize(render->getCameraFront());
	//printf("%.2f %.2f %.2f\n", LookAt.x, LookAt.y, LookAt.z);
	float verticalAngle = asin(LookAt.y);
	float horizontalAngle;
	if (LookAt.z != 0)
		horizontalAngle = atan(LookAt.x / LookAt.z);
	else
		horizontalAngle = 3.1415926 / 2;
	if (LookAt.z < 0)
		horizontalAngle += 3.1415926;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		//position += direction * deltaTime * speed;
		verticalAngle += AngleMoveSpeed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		verticalAngle -= AngleMoveSpeed;
		//position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		horizontalAngle -= AngleMoveSpeed;
		//position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		horizontalAngle += AngleMoveSpeed;
		//position -= right * deltaTime * speed;
	}

	render->updateCameraFront(vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	));
	//LookAt = render->getCameraFront();
	//printf("%.2f %.2f %.2f\n\n", LookAt.x, LookAt.y, LookAt.z);
}

void Window::run() {


	while (!glfwWindowShouldClose(window)) {
		myGUI->newframe();
		myGUI->draw(render);

		/* Render here */
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Control();
		render->draw();
		vec3 LookAt = render->getCameraFront();
		//printf("%.2f %.2f %.2f\n", LookAt.x, LookAt.y, LookAt.z);
		
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
