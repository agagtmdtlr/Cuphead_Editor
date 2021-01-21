#include "stdafx.h"
#include "Sound.h"

Sound::Sound()
{
	directSound = 0;
	primaryBuffer = 0;
	secondaryBuffer1 = 0;
}

Sound::Sound(const Sound &)
{
}

Sound::~Sound()
{
}

bool Sound::Initiialize(char * fileName)
{
	bool result;

	//init direct sound primary sound buffer 
	result = InitializeDirectSound();
	if (!result)
	{
		assert(result);
		return false;
	}
	// wav file secondary file buffer load
	result = LoadWaveFile(fileName, &secondaryBuffer1);
	if (!result)
	{
		assert(result);
		return false;
	}
	// loaded wava file play

	result = PlayWaveFile();
	if (!result)
	{
		assert(result);
		return false;
	}
	return true;
}

void Sound::Shutdown()
{
	ShutdownWaveFile(&secondaryBuffer1);

	// shut down directsound api
	ShutdownDirectSound();
	return;
}

void Sound::SetVolume(int volume)
{
	HRESULT result;
	result = secondaryBuffer1->SetVolume((long)volume);
	if (FALSE(result))
	{
		assert(false);
	}
}

bool Sound::InitializeDirectSound()
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// 기본 사운드 장치에 대한 사운드 인터페이스 포인터를 초기화 합니다.
	result = DirectSoundCreate8(NULL, &directSound, NULL);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	//InitializeDirectSound 는 directsound에 대한 인터페이스 포인터와 기본 사운드 버퍼를 가져옵니다.
	//시스템에 모든 사운드 장치를 쿼리한 다음 특정 장치에 대한
	result = directSound->SetCooperativeLevel(Hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	// set up primary buffer 기본 버퍼 설정하빈다.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// get control primary sound device's sound 
	result = directSound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, NULL);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	// set up pirmary wave file buffer format
	// 이 경우 16 비트 스테레오(cd 오디오 형식)로 초당 44, 100 개 샘플로 녹음 된.WAV 파일입니다.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;


	// 기본 버퍼를 지정된 웨이브 형식으로 설정합니다. 
	result = primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	return true;
}

void Sound::ShutdownDirectSound()
{	
	// 기본 사운드 버퍼 포인터를 해제합니다. 
	if (primaryBuffer)
	{
		primaryBuffer->Release();
		primaryBuffer = 0;
	}

	// 직접 사운드 인터페이스 포인터를 해제합니다. 
	if (directSound)
	{
		directSound->Release();
		directSound = 0;
	}
}

bool Sound::LoadWaveFile(char * filename, IDirectSoundBuffer8 ** secondaryBuffer)
{
	//The LoadWaveFile function is what handles loading in a .wav audio file
	//and then copies the data onto a new secondary buffer.
	//If you are looking to do different formats you would replace this function or write a similar one.
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	//
	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		assert(false);
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		assert(false);
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		assert(false);
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		assert(false);
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		assert(false);
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		assert(false);
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
	{
		assert(false);
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	/*if (waveFileHeader.sampleRate != 44100)
	{
		assert(false);
		return false;
	}*/

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		assert(false);
		return false;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		assert(false);
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	//waveFormat.nSamplesPerSec = 44100;
	waveFormat.nSamplesPerSec = waveFileHeader.sampleRate;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = directSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		assert(false);
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		assert(false);
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		assert(false);
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		assert(false);
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return true;
}

void Sound::ShutdownWaveFile(IDirectSoundBuffer8 **secondaryBuffer)
{
	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

bool Sound::PlayWaveFile()
{
	HRESULT result;

	// Set position at the beginning of the sound buffer.
	result = secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%.
	//DSBVOLUME_MIN
	result = secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary sound buffer.
	result = secondaryBuffer1->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
