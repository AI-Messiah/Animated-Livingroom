#include "SnowTexture.h"
#include <cassert>

using namespace std;

SnowTexture::SnowTexture()
{
	
}

SnowTexture::~SnowTexture()
{
	delete[] snowSource1;
	delete[] snowSource2;
}

void SnowTexture::update(float deltaTime)
{
	flickTime += deltaTime * 45.0;
	if (flickTime >= 1) {
		flickTime -= 1;
		if (flickTime > 10) flickTime = 0;
		
		unsigned char* imageData;

		imageData = new unsigned char[width * height];
		int apow = 7; //This is OK because the size of the image is divisible by 8
		for (int i = 0; i < width * height; i++) {
			imageData[i] = (((snowSource1[snowPhaze1] ^ snowSource2[snowPhaze2]) & (1 << apow)) > 0) ? 255 : 0;
			apow--;
			if (apow < 0) {
				apow = 7;
				snowPhaze1++;
				if (snowPhaze1 >= snowSize1) snowPhaze1 = 0;
				snowPhaze2++;
				if (snowPhaze2 >= snowSize2) snowPhaze2 = 0;
			}
		}

		if (first) {
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture); // all upcoming GL_TEXTURE_2D operations will affect our texture object (mTexture)

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, imageData);

			glBindTexture(GL_TEXTURE_2D, 0);
			first = false;
		}
		else {
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, imageData);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		delete[] imageData;
	}
}

void SnowTexture::bind(GLuint texUnit)
{
	assert(texUnit >= 0 && texUnit < 32);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

//-----------------------------------------------------------------------------
// Unbind the texture unit passed in as the active texture in the shader
//-----------------------------------------------------------------------------
void SnowTexture::unbind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SnowTexture::start() {
	RequireFile(window, "../Resource/spirit.bin");
	std::ifstream file("../Resource/spirit.bin", std::ios::binary | std::ios::ate);

	if (!file) {
		std::cout << "File not loaded.\n";
	}

	size_t size = file.tellg();
	file.seekg(0, std::ios::beg);

	snowSource1 = new unsigned char[size];
	file.read(reinterpret_cast<char*>(snowSource1), size);

	file.close();
	snowSize1 = static_cast<int>(size);

	RequireFile(window, "../Resource/message.bin");
	std::ifstream file1("../Resource/message.bin", std::ios::binary | std::ios::ate);
	if (!file1) {
		std::cout << "File not loaded.\n";
	}

	size = file1.tellg();
	file1.seekg(0, std::ios::beg);

	snowSource2 = new unsigned char[size];
	file.read(reinterpret_cast<char*>(snowSource2), size);

	file1.close();
	snowSize2 = static_cast<int>(size);
}