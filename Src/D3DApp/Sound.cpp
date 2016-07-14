#include "Sound.h"

Sound::Sound(char* const _path)
	:path(_path), looping(false), volume(1)
{
}

Sound::Sound(char * const _path, bool _loop) :
	 path(_path), looping(_loop),volume(1)
{
}

Sound::Sound(char * const _path, float _volume) :
	path(_path), looping(false), volume(_volume)
{
}

Sound::Sound(char * const _path, bool _loop, float _volume) :
	path(_path), looping(_loop),volume(_volume)
{
}

Sound::~Sound()
{
}

void Sound::Play()
{
	/*waveOutSetVolume(0, 0);
	if (looping) {
		PlaySound((LPCSTR)path, NULL, SND_FILENAME | SND_LOOP);
	}
	else {
		PlaySound((LPCSTR)path, NULL, SND_FILENAME);
	}*/
}

void Sound::SetLooping(bool _loop)
{
	looping = _loop;
}

void Sound::SetVolume(bool _volume)
{
	volume = _volume;
}