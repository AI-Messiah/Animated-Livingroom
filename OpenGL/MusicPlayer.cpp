#include "MusicPlayer.h"

MusicPlayer::MusicPlayer()
{
	MusicEngine = createIrrKlangDevice();
}

MusicPlayer::~MusicPlayer()
{
	if (snd) {
		snd->drop();
		snd = 0;
	}
}

void MusicPlayer::start()
{
	if (!RequireFile(window, "../Resource/Silent Night.mp3"))
		return;
	irrklang::ISound* snd = MusicEngine->play2D("../Resource/Silent Night.mp3", true, false, true);
	if (!snd) {
		CorruptedFile(window, "../Resource/Silent Night.mp3");
	}
}

void MusicPlayer::setVolume(float vol)
{
	if (MusicEngine) {
		MusicEngine->setSoundVolume(vol);
	}
}
