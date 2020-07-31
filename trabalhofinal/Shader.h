#pragma once
#ifndef Shader_h
#define Shader_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Commonvalues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShoptLight.h"

class Shader
{
public:
	Shader();

	void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void createFromStrings(const char* vertexCode, const char* fragmentCode);
	void createFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	
	void validate();
	
	std::string readFile(const char* fileLocation);
	
	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint getAmbientColourLocation();
	GLuint getDiffuseIntensityLocation();
	GLuint getDirectionLocation();
	GLuint getSpecularIntensityLocation();
	GLuint getShininessLocation();
	GLuint getEyePositionLocation();
	GLuint getOmniLightPosLocation();
	GLuint getFarPlaneLocation();

	void setDirectionalLight(DirectionalLight* dLight);
	void setPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void setSpotLights(ShoptLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void setTexture(GLuint textureUnit);
	void setDirectionalShadowMap(GLuint textureUnit);
	void setDirectionalLightTransform(glm::mat4* lTransform);
	void setLightMatrices(std::vector<glm::mat4> lightMatrices);

	void useShader();
	void clearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePostion,
		uniformSpecularIntensity, uniformShininess,
		uniformTexture,
		uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformFarPlane;

	GLuint uniformLightMatrices[6];

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformShoptLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformShoptLight[MAX_SPOT_LIGHTS];

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];


	void compileShader(const char* vertexCode, const char* fragmentCode);
	void compileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void addShader(GLuint program, const char* shaderCode, GLenum shaderType);


	void compileProgram();
};

#endif 