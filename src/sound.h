/*******************************************************
 * Sound interface -- header file                      *
 *                                                     *
 * Author: Ferry Timmers                               *
 *                                                     *
 * Date: 21:44 6-3-2014                                *
 *                                                     *
 * Description: Audio device output interface.         *
 *   The interface should be transparent to enable     *
 *   crossplatform behaviour.                          *
 *                                                     *
 * License: zlib                                       *
 *                                                     *
 *******************************************************/

#ifndef _SOUND_H
#define _SOUND_H

namespace Sound {

//------------------------------------------------------------------------------

// Initializes the audio interface
bool Initialize(int samplerate, int bitrate, int channels, float buffersize);
void Terminate(); // Cleans up the audio interface

bool Play(); // Start playback, callback will be fired appropriately
void Stop(); // Immediately stop playback, buffers are cleared.

// Implement this function for playback. The ouput parameter points to an
// internal buffer used to store the output data. it is a void pointer as the
// variable type depends on the bitrate (8 = char, 16 = short, etc).
// Length indicated how many samples should be stored in the buffer (note that
// for stereo this is twice as much).
// If the function returns false plackback will halt.
extern bool (*Update)(void *output, size_t length);

// Implement this function to allow error messages to be shown. Not required though.
extern void (*Error)(const char *msg);

//------------------------------------------------------------------------------

} /* namespace Sound */

#endif /* _SOUND_H */

//..............................................................................
