#pragma once
#ifndef Texture_h
#define Texture_h

#include <GL/glew.h>
//#include "stb_image.h"
#include "CommonValues.h"

class Texture
{
public:

	Texture();
	Texture(const char* filePath);

	bool loadTexture();
	bool LoadTextureA();
	void useTexture();
	void clearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitdepth;

	const char* filePath;
};

#endif // !Texture_h