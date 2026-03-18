#ifndef SNOW_TEX
#define SNOW_TEX

#include <fstream>
#include <cstdint>
#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h"

#include "ErrorReport.h"

class SnowTexture
{
public:
	SnowTexture();
	~SnowTexture();

	void update(float deltaTime);
	void bind(GLuint texUnit = 0);
	void unbind(GLuint texUnit = 0);
	void start();
	GLFWwindow* window;
private:
	float flickTime = 1.00001;
	int width = 256;
	int height = 192;
	unsigned char* snowSource1;
	int snowSize1;
	int snowPhaze1 = 0;
	unsigned char* snowSource2;
	int snowSize2;
	int snowPhaze2 = 0;
	bool first = true;
	GLuint mTexture;
};

#endif