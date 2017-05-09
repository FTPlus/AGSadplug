/*******************************************************
 * Adplug wrapper -- header file                       *
 *                                                     *
 * Autor: Ferry "Wyz" Timmers                          *
 *                                                     *
 * Date: 17:21 14-6-2014                               *
 *                                                     *
 * Description: Adlib player module that wraps adplug  *
 *                                                     *
 * License: zlib                                       *
 *                                                     *
 *******************************************************/

#ifndef _ADPLUG_H
#define _ADPLUG_H

namespace AGSAdp {

//------------------------------------------------------------------------------

// Note: maybe have this variable in the future but for now this is fine IMHO
const int SampleRate = 44100;
const int BitRate    = 16;
const int Channels   = 2; // Stereo

const float BufferSize = 20.0; // buffer size in miliseconds
// Note: The bigger the buffer, the less likely playback will skip,
//       the more playback delay there is.

const int SampleSize = (BitRate / 8) * Channels;

//------------------------------------------------------------------------------

bool Initialize(); // Initializes the player, emulator and sound interface
void Terminate();  // Cleans up the player, emulator and sound interface

//------------------------------------------------------------------------------

long Load(const char *filename);
void Unload();
long GetSongLength(const char *filename, long subsong);

void Play(long loop);
void Stop();
void Pause();
long IsPlaying();
long GetPos();
void SetPos(long pos);
long GetSubSong();
void SetSubsong(long subsong);
long GetVolume();
void SetVolume(long volume);
long GetRate();
void SetRate(long rate);

long GetLength();
const char *GetType();
const char *GetTitle();
const char *GetAuthor();
const char *GetDesc();
long GetPatterns();
long GetPattern();
long GetOrders();
long GetOrder();
long GetRow();
long GetSpeed();
long GetMiliHertz();
long GetSubsongs();
long GetInstruments();
const char *GetInstrument(long index);

//------------------------------------------------------------------------------

#define REGISTER(x, a) \
	engine->RegisterScriptFunction("AdPlug::" #x "^" #a, (void *) (AGSAdp::x));

#define SCRIPT_ENTRY            \
	REGISTER(Load,           1) \
	REGISTER(Unload,         0) \
	REGISTER(GetSongLength,  2) \
	REGISTER(Play,           1) \
	REGISTER(Stop,           0) \
	REGISTER(Pause,          0) \
	REGISTER(IsPlaying,      0) \
	REGISTER(GetPos,         0) \
	REGISTER(SetPos,         1) \
	REGISTER(GetSubSong,     0) \
	REGISTER(SetSubsong,     1) \
	REGISTER(GetVolume,      0) \
	REGISTER(SetVolume,      1) \
	REGISTER(GetRate,        0) \
	REGISTER(SetRate,        1) \
	REGISTER(GetLength,      0) \
	REGISTER(GetType,        0) \
	REGISTER(GetTitle,       0) \
	REGISTER(GetAuthor,      0) \
	REGISTER(GetDesc,        0) \
	REGISTER(GetPatterns,    0) \
	REGISTER(GetPattern,     0) \
	REGISTER(GetOrders,      0) \
	REGISTER(GetOrder,       0) \
	REGISTER(GetRow,         0) \
	REGISTER(GetSpeed,       0) \
	REGISTER(GetMiliHertz,   0) \
	REGISTER(GetSubsongs,    0) \
	REGISTER(GetInstruments, 0) \
	REGISTER(GetInstrument,  1)

#define SCRIPT_HEADER                                                                      \
	"#define ADPLUG \"1.0\"\r\n"                                                           \
	"\r\n"                                                                                 \
	"struct AdPlug\r\n"                                                                    \
	"{\r\n"                                                                                \
	"	/// \n"                                                                            \
	"	/// Loads an adlib music file and returns true if succesfull.\r\n"                 \
	"	import static bool Load(String filename);         // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Frees resources of the player.\r\n"                                            \
	"	import static void Unload();                      // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the length of a specific music file (in milliseconds).\r\n"               \
	"	import static void GetSongLength"                                                  \
		"(String filename, int subsong = -1);             // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	\r\n"                                                                              \
	"	/// Starts playback of the currently loaded music file.\r\n"                       \
	"	import static void Play(bool looping = true);     // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Stops playback and resets the player position.\r\n"                            \
	"	import static void Stop();                        // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Stops playback but keeps the player position.\r\n"                             \
	"	import static void Pause();                       // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Return whether the player is currently playin.\r\n"                            \
	"	import static bool IsPlaying();                   // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the current player position (in milliseconds).\r\n"                       \
	"	import static int GetPos();                       // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Sets the current player position (in milliseconds).\r\n"                       \
	"	import static void SetPos(int pos);               // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the current selected subsong. (Some files contain multiple songs)\r\n"    \
	"	import static int GetSubSong();                   // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Selects another subsong (resets player position.\r\n"                          \
	"	import static void SetSubsong(int subsong);       // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the playback volume. (0-100)\r\n"                                         \
	"	import static int GetVolume();                    // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Sets the playback volume. (0-100)\r\n"                                         \
	"	import static void SetVolume(int volume);         // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the playback speed. (100 = normal)\r\n"                                   \
	"	import static int GetRate();                      // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Sets the playback speed. (100 = normal)\r\n"                                   \
	"	import static void SetRate(int rate);             // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	\r\n"                                                                              \
	"	/// Gets the length of the current song (in milliseconds).\r\n"                    \
	"	import static int GetLength();                    // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the file format of the current song.\r\n"                                 \
	"	import static String GetType();                   // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the title of the current song.\r\n"                                       \
	"	import static String GetTitle();                  // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the author of the current song. (not supported by all formats)\r\n"       \
	"	import static String GetAuthor();                 // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets a description of the current song. (not supported by all formats)\r\n"    \
	"	import static String GetDesc();                   // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the number of patterns of the current song (when available).\r\n"         \
	"	import static int GetPatterns();                  // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the pattern number currently playing (when available).\r\n"               \
	"	import static int GetPattern();                   // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the number of orders of the current song. (when available)\r\n"           \
	"	import static int GetOrders();                    // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the order number currently playing (when available).\r\n"                 \
	"	import static int GetOrder();                     // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the row number currently playing (when available).\r\n"                   \
	"	import static int GetRow();                       // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the speed multiplier of the current song. (when available)\r\n"           \
	"	import static int GetSpeed();                     // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the current playback frequency (in millihertz).\r\n"                      \
	"	import static int GetMiliHertz();                 // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the number of subsongs in the current song.\r\n"                          \
	"	import static int GetSubsongs();                  // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the number of instruments in the current song (when available).\r\n"      \
	"	import static int GetInstruments();               // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	/// Gets the n-th instrument name (when available).\r\n"                           \
	"	import static String GetInstrument(int index);    // $AUTOCOMPLETESTATICONLY$\r\n" \
	"	int dummy;                                        // $AUTOCOMPLETEIGNORE$\r\n"     \
	"};\r\n"

//------------------------------------------------------------------------------

} /* namespace AGSAdp */

#endif /* _ADPLUG_H */

//..............................................................................
