#pragma once
#include <xaudio2.h>
class Sound
{
public:
	Sound(char* const _path);
	Sound(char* const _path,bool _loop);
	Sound(char* const _path, float _volume);
	Sound(char* const _path, bool _loop, float _volume);
	~Sound();

	void Play();
	void SetLooping(bool _loop);
	void SetVolume(bool _volume);
private :
	float volume;
	bool looping;
	char* const path;
	WAVEFORMATEX wf;
};

