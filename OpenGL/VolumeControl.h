#ifndef VOLUME
#define VOLUME

#define GLEW_STATIC
#include "GL/glew.h"

class VolumeControl
{
public:
	~VolumeControl();
	void init();
	void drag(float x, float y);
	void draw();
	void reset();
	float volume = 1;
private:
	float xval = .242;
	float oldx = -1;
	GLuint VAO, VBO;
};

#endif