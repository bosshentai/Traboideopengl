// OpenGLCourseApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "Texture.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShoptLight.h"
#include "Material.h"
#include "Model.h"

#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture testeTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackhawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
ShoptLight spotLights[MAX_SPOT_LIGHTS];

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat blackhawkAngle = 0.0f;

// Vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment shader
static const char* fShader = "Shaders/shader.frag";

// A helper function to calculate normal averages
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	// for each triangle
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		// the edge connecting indices[i] and indices[i + 1]
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		// the edge connecting indices[i] and indices[i + 2]
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		// now calculate the normal based on the two edges above
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		// now need to save the normal value above to a proper vertex location
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		// x   y      z			u     v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.f, 1.0f, 0.f,			0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorindices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	//teste
	unsigned int paredeindices[] = {
		0,2,1,
		3,1,2
	};

	GLfloat paredesVertices[] = {
		// x   y      z			u     v			nx	  ny    nz
		-5.0f,0.0f,0.0f,		0.0f,0.0f,		0.0f,0.0f,0.0f,
		-5.0f,0.0f,3.0f,		0.0f,3.0f,		0.0f,0.0f,0.0f,
		-5.0f,3.0f,0.0f,		3.0f,0.0f,		0.0f,0.0f,0.0f,
		-5.0f,3.0f,3.0f,		3.0f,3.0f,		0.0f,0.0f,0.0f
	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->createMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->createMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->createMesh(floorVertices, floorindices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->createMesh(paredesVertices, paredeindices, 32, 6);
	meshList.push_back(obj4);
	
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->createFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.createFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");

	omniShadowShader = Shader();
	omniShadowShader.createFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.useTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->renderMesh();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.f, 4.f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.useTexture();
	dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->renderMesh();


	// teste 
	


	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.useTexture();
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->renderMesh();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	xwing.renderModel();

	blackhawkAngle += 0.1f;
	if (blackhawkAngle > 360.0f)
	{
		blackhawkAngle = 0.1f;
	}

	model = glm::mat4(1.0);
	/*model = glm::rotate(model, -blackhawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));*/
	model = glm::translate(model, glm::vec3(2.0f, -1.0f, 0.0f));
	/*model = glm::rotate(model, -20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));*/
	model = glm::rotate(model, -180.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	blackhawk.renderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.useShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.getModelLocation();
	directionalShadowShader.setDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.useShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.getModelLocation();
	uniformOmniLightPos = omniShadowShader.getOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.getFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.setLightMatrices(light->CalculateLightTransform());

	omniShadowShader.validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	glViewport(0, 0, 1920, 1080);

	// clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].useShader();

	uniformModel = shaderList[0].getModelLocation();
	uniformProjection = shaderList[0].getProjectionLocation();
	uniformView = shaderList[0].getViewLocation();
	uniformEyePosition = shaderList[0].getEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].getSpecularIntensityLocation();
	uniformShininess = shaderList[0].getShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].setDirectionalLight(&mainLight);
	shaderList[0].setPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].setDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].setTexture(1);
	shaderList[0].setDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.0f;
	/*spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/

	shaderList[0].validate();

	RenderScene();
}

int main()
{
	mainWindow = Window(1024, 768);
	mainWindow.Initialise();

	/*glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);*/
	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/grass.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	

	shinyMaterial = Material(10.0f, 256);
	dullMaterial = Material(0.3f, 4);

	/*xwing = Model();
	xwing.loadModel("Models/x-wing.obj");*/

	blackhawk = Model();
	/*blackhawk.loadModel("Models/uh60.obj");*/
	blackhawk.loadModel("Models/pclomba.obj");

	mainLight = DirectionalLight(2048, 2048,
		1.0f, 0.53f, 0.3f,
		0.1f, 0.9f,
		-10.0f, -12.0f, 18.5f);

	pointLights[0] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 2.0f, 0.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
		0.01f, 100.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		-4.0f, 3.0f, 0.0f,
		0.3f, 0.1f, 0.1f);
	pointLightCount++;

	spotLights[0] = ShoptLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;
	spotLights[1] = ShoptLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
/*	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
*/

	skyboxFaces.push_back("Textures/Skybox/left.jpg");
	skyboxFaces.push_back("Textures/Skybox/right.jpg");
	skyboxFaces.push_back("Textures/Skybox/top.jpg");
	skyboxFaces.push_back("Textures/Skybox/bottom.jpg");
	skyboxFaces.push_back("Textures/Skybox/back.jpg");
	skyboxFaces.push_back("Textures/Skybox/front.jpg");
	


	skybox = Skybox(skyboxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),(float) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();  // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime;
		lastTime = now;

	
		// Get and handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getkeys(), deltaTime);
		camera.mouseControl(mainWindow.getDeltaMouseX(), mainWindow.getDeltaMouseY());
/*
		if (mainWindow.getkeys()[GLFW_KEY_E])
		{
			spotLights[0].Tooggle();
			mainWindow.getkeys()[GLFW_KEY_E] = false;
		}
*/
	DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(projection, camera.calculateViewMatrix());

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
