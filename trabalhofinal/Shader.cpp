
#include "Shader.h"
#include "Utils.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
}

void Shader::createFromStrings(const char *vertexCode, const char* fragmentCode) {
	compileShader(vertexCode, fragmentCode);
}

void Shader::createFromFiles(const char* vertexLocation, const char* fragmentLocation) {
	Shader::createFromStrings(Utils::ReadFile(vertexLocation).c_str(), Utils::ReadFile(fragmentLocation).c_str());
}

void Shader::createFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = readFile(vertexLocation);
	std::string geometryString = readFile(geometryLocation);
	std::string fragmentString = readFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	compileShader(vertexCode, geometryCode, fragmentCode);
}

std::string Shader::readFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s. File doesn't exist.\n", fileLocation);
		return content;
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::compileShader(const char* vertexCode, const char* fragmentCode) {
	shaderID = glCreateProgram();

	if (!shaderID) {
		std::cout << "Failed to create shader program!" << std::endl;
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	compileProgram();

	// handle logginf
	/*GLint result = 0;
	GLchar log[1024] = { 0 };


	//Get the link status
	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(log), NULL, log);
		printf("Failed to validate the shader program ! %s \n", log);
		return;
	}*/

	/*std::cout << "Complete." << std::endl;


	// get the uniform shader variables 
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformAmbientColour = glGetUniformLocation(shaderID, "directionaLight.colour");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionaLight.ambientIntensity");*/

}

void Shader::compileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		std::cout << "Error creating shader program!" << std::endl;
		return;
	}

	addShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	addShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	addShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	compileProgram();
}

void Shader::validate()
{
	GLint result = 0;
	GLchar eLog[1024]{};

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return;
	}
}
void Shader::compileProgram()
{
	GLint result = 0;
	GLchar eLog[1024]{};

	glLinkProgram(shaderID);   // generate executable in GPU
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");

	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");

	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePostion = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	uniformShoptLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}

	uniformShoptLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		uniformShoptLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformShoptLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformShoptLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformShoptLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformShoptLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformShoptLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformShoptLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformShoptLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformShoptLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}

	for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMaps[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMaps[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}
}


GLuint Shader::getProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::getModelLocation()
{
	return uniformModel;
}

GLuint Shader::getViewLocation()
{
	return uniformView;
}

GLuint Shader::getAmbientColourLocation()
{
	return uniformDirectionalLight.uniformColour;
}

GLuint Shader::getAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::getEyePositionLocation()
{
	return uniformEyePostion;
}

GLuint Shader::getOmniLightPosLocation()
{
	return uniformOmniLightPos;
}

GLuint Shader::getFarPlaneLocation()
{
	return uniformFarPlane;
}

void Shader::setDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::setPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

		pLight[i].getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}

void Shader::setSpotLights(ShoptLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformShoptLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformShoptLight[i].uniformAmbientIntensity, uniformShoptLight[i].uniformColour,
			uniformShoptLight[i].uniformDiffuseIntensity, uniformShoptLight[i].uniformPosition, uniformShoptLight[i].uniformDirection,
			uniformShoptLight[i].uniformConstant, uniformShoptLight[i].uniformLinear, uniformShoptLight[i].uniformExponent,
			uniformShoptLight[i].uniformEdge);

		sLight[i].getShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, sLight[i].GetFarPlane());
	}
}

void Shader::setTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::useShader()
{
	glUseProgram(shaderID);
}

void Shader::clearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::addShader(GLuint program, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = (GLint)strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	// handle logging 
	GLint result = 0;
	GLchar log[1024] = { 0 };

	std::cout << "Compiling shader..." << std::endl;
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(log), NULL, log);
		std::cout << "Failed to compile the %d shader! \n" << log << std::endl;
		printf("Failed to compile the %d shader:\n %s \n", shaderType, log);
		exit(1);
	}
	std::cout << "Complete." << std::endl;
	glAttachShader(program, theShader);

}

Shader::~Shader() {

	Shader::clearShader();
}

