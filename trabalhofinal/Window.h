#pragma once
#ifndef Window_h

#define Window_h

#include <GL\glew.h>
#include <GLFW\glfw3.h>
class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getkeys() { return keys; }
	GLfloat getDeltaMouseX();
	GLfloat getDeltaMouseY();


	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastMouseX, lastMouseY, deltaMouseX, deltaMouseY;
	bool mouseFirstMove;

	void createCallbacks();
	static void handlekeys(GLFWwindow* window, int key , int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
	  
};

#endif // !Window_h
