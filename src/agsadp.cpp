/***********************************************************
 * Adplug wrapper -- See header file for more information. *
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <adplug/adplug.h>
#include <adplug/wemuopl.h>
#include <adplug/silentopl.h>

#include "agsplugin.h"
#include "sound.h"
#include "agsadp.h"

extern IAGSEngine *engine;
#define STRING(x) (engine->CreateScriptString(x))

namespace AGSAdp {

enum PlayMode
{
	pmNo      = 0,
	pmPlaying = 1,
	pmLooping = 2
};

CPlayer *player;    // adplug player instance
Copl *opl;          // adplug OPL interface instance
PlayMode playing;   // is the player currently active
long songlength;    // song length
unsigned long pos;  // portion of the song the player is at
signed long read;   // portion of the song that the player is ahead
float volume;       // current volume (multiplier) of the player
float speed;        // current speed (multilpier) of the player

// *** Invariant 1: playing => player != NULL          ***
// *** Invariant 2: player == NULL => pos == 0         ***
// *** Invariant 2: player == NULL => songlength == -1 ***
// *** Invariant 4: pos == position of player          ***

bool Update(void *buffer, size_t size);
void Error(const char *msg);

//------------------------------------------------------------------------------

bool Initialize()
{
	Sound::Update = AGSAdp::Update;
	Sound::Error = AGSAdp::Error;
	Sound::Initialize(SampleRate, BitRate, Channels, BufferSize);
	
	player = NULL;
	opl = new CWemuopl(SampleRate, BitRate == 16, Channels == 2);
	playing = pmNo;
	
	songlength = -1;
	pos = 0;
	read = 0;
	
	volume = 1.0;
	speed = 1.0;
	
	return opl != NULL;
}

//------------------------------------------------------------------------------

void Terminate()
{
	if (playing)
		Stop();
	
	if (player)
		delete player;
	
	if (opl)
		delete opl;
	
	Sound::Terminate();
}

//==============================================================================

long Load(const char *filename)
{
	if (!opl)
	{
		Error("The OPL emulator failed to load.");
		return 0;
	}
	
	if (playing)
		Sound::Stop();
	
	player = CAdPlug::factory(filename, opl);
	if (!player)
		Error("Failed to open file.");
	else
	{
		songlength = player->songlength();
		pos = 0;
		read = 0;
		
		if (playing)
			Sound::Play();
			
	}
	
	return (player ? 1 : 0);
}

//------------------------------------------------------------------------------

void Unload()
{
	Sound::Stop();
	playing = pmNo;
	songlength = -1;
	pos = 0;
	read = 0;
	
	if (player)
		delete player;
	player = NULL;
}

//------------------------------------------------------------------------------

long GetSongLength(const char *filename, long subsong)
{
	CSilentopl silent;
	CPlayer *p = CAdPlug::factory(filename, &silent);
	
	if (!p)
		return -1;
	
	long length = p->songlength(subsong);
	
	delete p;
	
	return length;
}

//------------------------------------------------------------------------------

void Play(long loop)
{
	if (!player)
		return;
	else if (playing)
	{
		player->rewind();
		pos = 0;
	}
	
	if (!Sound::Play())
		return;
	
	playing = loop ? pmLooping : pmPlaying;
}

//------------------------------------------------------------------------------

void Stop()
{
	if (player)
		player->rewind();
	
	pos = 0;
	read = 0;
	playing = pmNo;
	Sound::Stop();
}

//------------------------------------------------------------------------------

void Pause()
{
	playing = pmNo;
	Sound::Stop();
	SetPos(pos);
}

//------------------------------------------------------------------------------

long IsPlaying()
{
	return playing ? 1 : 0;
}

//------------------------------------------------------------------------------

long GetPos()
{
	return pos;
}

//------------------------------------------------------------------------------

void SetPos(long newpos)
{
	if (!player)
		return;
	
	if (newpos < pos)
	{
		player->rewind();
		pos = 0;
		read = 0;
	}
	
	while ((pos < newpos) && player->update())
		pos += 1000 / player->getrefresh(); // hz -> ms-delay
}

//------------------------------------------------------------------------------

long GetSubSong()
{
	return player ? player->getsubsong() : -1;
}

//------------------------------------------------------------------------------

void SetSubsong(long subsong)
{
	if (player)
	{
		player->rewind(subsong);
		songlength = player->songlength();
	}
	
	pos = 0;
}

//------------------------------------------------------------------------------

long GetVolume()
{
	return volume * 100;
}

//------------------------------------------------------------------------------

void SetVolume(long vol)
{
	volume = ((double) vol) / 100.0;
}

//------------------------------------------------------------------------------

long GetRate()
{
	return speed * 100;
}

//------------------------------------------------------------------------------

void SetRate(long rate)
{
	speed = ((double) rate) / 100.0;
}

//------------------------------------------------------------------------------

long GetLength()
{
	return songlength;
}

//------------------------------------------------------------------------------

const char *GetType()
{
	return STRING(player ? player->gettype().c_str() : "");
}

//------------------------------------------------------------------------------

const char *GetTitle()
{
	return STRING(player ? player->gettitle().c_str() : "");
}

//------------------------------------------------------------------------------

const char *GetAuthor()
{
	return STRING(player ? player->getauthor().c_str() : "");
}

//------------------------------------------------------------------------------

const char *GetDesc()
{
	return STRING(player ? player->getdesc().c_str() : "");
}

//------------------------------------------------------------------------------

long GetPatterns()
{
	return player ? player->getpatterns() : -1;
}

//------------------------------------------------------------------------------

long GetPattern()
{
	return player ? player->getpattern() : -1;
}

//------------------------------------------------------------------------------

long GetOrders()
{
	return player ? player->getorders() : -1;
}

//------------------------------------------------------------------------------

long GetOrder()
{
	return player ? player->getorder() : -1;
}

//------------------------------------------------------------------------------

long GetRow()
{
	return player ? player->getrow() : -1;
}

//------------------------------------------------------------------------------

long GetSpeed()
{
	return player ? player->getspeed() : -1;
}

//------------------------------------------------------------------------------

long GetMiliHertz()
{
	return player ? player->getrefresh() * 1000 : -1;
}

//------------------------------------------------------------------------------

long GetSubsongs()
{
	return player ? player->getsubsongs() : -1;
}

//------------------------------------------------------------------------------

long GetInstruments()
{
	return player ? player->getinstruments() : -1;
}

//------------------------------------------------------------------------------

const char *GetInstrument(long index)
{
	return STRING(player ? player->getinstrument(index).c_str() : "");
}

//==============================================================================
// This function is based on the adplug winamp binding and has possibly a few
// rounding issues. I know a better way to do this but this requires a completly
// different system which I can not be bothered by right now. -Wyz

bool Update(void *output, size_t length)
{
	if (!player || !playing)
		return false;
	
	char *ptr = (char *) output;
	bool stopped = false;
	while (length)
	{
		while (read < 0)
		{
			if (!stopped)
				stopped = !player->update();
			pos += 1000 / player->getrefresh();
			read += SampleRate;
		}
		
		long write = (long) (read / (player->getrefresh() * speed) + 4) & ~3;
		if (write > length)
			write = length;
		
		opl->update((short *) ptr, write);
		read -= (long) ((player->getrefresh() * speed) * write);
		if (volume <= 0.0)
			memset(ptr, 0, write * SampleSize);
		else if (volume < 1.0)
		{
			short *ptr2 = (short *)ptr;
			for (int i = write * 2; i; --i)
				*(ptr2++) *= volume;
		}
		
		ptr += write * SampleSize;
		length -= write;
	}
	
	if (pos >= songlength)
	{
		if (playing != pmLooping)
			playing = pmNo;
		
		player->rewind();
		pos = 0;
		read = 0;
	}
	
	return true;
}

//------------------------------------------------------------------------------

void Error(const char *msg)
{
	const size_t size = strlen(msg) + 13;
	char *buffer = new char[size];
	snprintf(buffer, size, "(AGS Adlib) %s", msg);
	engine->PrintDebugConsole(buffer);
	delete[] buffer;
}

//------------------------------------------------------------------------------

} /* namespace AGSAdp */

//..............................................................................
