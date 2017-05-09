/************************************************************
 * Sound interface -- See header file for more information. *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <RtAudio.h>

#include "sound.h"

#define ERR(msg,msg2,ret) { report(msg,msg2); return ret; }

namespace Sound {

//------------------------------------------------------------------------------

bool (*Update)(void *output, size_t length) = 0;
void (*Error)(const char *msg) = 0;

int SampleRate, BitRate, Channels;
unsigned int BufferSize;

RtAudio *dac;
bool valid = false;

int callback(void *out, void *in, unsigned int frames, double duration,
	RtAudioStreamStatus status, void *userData);

inline RtAudioFormat getformat(int bitrate);
void report(const char *str, const char *str2);

//------------------------------------------------------------------------------

bool Initialize(int samplerate, int bitrate, int channels, float buffersize)
{
	SampleRate = samplerate;
	BitRate = bitrate;
	Channels = channels;
	BufferSize = ((float) samplerate) * buffersize / 1000.0;
	BufferSize *= channels;
	valid = false;
	
	if (!(dac = new RtAudio))
		ERR("Audio interface failed", NULL, false)
	
	if (dac->getDeviceCount() < 1)
		ERR("No audio devices found.", NULL, false);
	
	RtAudio::StreamParameters sp;
	sp.deviceId = dac->getDefaultOutputDevice();
	sp.nChannels = channels;
	sp.firstChannel = 0;
	RtAudioFormat format = getformat(bitrate);
	
	try
	{
		dac->openStream(&sp, NULL, format, samplerate, &BufferSize, &callback, NULL);
	}
	catch (RtAudioError &e)
		ERR("Could not open output", e.what(), false);
	
	valid = true;
	return true;
}

//------------------------------------------------------------------------------

void Terminate()
{
	if (!valid) return;
	
	Stop();
	
	if (dac->isStreamOpen())
		dac->closeStream();
	
	if (dac)
		delete dac;
	
	valid = false;
}

//------------------------------------------------------------------------------

bool Play()
{
	if (!valid) return false;
	
	try
	{
		dac->startStream();
	}
	catch (RtAudioError &e)
		ERR("Could not start output stream", e.what(), false);
	
	return true;
}

//------------------------------------------------------------------------------

void Stop()
{
	if (!valid) return;
	
	try
	{
		dac->stopStream();
	}
	catch (RtAudioError &e)
		ERR("Could not stop output stream", e.what(), );
}

//==============================================================================

int callback(void *out, void *in, unsigned int frames, double duration,
	RtAudioStreamStatus status, void *userData)
{
	if (status)
		report("Buffer underrun detected.", NULL);
	
	if (Update)
		return Update(out, frames) ? 0 : 1;
	
	return 0;
}

//==============================================================================

inline RtAudioFormat getformat(int bitrate)
{
	switch (bitrate)
	{
		case 8:
			return RTAUDIO_SINT8;
		case 16:
			return RTAUDIO_SINT16;
		case 24:
			return RTAUDIO_SINT24;
		case 32:
			return RTAUDIO_SINT32;
		default:
			return 0;
	}
}

//------------------------------------------------------------------------------

void report(const char *str, const char *str2)
{
	if (!Error)
		return;
	
	if (!str2)
	{
		Error(str);
		return;
	}
	
	const size_t size = strlen(str) + strlen(str2) + 5;
	char *buffer = new char[size];
	snprintf(buffer, size, "%s -- %s", str, str2);
	
	Error(buffer);
	delete[] buffer;
}

//------------------------------------------------------------------------------

} /* namespace Sound */

//..............................................................................
