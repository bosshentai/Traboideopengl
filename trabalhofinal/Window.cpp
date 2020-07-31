#include <iostream>
#include "pch.h"
#include "Window.h"




Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	Window::Initialise();
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

	Window::Initialise();
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	// Configurar propriedades de GLFW windows 
	// Versao OpenGL  

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Perfil principal  = Sem Compatiblidade com versoes anteriores 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Permitir compatibilidade direta
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	 mainWindow = glfwCreateWindow(width, height, "Test windows",nullptr, nullptr);
	if (!mainWindow)
	{
		printf("GlFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// get  informacóes sobre o tamanho do buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set contexto atual
	glfwMakeContextCurrent(mainWindow);


	// manipular teclas do teclado + input do rato
	createCallbacks();

	
	//permitir recursos de exções modernas
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// configurar o tamanho da janela de visualização
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);

	return 0;

}

void Window::createCallbacks() 
{
	glfwSetKeyCallback(mainWindow, handlekeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);

}

GLfloat Window::getDeltaMouseX() {
	GLfloat delta = deltaMouseX;
	//reset to 0 
	deltaMouseX = 0.0f;
	return delta;
}

GLfloat Window::getDeltaMouseY() {
	GLfloat delta = deltaMouseY;
	// reset to 0
	deltaMouseY = 0.0f;
	return delta;
}


void Window::handlekeys(GLFWwindow* window, int key, int code, int action, int mode) 
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if(key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow-> keys[key] = true;
			
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			
		}
	}

}


void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	// get the focused window

	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMove) {
		theWindow->lastMouseX = xPos;
		theWindow->lastMouseY = yPos;
		theWindow->mouseFirstMove = false;
	
	}

	theWindow->deltaMouseX = xPos - theWindow->lastMouseX;
	theWindow->deltaMouseY = theWindow->lastMouseY - yPos;

	theWindow->lastMouseX = xPos;
	theWindow->lastMouseY = yPos;

}




Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}