#include <iostream>
#include <vector>
#include "Globals.h"

#pragma once
std::vector<byte> make_shellcode(void* string, uintptr_t engine);
DWORD WINAPI CMThread(LPVOID lpParam);
void sendcommand();