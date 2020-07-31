#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat shadowWidth,GLfloat shadowHeight,GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,GLfloat dIntensity)
{
	shadowMap = new ShadowMap();
	shadowMap->Initialise(shadowWidth, shadowHeight);

	colour = glm::vec3(red,green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}
/*
void Light::UseLight(GLfloat ambientIntensityLocation,GLfloat ambientColourLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}*/

Light::~Light()
{

}