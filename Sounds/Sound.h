#pragma once


class Sound
{
private:
	// wav file format
	// if other file change format data variable
	struct  WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	Sound();
	Sound(const Sound&);
	~Sound();

	bool Initiialize(char *);
	void Shutdown();

	void SetVolume(int volume);

private:
	bool InitializeDirectSound();
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

private:
	IDirectSound8* directSound;
	IDirectSoundBuffer* primaryBuffer;

	IDirectSoundBuffer8* secondaryBuffer1;
};