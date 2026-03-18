#ifndef MUSIC_PLAYER
#define MUSIC_PLAYER

#define GLEW_STATIC
#include "GL/glew.h"

#include "ErrorReport.h"

#include "irrKlang.h"
using namespace irrklang;

class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();
	void start();
	void setVolume(float vol);
	GLFWwindow* window;
private:
	ISoundEngine* MusicEngine;
	irrklang::ISound* snd;
};

#endif