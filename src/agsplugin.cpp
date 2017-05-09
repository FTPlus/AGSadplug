/***********************************************************
 * AGS adlib plugin                                        *
 *                                                         *
 * Author: Ferry "Wyz" Timmers                             *
 *                                                         *
 * Date: 14-06-14 17:13                                    *
 *                                                         *
 * Description: Plugin that plays good/crappy old          *
 *              (you choose) adlib songs.                  *
 *                                                         *
 * License: zlib                                           *
 *                                                         *
 ***********************************************************/

#define MIN_EDITOR_VERSION 1
#define MIN_ENGINE_VERSION 17

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#if !defined(_WINDOWS_) && defined(_WIN32)
#define _WINDOWS_
#endif

#include "agsplugin.h"
#include "agsadp.h"

DLLEXPORT int AGS_PluginV2() { return 1; }

//==============================================================================

BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
	switch(reason)
	{
		case DLL_PROCESS_ATTACH:
			break;
		
		case DLL_PROCESS_DETACH:
			break;
		
		case DLL_THREAD_ATTACH:
			break;
		
		case DLL_THREAD_DETACH:
			break;
	}
	
	return TRUE;
}

//==============================================================================

// ***** Design time *****

IAGSEditor *editor; // Editor interface

const char *ourScriptHeader = SCRIPT_HEADER;

//------------------------------------------------------------------------------

LPCSTR AGS_GetPluginName()
{
	return ("Adlib plugin");
}

//------------------------------------------------------------------------------

int AGS_EditorStartup(IAGSEditor *lpEditor)
{
	if (lpEditor->version < MIN_EDITOR_VERSION)
		return (-1);
	
	editor = lpEditor;
	editor->RegisterScriptHeader(ourScriptHeader);
	
	return (0);
}

//------------------------------------------------------------------------------

void AGS_EditorShutdown()
{
	editor->UnregisterScriptHeader(ourScriptHeader);
}

//------------------------------------------------------------------------------

void AGS_EditorProperties(HWND parent)
{
	MessageBoxA(parent,
	           "AdPlug bindings for AGS;\n"
			   "Play Adlib music in AGS games.\n"
			   "Made by Ferry \"Wyz\" Timmers, June 2014",
	           "About",
			   MB_OK | MB_ICONINFORMATION);
}

//==============================================================================

// ***** Run time *****

extern IAGSEngine *engine;
IAGSEngine *engine; // Engine interface

//------------------------------------------------------------------------------

#define STRINGIFY(s) STRINGIFY_X(s)
#define STRINGIFY_X(s) #s

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
	engine = lpEngine;
	
	// Make sure it's got the version with the features we need
	if (engine->version < MIN_ENGINE_VERSION)
		engine->AbortGame("Plugin needs engine version " STRINGIFY(MIN_ENGINE_VERSION) " or newer.");
	
	AGSAdp::Initialize();
	
	// Register functions
	SCRIPT_ENTRY
}

//------------------------------------------------------------------------------

void AGS_EngineShutdown()
{
	AGSAdp::Terminate();
}

//..............................................................................
