#include "Texture2D.h"
#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture2D::Texture2D()
	: mTexture(0)
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &mTexture);
}

bool Texture2D::loadTexture(string fileName, bool generateMipMaps)
{
	fileName = "../Textures/" + fileName;

	if (!RequireFile(window, fileName))
		return false;

	int width, height, components;

	stbi_set_flip_vertically_on_load(true);

	// Use stbi image library to load our image
	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha);
	if (width <= 0 || height <= 0)
	{
		stbi_image_free(imageData);
		CorruptedFile(window,
			fileName + "\n\nInvalid image dimensions.");
		return false;
	}
	if (!imageData)
	{
		CorruptedFile(window,
			fileName + "\n\n" + stbi_failure_reason());
		return false;
	}
	
	bool isgray = false;
	

	if (imageData == NULL)
	{
		std::cerr << "Error loading texture '" << fileName << "'" << std::endl;
		return false;
	}

	// Invert image
	
	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (isgray) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, imageData);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture when done so we don't accidentally mess up our mTexture

	return true;
}

void Texture2D::bind(GLuint texUnit)
{
	assert(texUnit >= 0 && texUnit < 32);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

void Texture2D::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
