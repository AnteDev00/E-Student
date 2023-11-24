#pragma once

#ifdef EXPORTS
	#define DLL_EXPORT __declspec(dllexport)
#else
	#define DLL_EXPORT __declspec(dllimport)
#endif

extern "C" char __declspec(dllexport) __stdcall modified_randomChar();
extern "C" void __declspec(dllexport) __stdcall RandomString(int length, char* output);
