#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

class Light
{
public: 
	Light();
	Light(GLfloat shadowWidht,GLfloat shadowHeight,GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	//void UseLight(GLfloat ambientIntensityLocation,GLfloat ambientColourLocation);
	ShadowMap* getShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};

