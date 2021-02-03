#include "stdafx.h"
#include "ConsoleManager.h"


#define EngineClientPtrSig "\x8B\x0D\x00\x00\x00\x00\x8B\x01\xFF\x90\x00\x00\x00\x00\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xFF\x50\x14"
#define EngineClientPtrMask "xx????xxxx????x????x????xxx"

std::vector<byte> make_shellcode(void* string, uintptr_t engine)
{
	std::vector<byte> shellcode;

	byte code[] = { "\x8b\x0d\x00\x00\x00\x00\x68\x00\x00\x00\x00\x8b\x01\x8b\x80\xb0\x01\x00\x00\xff\xd0\xc3" }; /* just a reconstructed thiscall */

	shellcode.insert(shellcode.end(), &code[0], &code[22]);

	*(uint32_t*)(&shellcode.at(2)) = engine;
	*(void**)(&shellcode.at(7)) = string;

	return shellcode;
}

void sendcommand()
{
	uintptr_t test = Globals::gamememory.FindPattern(Globals::dwClient, Globals::clientmodule.modBaseSize, EngineClientPtrSig, EngineClientPtrMask);

	uintptr_t engine = Globals::gamememory.Read<uintptr_t>((DWORD)(test + 2));

	std::string command = Globals::command;

	std::vector<byte> inject = make_shellcode(Globals::gamememory.AllocateWrite(command.c_str()), engine);
	void* map = Globals::gamememory.AllocateWrite(&inject.at(0), inject.size());

	if (getgamestate() == 6)
	{
		HANDLE hThread = Globals::gamememory.MakeThreadInProcess(map);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
	Globals::gamememory.FreeBlock(map, inject.size());
}

DWORD WINAPI CMThread(LPVOID lpParam)
{
	uintptr_t test = Globals::gamememory.FindPattern(Globals::dwClient, Globals::clientmodule.modBaseSize, EngineClientPtrSig, EngineClientPtrMask);

	uintptr_t engine = Globals::gamememory.Read<uintptr_t>((DWORD)(test + 2));

	std::string command = Globals::command;

	std::vector<byte> inject = make_shellcode(Globals::gamememory.AllocateWrite(command.c_str()), engine);
	void* map = Globals::gamememory.AllocateWrite(&inject.at(0), inject.size());

	while (getgamestate() == 6)
	{
		HANDLE hThread = Globals::gamememory.MakeThreadInProcess(map);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		Sleep(1000);
	}

	Globals::gamememory.FreeBlock(map, inject.size());
	return 0;
}